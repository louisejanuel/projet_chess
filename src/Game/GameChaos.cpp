#include "GameChaos.hpp"
#include "ChaosUtils.hpp"
#include "Pieces/BerolinaPawn.hpp"
#include <cstdlib>
#include <vector>

// initialize board and apply chaos mutations
void GameChaos::setup() 
{
    setup_classic_pieces();

    m_collapsing_square_idx = rand() % 64;

    int numBerolinaBlack = tirageBinomial(8, 0.25);
    std::vector<int> blackPawns = {8, 9, 10, 11, 12, 13, 14, 15};
    for (int i = 0; i < numBerolinaBlack; ++i) {
        int r = rand() % blackPawns.size();
        int idx = blackPawns[r];
        m_board.set_piece(idx, std::make_unique<BerolinaPawn>(Color::Black));
        blackPawns.erase(blackPawns.begin() + r);
    }

    int numBerolinaWhite = tirageBinomial(8, 0.25);
    std::vector<int> whitePawns = {48, 49, 50, 51, 52, 53, 54, 55};
    for (int i = 0; i < numBerolinaWhite; ++i) {
        int r = rand() % whitePawns.size();
        int idx = whitePawns[r];
        m_board.set_piece(idx, std::make_unique<BerolinaPawn>(Color::White));
        whitePawns.erase(whitePawns.begin() + r);
    }

    m_chaos_turn_trigger = tirageGeometrique(0.05);
}

// execute a move
bool GameChaos::play_move(int fromIdx, int toIdx, Type promotion) 
{
    Piece* p = m_board.get_piece(fromIdx);
    
    if (p == nullptr || p->get_color() != m_current_turn) {
        return Game::play_move(fromIdx, toIdx, promotion);
    }

    // bernoulli distribution (25% survival chance on collapsing square)
    if (toIdx == m_collapsing_square_idx) 
    {
        bool survives = tirageBernoulli(0.25); 

        if (!survives) {
            if (p->get_type() == Type::King) {
                m_state = (p->get_color() == Color::White) ? GameState::BlackWins : GameState::WhiteWins;
            }
            else if (!m_board.is_empty(toIdx) && m_board.get_piece(toIdx)->get_type() == Type::King) {
                m_state = (m_board.get_piece(toIdx)->get_color() == Color::White) ? GameState::BlackWins : GameState::WhiteWins;
            }

            m_board.remove_piece(fromIdx); 
            m_board.remove_piece(toIdx);   
            
            m_current_turn = (m_current_turn == Color::White) ? Color::Black : Color::White;
            return true; 
        }
    }

    // exponential distribution (random piece promotion)
    bool is_pawn = (p->get_type() == Type::Pawn || p->get_type() == Type::BerolinaPawn);
    int target_y = toIdx / 8;
    bool reaching_end = (target_y == 0 || target_y == 7);

    if (is_pawn && reaching_end) {
        double x = tirageExponentiel(0.7675);
        if (x < 1.0)      promotion = Type::Knight;
        else if (x < 2.0) promotion = Type::Bishop;
        else if (x < 3.0) promotion = Type::Rook;
        else              promotion = Type::Queen;
        
        m_last_promoted = promotion;
    }

    bool move_success = Game::play_move(fromIdx, toIdx, promotion);

    // update game state and check for rules inversion (lose to win)
    if (move_success) 
    {
        m_turn_count++;
        
        if (m_turn_count == m_chaos_turn_trigger) {
            m_inverted_rules = true;
        }

        if (m_inverted_rules && m_state != GameState::Playing) 
        {
            if (m_state == GameState::WhiteWins) {
                m_state = GameState::BlackWins;
            } 
            else if (m_state == GameState::BlackWins) {
                m_state = GameState::WhiteWins;
            }
        }
    }

    return move_success;
}