#pragma once
#include <array>
#include <memory>
#include "Pieces/Piece.hpp"
#include "Utils.hpp"

// represent the physical chessboard and store the pieces
class Chessboard {
private:
    std::array<std::unique_ptr<Piece>, 64> m_pieces;

public:
    // get a raw pointer to a piece at a specific index
    Piece* get_piece(int index) const {
        if (index < 0 || index >= 64) return nullptr;
        return m_pieces[index].get();
    }

    // place a new piece
    void set_piece(int index, std::unique_ptr<Piece> piece) {
        if (index >= 0 && index < 64) m_pieces[index] = std::move(piece);
    }

    // remove a piece
    void remove_piece(int index) {
        if (index >= 0 && index < 64) m_pieces[index].reset();
    }

    bool is_empty(int index) const {
        if (index < 0 || index >= 64) return true;
        return m_pieces[index] == nullptr;
    }

    // execute a basic physical move
    void move_piece_basic(int fromIdx, int toIdx, Type promotion = Type::None);
};