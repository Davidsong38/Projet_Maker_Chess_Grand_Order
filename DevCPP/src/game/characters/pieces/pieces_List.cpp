//
// Created by david on 25/01/2025.
//

#include "Pieces_List.h"

#include "Pieces.h"
string Pieces_List_to_string[PIECE_COUNT];
void loadPiecesList() {
    Pieces_List_to_string[PAWN] = "PAWN";
    Pieces_List_to_string[ROOK] = "ROOK";
    Pieces_List_to_string[KNIGHT] = "KNIGHT";
    Pieces_List_to_string[BISHOP] = "BISHOP";
    Pieces_List_to_string[QUEEN] = "QUEEN";
    Pieces_List_to_string[KING] = "KING";

}


//bool segregation_W(Pieces_List piece) {
//    if (piece < PIECE_COUNT) {
//        return true;
//    }
//    return false;
//}
//
//bool segregation_B(Pieces_List piece) {
//    if (piece < PIECE_COUNT && piece > EMPTY ) {
//        return true;
//    }
//    return false;
//}


