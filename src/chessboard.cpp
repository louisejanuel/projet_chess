#include "Chessboard.hpp"
#include "Pieces/Pawn.hpp"
#include "Pieces/piece.hpp"
#include "Pieces/Bishop.hpp"
#include "Pieces/Rook.hpp"
#include "Pieces/Knight.hpp"
#include "Pieces/Queen.hpp"
#include "Pieces/King.hpp"

Chessboard::Chessboard() : m_current_turn(Color::White) {
    // INITIALIZATION
    // black pawns
    for (int i = 0; i < 8; ++i) {
        m_pieces[8 + i] = std::make_unique<Pawn>(Color::Black);
    }

    // white pawns
    for (int i = 0; i < 8; ++i) {
        m_pieces[48 + i] = std::make_unique<Pawn>(Color::White);
    }

    // black bishops
    m_pieces[2] = std::make_unique<Bishop>(Color::Black);
    m_pieces[5] = std::make_unique<Bishop>(Color::Black);

    // white bishops
    m_pieces[58] = std::make_unique<Bishop>(Color::White);
    m_pieces[61] = std::make_unique<Bishop>(Color::White);

    // black rooks
    m_pieces[0] = std::make_unique<Rook>(Color::Black);
    m_pieces[7] = std::make_unique<Rook>(Color::Black);

    // white rooks
    m_pieces[56] = std::make_unique<Rook>(Color::White);
    m_pieces[63] = std::make_unique<Rook>(Color::White);

    // black knights
    m_pieces[1] = std::make_unique<Knight>(Color::Black);
    m_pieces[6] = std::make_unique<Knight>(Color::Black);

    // white knights
    m_pieces[57] = std::make_unique<Knight>(Color::White);
    m_pieces[62] = std::make_unique<Knight>(Color::White);

    // queens
    m_pieces[3] = std::make_unique<Queen>(Color::Black);
    m_pieces[59] = std::make_unique<Queen>(Color::White);

    // kings
    m_pieces[4] = std::make_unique<King>(Color::Black);
    m_pieces[60] = std::make_unique<King>(Color::White);
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