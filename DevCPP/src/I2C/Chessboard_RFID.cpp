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
    char piece;
    switch (uid) {
        case 1001 :
            piece = *"White_Pawn";
        break;

        

        default:
            return "Empty";
    }
}
