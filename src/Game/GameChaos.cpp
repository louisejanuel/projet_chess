#include "GameChaos.hpp"
#include "ChaosUtils.hpp"
#include "Pieces/BerolinaPawn.hpp"
#include <cstdlib>
#include <vector>

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
}

bool GameChaos::play_move(int fromIdx, int toIdx, Type promotion) 
{
    Piece* p = m_board.get_piece(fromIdx);
    
    if (p == nullptr || p->get_color() != m_current_turn) {
        return Game::play_move(fromIdx, toIdx, promotion);
    }

    // Loi de Bernoulli pour le trou
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

    return Game::play_move(fromIdx, toIdx, promotion);
}