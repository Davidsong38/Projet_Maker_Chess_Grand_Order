//
// Created by sebas on 24/02/2025.
//

#ifndef SELECTION_H
#define SELECTION_H

#include "vector"
#include "Chessboard.h"


struct selection_type {
    std::vector<chessboard_cell*> white_pieces;
    std::vector<chessboard_cell*> black_pieces;
    std::vector<chessboard_cell*> empty_pieces;
    int whites{0};
    int blacks{0};
    int emptys{0};
};

struct selection_request_type {
    int whites{0};
    int blacks{0};
    int emptys{0};
    bool instantValidation{true};
    std::vector<chessboard_cell*> banned_cells;
    EffectInstance* effect_instance{nullptr};
};

#endif //SELECTION_H
