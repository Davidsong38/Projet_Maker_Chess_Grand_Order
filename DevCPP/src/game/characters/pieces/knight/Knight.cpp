//
// Created by david on 24/01/2025.
//

#include "Knight.h"

#include <iostream>


vector<glm::ivec2> Knight::getMoves() {
    vector<glm::ivec2> moves;
        if (coordX + 1 < 8 && coordY + 2 < 8) moves.emplace_back(coordX + 1, coordY + 2);
        if (coordX - 1 >= 0 && coordY + 2 < 8) moves.emplace_back(coordX - 1, coordY + 2);
        if (coordX + 1 < 8 && coordY- 2 >= 0) moves.emplace_back(coordX + 1, coordY - 2);
        if (coordX - 1 >= 0 && coordY - 2 >= 0) moves.emplace_back(coordX - 1, coordY - 2);
        if (coordX + 2 < 8 && coordY + 1 < 8) moves.emplace_back(coordX + 2, coordY + 1);
        if (coordX - 2 >= 0 && coordY + 1 < 8) moves.emplace_back(coordX - 2, coordY + 1);
        if (coordX + 2 < 8 && coordY- 1 >= 0) moves.emplace_back(coordX + 2, coordY - 1);
        if (coordX - 2 >= 0 && coordY - 1 >= 0) moves.emplace_back(coordX - 2, coordY - 1);
    return moves;
}
