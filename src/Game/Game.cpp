#include "Game.hpp"
#include "Pieces/Pawn.hpp"
#include "Pieces/Rook.hpp"
#include "Pieces/Knight.hpp"
#include "Pieces/Bishop.hpp"
#include "Pieces/Queen.hpp"
#include "Pieces/King.hpp"

void Game::setup_classic_pieces() 
{
    for (int i = 0; i < 8; ++i) m_board.set_piece(8 + i, std::make_unique<Pawn>(Color::Black));
    for (int i = 0; i < 8; ++i) m_board.set_piece(48 + i, std::make_unique<Pawn>(Color::White));

    m_board.set_piece(2, std::make_unique<Bishop>(Color::Black));
    m_board.set_piece(5, std::make_unique<Bishop>(Color::Black));
    m_board.set_piece(58, std::make_unique<Bishop>(Color::White));
    m_board.set_piece(61, std::make_unique<Bishop>(Color::White));

    m_board.set_piece(0, std::make_unique<Rook>(Color::Black));
    m_board.set_piece(7, std::make_unique<Rook>(Color::Black));
    m_board.set_piece(56, std::make_unique<Rook>(Color::White));
    m_board.set_piece(63, std::make_unique<Rook>(Color::White));

    m_board.set_piece(1, std::make_unique<Knight>(Color::Black));
    m_board.set_piece(6, std::make_unique<Knight>(Color::Black));
    m_board.set_piece(57, std::make_unique<Knight>(Color::White));
    m_board.set_piece(62, std::make_unique<Knight>(Color::White));

    m_board.set_piece(3, std::make_unique<Queen>(Color::Black));
    m_board.set_piece(59, std::make_unique<Queen>(Color::White));
    m_board.set_piece(4, std::make_unique<King>(Color::Black));
    m_board.set_piece(60, std::make_unique<King>(Color::White));
}

bool Game::play_move(int fromIdx, int toIdx, Type promotion) 
{
    if (fromIdx < 0 || fromIdx >= 64 || toIdx < 0 || toIdx >= 64) return false;
    if (m_board.is_empty(fromIdx)) return false;

    Piece* p = m_board.get_piece(fromIdx);
    if (p->get_color() != m_current_turn) return false;

    if (!m_board.is_empty(toIdx)) {
        Piece* target = m_board.get_piece(toIdx);
        if (target->get_type() == Type::King) {
            m_state = (target->get_color() == Color::White) ? GameState::BlackWins : GameState::WhiteWins;
        }
    }

    m_board.move_piece_basic(fromIdx, toIdx, promotion);
    m_current_turn = (m_current_turn == Color::White) ? Color::Black : Color::White;

    return true;
}