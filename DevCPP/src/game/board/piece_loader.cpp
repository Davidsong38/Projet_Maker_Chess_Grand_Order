//
// Created by sebas on 01/02/2025.
//

#include "piece_loader.h"

#include <Arceuid.h>
#include <Artoria.h>
#include <Chessboard.h>
#include <GameEngine.h>

#include "game_cfg.h"
#include "log.h"

#include <iostream>
#include <Kukulkan.h>
#include <Medusa_Saber.h>
#include <Nemo_Marine.h>
#include <Sesshoin_Kiara.h>
#include <Tamamo.h>
#include <Ushiwakamaru.h>

#include "Gilgamesh.h"
#include "Kintoki_Rider.h"
#include "Merlin.h"
#include "Nitocris_Alter.h"
#include "Okita.h"
#include "Xu_Fu.h"

bool didPiecesGetInitiated = false;

Characters_List choose_pawn(bool isWhite) {
    int id_character = NEMO_MARINE;
    if constexpr (NOT_QUICK_TEST) {
        cout << " Choose" << (isWhite ? " White" : " Black") << " pawn character: " << endl;
        cin >> id_character;
    }
    return static_cast<Characters_List>(id_character);
}

Characters_List choose_knight(bool isWhite, bool isRight) {
    int id_character = KINTOKI_RIDER;
    if constexpr (NOT_QUICK_TEST) {
        cout << " Choose" << (isWhite ? " White" : " Black") << " knight"<< (isRight ? "Right" : "Left")<<" character: " << endl;
        cin >> id_character;
    }
    return static_cast<Characters_List>(id_character);
}

Characters_List choose_bishop(bool isWhite, bool isRight) {
    int id_character = TAMAMO_NO_MAE;
    if constexpr (NOT_QUICK_TEST) {
        cout << "Choose" << (isWhite ? " White" : " Black") << " bishop"<< (isRight ? "Right" : "Left")<<" character: " << endl;
        cin >> id_character;
    }
    return static_cast<Characters_List>(id_character);
}

Characters_List choose_rook(bool isWhite, bool isRight) {
    int id_character = XU_FU;
    if constexpr (NOT_QUICK_TEST) {
        cout << "Choose" << (isWhite ? " White" : " Black") << " rook"<< (isRight ? "Right" : "Left")<<" character: " << endl;
        cin >> id_character;
    }
    return static_cast<Characters_List>(id_character);
}

Characters_List choose_king(bool isWhite) {
    int id_character = GILGAMESH;
    if constexpr (NOT_QUICK_TEST) {
        cout << "Choose" << (isWhite ? " White" : " Black") << " king character: " << endl;
        cin >> id_character;
    }
    return static_cast<Characters_List>(id_character);
}

Characters_List choose_queen(bool isWhite) {
    int id_character = KUKULKAN;
    if constexpr (NOT_QUICK_TEST) {
        cout << "Choose" << (isWhite ? " White" : " Black") << " queen character: " << endl;
        cin >> id_character;
    }
    return static_cast<Characters_List>(id_character);
}

void add_piece_to_board(int x, int y, bool isWhite, Characters_List character) {
    auto* piece = get_piece(x,y,isWhite,character);
    if (piece->isKing())
        if (isWhite)
            GameEngine::getInstance()->setWhiteKing(piece);
        else
            GameEngine::getInstance()->setBlackKing(piece);
    Chessboard::getInstance()->placePiece(x,y,piece);
}

