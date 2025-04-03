//
// Created by david on 31/03/2025.
//

#include "Chessboard_RFID.h"
#include <iostream>
#include <thread>
#include <chrono>

Chessboard_RFID::Chessboard_RFID(MCP23017& mcp, CLRC663& rfid) : mcp_(mcp), rfid_(rfid) {}

void Chessboard_RFID::begin() {
    if (!mcp_.begin()) {
        std::cerr << "Chessboard: Failed to initialize MCP23017" << std::endl;
        throw std::runtime_error("MCP23017 initialization failed");
    }
    if (!rfid_.begin()) {
        std::cerr << "Chessboard: Failed to initialize CLRC663" << std::endl;
        throw std::runtime_error("CLRC663 initialization failed");
    }
}

void Chessboard_RFID::update() {
    for (int antenna = 0; antenna < 16; antenna++) {
        if (!mcp_.selectAntenna(antenna)) {
            std::cerr << "Chessboard: Failed to select antenna A" << antenna << std::endl;
            chessboard_rfid_[antenna] = "Error";
            continue;
        }
        chessboard_rfid_[antenna] = rfid_.readTag();
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Attendre pour éviter les interférences
    }
}

void Chessboard_RFID::print() {
    for (int i = 0; i < 16; i++) {
        std::string piece = tagToPiece(chessboard_rfid_[i]);
        std::cout << "A" << i << ": " << piece << std::endl;
    }
}

std::string Chessboard_RFID::tagToPiece(const std::string& uid) {
    std::string piece;
    switch (uid) {
        case 0000 :
            piece = "White_Pawn_0";
        break;
        case 0001 :
            piece = "White_Pawn_1";
        break;
        case 0002 :
            piece = "White_Pawn_2";
        break;
        case 0003 :
            piece = "White_Pawn_3";
        break;
        case 0004 :
            piece = "White_Pawn_4";
        break;
        case 0005 :
            piece = "White_Pawn_5";
        break;
        case 0006 :
            piece = "White_Pawn_6";
        break;
        case 0007 :
            piece = "White_Pawn_7";
        break;
        case 1000 :
            piece = "Black_Pawn_0";
        break;
        case 1001 :
            piece = "Black_Pawn_1";
        break;
        case 1002 :
            piece = "Black_Pawn_2";
        break;
        case 1003 :
            piece = "Black_Pawn_3";
        break;
        case 1004 :
            piece = "Black_Pawn_4";
        break;
        case 1005 :
            piece = "Black_Pawn_5";
        break;
        case 1006 :
            piece = "Black_Pawn_6";
        break;
        case 1007 :
            piece = "Black_Pawn_7";
        break;
        case 0010 :
            piece = "White_Rook_0";
        break;
        case 0011 :
            piece = "White_Rook_1";
        break;
        case 1010 :
            piece = "Black_Rook_0";
        break;
        case 1011 :
            piece = "Black_Rook_1";
        break;
        case 0020 :
            piece = "White_Knight_0";
        break;
        case 0021 :
            piece = "White_Knight_1";
        break;
        case 1020 :
            piece = "Black_Knight_0";
        break;
        case 1021 :
            piece = "Black_Knight_1";
        break;
        case 0030 :
            piece = "White_Bishop_0";
        break;
        case 0031 :
            piece = "White_Bishop_1";
        break;
        case 1030 :
            piece = "Black_Bishop_0";
        break;
        case 1031 :
            piece = "Black_Bishop_1";
        break;
        case 0040 :
            piece = "White_Queen";
        break;
        case 1040 :
            piece = "Black_Queen";
        break;
        case 0050 :
            piece = "White_King";
        break;
        case 1050 :
            piece = "Black_King";
        break;
        default:
            return "Empty";
        break;

    }
    return piece;
}
