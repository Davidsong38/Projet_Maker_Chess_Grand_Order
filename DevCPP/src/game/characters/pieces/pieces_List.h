//
// Created by david on 25/01/2025.
//

#ifndef PIECES_LIST_H
#define PIECES_LIST_H
#include <string>

using namespace std;


enum Pieces_List {
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
    PIECE_COUNT,

};
extern string Pieces_List_to_string[PIECE_COUNT];
void loadPiecesList();


bool segregation_W(Pieces_List piece);
bool segregation_B(Pieces_List piece);





#endif //PIECES_LIST_H
