//
// Created by david on 24/01/2025.
//

#include "Bishop.h"

vector<pair<int, int> > Bishop::getMoves() {
    vector<std::pair<int, int>> moves;
    for (int i = 1; i < 8; ++i) {
        if (coordX + i < 8 && coordY + i < 8) moves.emplace_back(coordX + i, coordY + i);
        if (coordX - i >= 0 && coordY + i < 8) moves.emplace_back(coordX - i, coordY + i);
        if (coordX + i < 8 && coordY- i >= 0) moves.emplace_back(coordX + i, coordY - i);
        if (coordX - i >= 0 && coordY - i >= 0) moves.emplace_back(coordX - i, coordY - i);
    }

    return moves;
}


