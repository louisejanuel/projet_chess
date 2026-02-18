#include "Chessboard.hpp"
#include "Pieces/Pawn.hpp"
#include "Pieces/piece.hpp"

Chessboard::Chessboard() : m_current_turn(Color::White) {
    // black pieces initialization
    for (int i = 0; i < 8; ++i) {
        m_pieces[8 + i] = std::make_unique<Pawn>(Color::Black);
    }

    // white pieces initialization
    for (int i = 0; i < 8; ++i) {
        m_pieces[48 + i] = std::make_unique<Pawn>(Color::White);
    }
}

bool Chessboard::move_piece(int fromIdx, int toIdx)
{
    if (fromIdx < 0 || fromIdx >= 64 || toIdx < 0 || toIdx >= 64) return false;
    
    // cannot move from an empty square
    if (is_empty(fromIdx)) return false;

    Piece* p = get_piece(fromIdx);
    // cannot move opponent's piece
    if (p->get_color() != m_current_turn) {
        return false;
    }

    // mark the piece as moved
    if (p) {
        p->set_moved(true);
    }

    // move the unique_ptr
    m_pieces[toIdx] = std::move(m_pieces[fromIdx]);

    //change turn
    m_current_turn = (m_current_turn == Color::White) ? Color::Black : Color::White;

    return true;
}