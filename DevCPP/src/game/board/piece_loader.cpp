//
// Created by sebas on 01/02/2025.
//

#include "piece_loader.h"

#include <Arceuid.h>
#include <Artoria.h>
#include <Chessboard.h>
#include <EffectHandler.h>
#include <exponential.hpp>

#include "game_cfg.h"
#include "log.h"

#include <iostream>
#include <Medusa_Saber.h>
#include <Nemo_Marine.h>
#include <Sesshoin_Kiara.h>
#include <Ushiwakamaru.h>

bool didPiecesGetInitiated = false;

Characters_List choose_pawn() {
    int id_character = NEMO_MARINE;
    if constexpr (NOT_QUICK_TEST)
        cin >> id_character;
    return static_cast<Characters_List>(id_character);
}

Characters_List choose_knight() {
    int id_character = USHIWAKAMARU;
    if constexpr (NOT_QUICK_TEST)
        cin >> id_character;
    return static_cast<Characters_List>(id_character);
}

Characters_List choose_bishop() {
    int id_character = MEDUSA_SABER;
    if constexpr (NOT_QUICK_TEST)
        cin >> id_character;
    return static_cast<Characters_List>(id_character);
}

Characters_List choose_rook() {
    int id_character = ARCEUID;
    if constexpr (NOT_QUICK_TEST)
        cin >> id_character;
    return static_cast<Characters_List>(id_character);
}

Characters_List choose_king() {
    int id_character = ARTORIA;
    if constexpr (NOT_QUICK_TEST)
        cin >> id_character;
    return static_cast<Characters_List>(id_character);
}

Characters_List choose_queen() {
    int id_character = SESSHOIN_KIARA;
    if constexpr (NOT_QUICK_TEST)
        cin >> id_character;
    return static_cast<Characters_List>(id_character);
}

void add_piece_to_board(int x, int y, bool isWhite, Characters_List character) {
    auto* piece = get_piece(x,y,isWhite,character);
    EffectHandler::configureEffectHandler(*Chessboard::getInstance(),piece);
    Chessboard::getInstance()->placePiece(x,y,piece);
}

Pieces* get_piece(int x, int y, bool isWhite, Characters_List character) {
    Pieces* piece;
    switch (character) {
        case MEDUSA_SABER:
        piece = new Medusa_Saber(x,y,isWhite,character,BISHOP);
        break;
        //case MERLIN:
        //piece = new Merlin(x,y,isWhite,character,PAWN);
        //break;
        //case ASTOLFO:
        //piece = new Astolfo(x,y,isWhite,character,PAWN);
        //break;
        //case TAMAMO_NO_MAE:
        //piece = new Tanamo_No_Mae(x,y,isWhite,character,PAWN);
        //break;
        case USHIWAKAMARU:
        piece = new Ushiwakamaru(x,y,isWhite,character,KNIGHT);
        break;
        //case OKITA:
        //piece = new Okita(x,y,isWhite,character,PAWN);
        //break;
        //case KINTOKI_RIDER:
        //piece = new Kintoki_Rider(x,y,isWhite,character,PAWN);
        //break;
        //case ZHOU_YU:
        //piece = new Zhou_Yu(x,y,isWhite,character,PAWN);
        //break;
        case ARCEUID:
        piece = new Arceuid(x,y,isWhite,character,ROOK);
        break;
        //case XU_FU:
        //piece = new Xu_Fu(x,y,isWhite,character,PAWN);
        //break;
        //case MELUSINE:
        //piece = new Melusine(x,y,isWhite,character,PAWN);
        //break;
        //case MASH:
        //piece = new Mash(x,y,isWhite,character,PAWN);
        //break;
        case SESSHOIN_KIARA:
        piece = new Sesshoin_Kiara(x,y,isWhite,character,QUEEN);
        break;
        //case NITOCRIS_ALTER:
        //piece = new Nitocris_Alter(x,y,isWhite,character,PAWN);
        //break;
        //case BB_DUBAI:
        //piece = new Bb_Dubai(x,y,isWhite,character,PAWN);
        //break;
        //case KUKULKAN:
        //piece = new Kukulkan(x,y,isWhite,character,PAWN);
        break;
        case ARTORIA:
        piece = new Artoria(x,y,isWhite,character,KING);
        break;
        //case GILGAMESH:
        //piece = new Gilgamesh(x,y,isWhite,character,PAWN);
        //break;
        //case OBERON:
        //piece = new Oberon(x,y,isWhite,character,PAWN);
        //break;
        //case ZHUGE_LIANG:
        //piece = new Zhuge_Liang(x,y,isWhite,character,PAWN);
        //break;
        case NEMO_MARINE:
        piece = new Nemo_Marine(x,y,isWhite,character,PAWN);
        break;
        //case NOBU:
        //piece = new Nobu(x,y,isWhite,character,PAWN);
        //break;
        default:
        piece = new Nemo_Marine(x,y,isWhite,character,PAWN);
        log(LOG_ERROR, "Piece type not found in piece_loader.hpp");
        break;
    }
    return piece;
}

void init_pieces() {
    init_pawns();
    init_knights();
    init_bishops();
    init_rooks();
    init_kings();
    init_queens();

    didPiecesGetInitiated = true;
}

void init_pawns() {
    for (int y = 0; y < BOARD_SIZE; y++) {
        Characters_List character = choose_pawn();
        add_piece_to_board(1,y,false,character);
    }

    for (int y = 0; y < BOARD_SIZE; y++) {
        Characters_List character = choose_pawn();
        add_piece_to_board(BOARD_SIZE-2,y,true,character);
    }
}

void init_knights() {
    Characters_List character = choose_knight();
    add_piece_to_board(0,1,false,character);
    character = choose_knight();
    add_piece_to_board(0,6,false,character);
    character = choose_knight();
    add_piece_to_board(BOARD_SIZE-1,1,false,character);
    character = choose_knight();
    add_piece_to_board(BOARD_SIZE-1,6,false,character);
}

void init_bishops() {
    Characters_List character = choose_bishop();
    add_piece_to_board(0,2,false,character);
    character = choose_bishop();
    add_piece_to_board(0,5,false,character);
    character = choose_bishop();
    add_piece_to_board(BOARD_SIZE-1,2,false,character);
    character = choose_bishop();
    add_piece_to_board(BOARD_SIZE-1,5,false,character);

}

void init_rooks() {
    Characters_List character = choose_rook();
    add_piece_to_board(0,0,false,character);
    character = choose_rook();
    add_piece_to_board(0,7,false,character);
    character = choose_rook();
    add_piece_to_board(BOARD_SIZE-1,0,false,character);
    character = choose_rook();
    add_piece_to_board(BOARD_SIZE-1,7,false,character);
}

void init_kings() {
    Characters_List character = choose_king();
    add_piece_to_board(0,3,false,character);
    character = choose_king();
    add_piece_to_board(BOARD_SIZE-1,3,false,character);
}

void init_queens() {
    Characters_List character = choose_queen();
    add_piece_to_board(0,4,false,character);
    character = choose_queen();
    add_piece_to_board(BOARD_SIZE-1,4,false,character);
}
