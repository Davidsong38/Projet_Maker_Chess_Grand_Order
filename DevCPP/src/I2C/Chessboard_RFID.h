//
// Created by david on 31/03/2025.
//

#ifndef CHESSBOARD_RFID_H
#define CHESSBOARD_RFID_H

#include "MCP23017.h"
#include "CLRC663.h"
#include <string>
#include <array>

class Chessboard_RFID {
public:
    Chessboard_RFID(MCP23017& mcp, CLRC663& rfid);
    void begin();
    void update();
    void print();

private:
    MCP23017& mcp_;
    CLRC663& rfid_;
    std::array<std::string, 16> chessboard_rfid_;

    std::string tagToPiece(const std::string& uid);
};

#endif //CHESSBOARD_RFID_H
