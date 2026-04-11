#pragma once
#pragma once

enum class Color { None,
                   White,
                   Black };
enum class GameMode { Classic,
                      Chaos };
enum class Type { None,
                  Pawn,
                  Rook,
                  Knight,
                  Bishop,
                  Queen,
                  King,
                  BerolinaPawn };
enum class GameState {
    Playing,
    WhiteWins,
    BlackWins
};

struct Position {
    int x, y;

    int  to_index() const { return y * 8 + x; }
    bool is_valid() const { return x >= 0 && x < 8 && y >= 0 && y < 8; }

    bool operator==(const Position& other) const
    {
        return x == other.x && y == other.y;
    }
};

struct Move {
    Position start;
    Position end;

    // bool is_capture = false;
    // bool is_castling = false;

    Type promotion = Type::None;
};