#pragma once
#include <vector>
#include "Utils.hpp"

class Chessboard; 

class Piece {
protected:
    Color m_color;
    Type m_type;
    bool m_has_moved = false;

public:
    Piece(Color color, Type type) : m_color(color), m_type(type) {}
    virtual ~Piece() = default; //destrucor

    Color get_color() const { return m_color; }
    Type get_type() const { return m_type; }
    bool has_moved() const { return m_has_moved; }
    void set_moved(bool moved) { m_has_moved = moved; }

    virtual std::vector<int> get_available_moves(const Chessboard& board, int currentIdx) const = 0;
};