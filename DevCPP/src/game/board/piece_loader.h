//
// Created by sebas on 01/02/2025.
//

#ifndef PIECE_LOADER_H
#define PIECE_LOADER_H

#include <Characters_List.h>
#include <Pieces.h>

extern bool didPiecesGetInitiated;

Characters_List choose_pawn();
Characters_List choose_knight();
Characters_List choose_bishop();
Characters_List choose_rook();
Characters_List choose_king();
Characters_List choose_queen();

void add_piece_to_board(int x, int y, bool isWhite, Characters_List character);
Pieces* get_piece(int x, int y, bool isWhite, Characters_List character);

void init_pieces();
void init_pawns();
void init_knights();
void init_bishops();
void init_rooks();
void init_kings();
void init_queens();

#endif //PIECE_LOADER_H
