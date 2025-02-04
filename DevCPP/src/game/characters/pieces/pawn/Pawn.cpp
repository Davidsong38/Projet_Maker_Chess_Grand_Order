//
// Created by david on 24/01/2025.
//

#include "Pawn.h"

vector<pair<int, int> > Pawn::getMoves() const {
    vector<std::pair<int, int>> moves;

    if (getIsWhite()) {
        if (coordY - 1 < 8) moves.emplace_back(coordX, coordY - 1);
        if (isFirstMove && coordY - 2 < 8) moves.emplace_back(coordX, coordY -2);
    } else {
        if (coordY + 1 < 8) moves.emplace_back(coordX, coordY + 1);
        if (isFirstMove && coordY + 2 < 8) moves.emplace_back(coordX, coordY + 2);
    }
    return moves;
}