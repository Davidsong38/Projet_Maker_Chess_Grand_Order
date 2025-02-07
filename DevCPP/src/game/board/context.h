//
// Created by david on 27/01/2025.
//

#ifndef CONTEXT_H
#define CONTEXT_H
#include "Chessboard.h"
#include "EffectHandler.h"
#include "Pieces.h"

struct context_type {
    Pieces* piece;
    Pieces* target_piece;
    Chessboard* chessboard;
    Pieces* black_king;
    Pieces* white_king;
    int toX;
    int toY;



};


#endif //CONTEXT_H

