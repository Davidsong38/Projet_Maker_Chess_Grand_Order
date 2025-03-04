//
// Created by david on 25/02/2025.
//

#ifndef PIECE_MOVES_H
#define PIECE_MOVES_H

#include <functional>
#include <string>
#include <vector>
#include "glm.hpp"

struct piece_move {
    std::function<std::vector<glm::ivec2>(const glm::ivec2)> get_positions;
    std::string name{"Unnamed piece_move"};
    bool ignoresObstacles{false};
    bool canKill{true};
    bool canKillKing{true};
    bool pathAlwaysClear{false};
};

extern piece_move* knight_default_moves;
extern piece_move* rook_default_moves;
extern piece_move* bishop_default_moves;
extern piece_move* queen_default_moves;
extern piece_move* king_default_moves;
extern piece_move* pawn_default_moves;

extern piece_move* alternate_pawn_moves;
extern piece_move* super_pawn_moves;
extern piece_move* ushiwakamaru_moves;
extern piece_move* arceuid_buff_move;
extern piece_move* kintoki_rider_buff_moves;
extern piece_move* small_cross_moves;
extern piece_move* small_cross_3_range_moves;

extern piece_move* inverted_shinji_moves;
extern piece_move* shinji_moves;

void init_moves();

#endif //PIECE_MOVES_H
