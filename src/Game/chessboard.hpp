#pragma once
#include <array>
#include <memory>
#include "Pieces/Piece.hpp"
#include "Utils.hpp"

class Chessboard {
private:
    std::array<std::unique_ptr<Piece>, 64> m_pieces;

public:
    Piece* get_piece(int index) const {
        if (index < 0 || index >= 64) return nullptr;
        return m_pieces[index].get();
    }

    void set_piece(int index, std::unique_ptr<Piece> piece) {
        if (index >= 0 && index < 64) m_pieces[index] = std::move(piece);
    }

    void remove_piece(int index) {
        if (index >= 0 && index < 64) m_pieces[index].reset();
    }

    bool is_empty(int index) const {
        if (index < 0 || index >= 64) return true;
        return m_pieces[index] == nullptr;
    }

    // Fait juste le déplacement en mémoire, sans vérifier les règles
    void move_piece_basic(int fromIdx, int toIdx, Type promotion = Type::None);
};