Pieces* get_piece(int x, int y, bool isWhite, Characters_List character) {
    Pieces* piece;
    switch (character) {
        case MEDUSA_SABER:
        piece = new Medusa_Saber(x,y,isWhite,character);
        break;
        case MERLIN:
        piece = new Merlin(x,y,isWhite,character);
        break;
        //case ASTOLFO:
        //piece = new Astolfo(x,y,isWhite,character);
        //break;
        case TAMAMO_NO_MAE:
        piece = new Tamamo_No_Mae(x,y,isWhite,character);
        break;
        case USHIWAKAMARU:
        piece = new Ushiwakamaru(x,y,isWhite,character);
        break;
        case OKITA:
        piece = new Okita(x,y,isWhite,character);
        break;
        case KINTOKI_RIDER:
        piece = new Kintoki_Rider(x,y,isWhite,character);
        break;
        //case ZHOU_YU:
        //piece = new Zhou_Yu(x,y,isWhite,character);
        //break;
        case ARCEUID:
        piece = new Arceuid(x,y,isWhite,character);
        break;
        case XU_FU:
        piece = new Xu_Fu(x,y,isWhite,character);
        break;
        //case MELUSINE:
        //piece = new Melusine(x,y,isWhite,character);
        //break;
        //case MASH:
        //piece = new Mash(x,y,isWhite,character);
        //break;
        case SESSHOIN_KIARA:
        piece = new Sesshoin_Kiara(x,y,isWhite,character);
        break;
        case NITOCRIS_ALTER:
        piece = new Nitocris_Alter(x,y,isWhite,character);
        break;
        //case BB_DUBAI:
        //piece = new Bb_Dubai(x,y,isWhite,character);
        //break;
        case KUKULKAN:
        piece = new Kukulkan(x,y,isWhite,character);
        break;
        case ARTORIA:
        piece = new Artoria(x,y,isWhite,character);
        break;
        case GILGAMESH:
        piece = new Gilgamesh(x,y,isWhite,character);
        break;
        //case OBERON:
        //piece = new Oberon(x,y,isWhite,character);
        //break;
        //case ZHUGE_LIANG:
        //piece = new Zhuge_Liang(x,y,isWhite,character);
        //break;
        case NEMO_MARINE:
        piece = new Nemo_Marine(x,y,isWhite,character);
        break;
        //case NOBU:
        //piece = new Nobu(x,y,isWhite,character);
        //break;
        default:
        piece = new Nemo_Marine(x,y,isWhite,character);
        ltr_log_error("Piece type not found in piece_loader.hpp");
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
    Characters_List character = choose_pawn(false);
    for (int y = 0; y < BOARD_SIZE; y++)
        add_piece_to_board(1,y,false,character);
    character = choose_pawn(true);
    for (int y = 0; y < BOARD_SIZE; y++)
        add_piece_to_board(BOARD_SIZE-2,y,true,character);
}

void init_knights() {
    Characters_List character = choose_knight(false,false);
    add_piece_to_board(0,1,false,character);
    character = choose_knight(false,true);
    add_piece_to_board(0,6,false,character);
    character = choose_knight(true,false);
    add_piece_to_board(BOARD_SIZE-1,1,true,character);
    character = choose_knight(true,true);
    add_piece_to_board(BOARD_SIZE-1,6,true,character);
}

void init_bishops() {
    Characters_List character = choose_bishop(false,false);
    add_piece_to_board(0,2,false,character);
    character = choose_bishop(false,true);
    add_piece_to_board(0,5,false,character);
    character = choose_bishop(true,false);
    add_piece_to_board(BOARD_SIZE-1,2,true,character);
    character = choose_bishop(true, true);
    add_piece_to_board(BOARD_SIZE-1,5,true,character);

}

void init_rooks() {
    Characters_List character = choose_rook(false,false);
    add_piece_to_board(0,0,false,character);
    character = choose_rook(false,true);
    add_piece_to_board(0,7,false,character);
    character = choose_rook(true,false);
    add_piece_to_board(BOARD_SIZE-1,0,true,character);
    character = choose_rook(true,true);
    add_piece_to_board(BOARD_SIZE-1,7,true,character);
}

void init_kings() {
    Characters_List character = choose_king(false);
    add_piece_to_board(0,4,false,character);
    character = choose_king(true);
    add_piece_to_board(BOARD_SIZE-1,4,true,character);
}

void init_queens() {
    Characters_List character = choose_queen(false);
    add_piece_to_board(0,3,false,character);
    character = choose_queen(true);
    add_piece_to_board(BOARD_SIZE-1,3,true,character);
}
