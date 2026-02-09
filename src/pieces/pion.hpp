#ifndef PION_HPP
#define PION_HPP

#include <vector>

struct Pos {
    int row, col;
    bool operator==(const Pos& other) const { return row == other.row && col == other.col; }
};

std::vector<int> getPionMoves(int currentIndex, const std::vector<char>& board, bool isWhite);

#endif