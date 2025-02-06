//
// Created by david on 24/01/2025.
//

#include "Pawn.h"

vector<pair<int, int> > Pawn::getMoves() const {
    vector<std::pair<int, int>> moves;

    //if (getIsWhite()) {
    //    if (coordX - 1 < 8) moves.emplace_back(coordX - 1, coordY );
    //    if (isFirstMove && coordX - 2 < 8) moves.emplace_back(coordX - 2, coordY);
    //} else {
    //    if (coordX + 1 < 8) moves.emplace_back(coordX + 1, coordY);
    //    if (isFirstMove && coordX + 2 < 8) moves.emplace_back(coordX + 2, coordY);
    //}
    return moves;

}
