#pragma once

enum class Color { None, White, Black };
enum class Type { None, Pawn, Rook, Knight, Bishop, Queen, King };

struct Position {
    int x, y;
    //convert to index
    int to_index() const { return y * 8 + x; }
    //check if the position is valid
    bool is_valid() const { return x >= 0 && x < 8 && y >= 0 && y < 8; }
};