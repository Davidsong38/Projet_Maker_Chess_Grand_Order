//
// Created by david on 24/01/2025.
//

#include "Queen.h"

vector<pair<int, int> > Queen::getMoves(){
    vector<std::pair<int, int>> moves;
    for (int i = 1; i < 8; ++i) {
        if (coordX + i < 8 && coordY + i < 8) moves.emplace_back(coordX + i, coordY + i);
        if (coordX - i >= 0 && coordY + i < 8) moves.emplace_back(coordX - i, coordY + i);
        if (coordX + i < 8 && coordY- i >= 0) moves.emplace_back(coordX + i, coordY - i);
        if (coordX - i >= 0 && coordY - i >= 0) moves.emplace_back(coordX - i, coordY - i);
        if (coordX + i < 8) moves.emplace_back(coordX + i, coordY);
        if (coordX - i >= 0) moves.emplace_back(coordX - i, coordY);
        if (coordY- i >= 0) moves.emplace_back(coordX, coordY - i);
        if (coordY + i < 8) moves.emplace_back(coordX, coordY + i);
    }

    return moves;
}
