#include "Chessboard.hpp"
#include "Pieces/Queen.hpp"
#include "Pieces/Rook.hpp"
#include "Pieces/Bishop.hpp"
#include "Pieces/Knight.hpp"

void Chessboard::move_piece_basic(int fromIdx, int toIdx, Type promotion) 
{
    m_pieces[toIdx] = std::move(m_pieces[fromIdx]);
    if (m_pieces[toIdx]) m_pieces[toIdx]->set_moved(true);

    if (promotion != Type::None) {
        Color c = m_pieces[toIdx]->get_color();
        if (promotion == Type::Queen)       m_pieces[toIdx] = std::make_unique<Queen>(c);
        else if (promotion == Type::Rook)   m_pieces[toIdx] = std::make_unique<Rook>(c);
        else if (promotion == Type::Bishop) m_pieces[toIdx] = std::make_unique<Bishop>(c);
        else if (promotion == Type::Knight) m_pieces[toIdx] = std::make_unique<Knight>(c);
    }
}