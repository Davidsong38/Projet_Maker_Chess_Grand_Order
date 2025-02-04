//
// Created by david on 24/01/2025.
//

#include "Chessboard.h"
#include "Characters_List.h"
#include "Pawn.h"
#include "game_cfg.h"

Chessboard* Chessboard::instance = nullptr;

Chessboard* Chessboard::getInstance() {
    if (!instance) {
        return new Chessboard(BOARD_SIZE);
    }
    return instance;
}

void Chessboard::placePiece(int to_coordX,int to_coordY, Pieces* piece) {
    if (to_coordX >= 0 && to_coordX < grid.size() && to_coordY >= 0 && to_coordY < grid.size()) {
        grid[to_coordX][to_coordY] = piece;
    }
}

vector<vector<Pieces *>> Chessboard::getGrid() const {
    return grid;
}

bool Chessboard::isAlly(Pieces *piece, Pieces *target_piece) {
    if (piece->getIsWhite() == target_piece->getIsWhite()) {
        return true;
    }
    return false;
}


bool Chessboard::isInGrid(int to_coordX, int to_coordY) const {
    if (to_coordX >= 0 && to_coordX < grid.size() && to_coordY >= 0 && to_coordY < grid.size()) {
        return true;
    }
    return false;
}

bool Chessboard::isPathClear(int startX, int startY, int endX, int endY, Pieces* piece) const {
    int dx = (endX - startX) == 0 ? 0 : (endX - startX) / abs(endX - startX);  // Direction en X
    int dy = (endY - startY) == 0 ? 0 : (endY - startY) / abs(endY - startY);  // Direction en Y

    int x = startX + dx;
    int y = startY + dy;

    // Parcours des cases intermédiaires
    while (x != endX || y != endY) {
        Pieces* currentPiece = grid[x][y];

        // Si la case contient une pièce
        if (currentPiece != nullptr && !piece->isCheating()) {
            // Si c'est une pièce alliée, on bloque le mouvement
            if (isAlly(piece, currentPiece)) {
                return false;
            }
            // Si c'est une pièce ennemie, on ne bloque que si la destination n'est pas cette pièce
            if (x != endX || y != endY) {
                return false;  // Ne peut pas passer au travers d'une pièce ennemie
            }
        }

        // Avance vers la prochaine case
        x += dx;
        y += dy;
    }

    return true;  // Le chemin est libre
}


vector<pair<int, int>> Chessboard::getValidMoves(Pieces* piece) const {
    vector<pair<int, int>> valid_moves;
    vector<pair<int, int>> piece_moves = piece->getMoves();
    for (const auto& move : piece_moves) {
        int to_coordX = move.first;
        int to_coordY = move.second;
        if (isInGrid(to_coordX, to_coordY) && (grid[to_coordX][to_coordY] == nullptr || !isAlly(piece,grid[to_coordX][to_coordY]))
            && isPathClear(piece->getCoordX(),piece->getCoordY(),to_coordX,to_coordY,piece)) {
            valid_moves.emplace_back(move);

        }
    }
    if (piece->isPawn()) {
        int pawnDirection = piece->getIsWhite() ? -1 : 1;
        int currentX = piece->getCoordX();
        int currentY = piece->getCoordY();

        vector<pair<int, int>> diagonalattack = {{pawnDirection, -1}, {pawnDirection, 1}};

        for (const auto& offset : diagonalattack) {
            int diagX = currentX + offset.first;
            int diagY = currentY + offset.second;

            if (isInGrid(diagX, diagY) && grid[diagX][diagY] != nullptr && !isAlly(piece, grid[diagX][diagY])) {
                valid_moves.emplace_back(diagX, diagY);
            }
        }
    }
    return valid_moves;
}



bool Chessboard::isMovePossible(Pieces* piece,int to_coordX, int to_coordY) const {
    for (const auto& moves : getValidMoves(piece)) {
        if (moves.first == to_coordX && moves.second == to_coordY) {
            return true;
        }
    }
    return false;
}

void Chessboard::movePiece(Pieces* piece, int to_coordX, int to_coordY) {
    int coordX = piece->getCoordX();
    int coordY = piece->getCoordY();
    if (isMovePossible(piece,to_coordX,to_coordY) && grid[coordX][coordY] != nullptr && isMoveable(piece)) {
        Pieces* target_piece = grid[to_coordX][to_coordY];
        if (target_piece == nullptr) {
            grid[to_coordX][to_coordY] = piece ;      // Place la pièce dans la nouvelle case
            grid[coordX][coordY] = nullptr; // Libère l'ancienne case
            piece->setPosition(to_coordX, to_coordY);
        } else {
            KillCheck(piece,target_piece);
        }
        std::cout << "Piece moved from (" << coordX << ", " << coordY
                  << ") to (" << to_coordX << ", " << to_coordY << ")." << std::endl;
    } else {
           std::cout << "Move error!" << std::endl;
       }
}



bool Chessboard::isKillable(Pieces *piece) {
    for (const auto& e : piece->getActive_effects()) {
        if (e.effect == SHIELD || e.effect == IMMORTALITY ) {
            piece->activateEffect(e.effect);
            return false;
        }

    }
    return true;
}

bool Chessboard::KillCheck(Pieces *piece, Pieces *target_piece) {
    int coordX1 = piece->getCoordX();
    int coordY1 = piece->getCoordY();
    int coordX2 = target_piece->getCoordX();
    int coordY2 = target_piece->getCoordY();
    if (isKillable(target_piece)&& !isAlly(piece,target_piece)) {
        grid[coordX2][coordY2] = piece;
        grid[coordX1][coordY1] = nullptr;
        piece->setPosition(coordX2,coordY2);
        std::cout << piece->getName() << " killed " << target_piece->getName() << std::endl;
        return true;
    }
    return false;
}

bool Chessboard::isKilled(Pieces *piece) const {
    for (const auto &row : grid) {
        for (const auto &cell : row) {
            if (cell == piece) {
                return false;
            }
        }
    }
    return true;
}

bool Chessboard::isMoveable( Pieces* piece) {
    for (const auto& e : piece->getActive_effects()) {
        if (e.effect == STUN) {
            return false;
        }
    }
    return true;
}


void Chessboard::displayBoard() const {
    for (const auto &row : grid) {
        for (const auto &cell : row) {
            if (cell != nullptr) {
                cout << cell->getName() << " ";
            } else {
                cout << ". ";
            }

        }
        cout << endl;
    }
}



