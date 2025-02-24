//
// Created by david on 24/01/2025.
//

#include "King.h"

vector<glm::ivec2> King::getMoves() {
    vector<glm::ivec2> moves;
        if (coordX + 1 < 8 && coordY + 1 < 8) moves.emplace_back(coordX + 1, coordY + 1);
        if (coordX - 1 >= 0 && coordY + 1 < 8) moves.emplace_back(coordX - 1, coordY + 1);
        if (coordX + 1 < 8 && coordY- 1 >= 0) moves.emplace_back(coordX + 1, coordY - 1);
        if (coordX - 1 >= 0 && coordY - 1 >= 0) moves.emplace_back(coordX - 1, coordY - 1);
        if (coordX + 1 < 8) moves.emplace_back(coordX + 1, coordY);
        if (coordX - 1 >= 0) moves.emplace_back(coordX - 1, coordY);
        if (coordY- 1 >= 0) moves.emplace_back(coordX, coordY - 1);
        if (coordY + 1 < 8) moves.emplace_back(coordX, coordY + 1);


    return moves;
}