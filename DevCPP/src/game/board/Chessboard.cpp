//
// Created by david on 24/01/2025.
//

#include "Chessboard.h"

#include <King.h>
#include <log.h>

#include "Characters_List.h"
#include "Pawn.h"
#include "game_cfg.h"
#include "RenderEngine.h"

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

vector<vector<Pieces *>>* Chessboard::getGrid_ptr(){
    return &grid;
}

vector<Pieces*> Chessboard::getDeadList() const{
    return deadList;
}

void Chessboard::addToDeadList(Pieces* piece){
    deadList.emplace_back(piece);
}

void Chessboard::deletePiece (Pieces* piece) {
    grid[piece->getCoordX()][piece->getCoordY()] = nullptr;
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

bool Chessboard::isPathClear(int endX, int endY, Pieces* piece) const {
    return isPathClear(piece->getCoordX(), piece->getCoordY(), endX, endY, piece);
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
        if (currentPiece != nullptr) {
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

bool Chessboard::isPathAllClear(int endX, int endY, Pieces* piece) const {
    return isPathAllClear(piece->getCoordX(), piece->getCoordY(), endX, endY);
}


bool Chessboard::isPathAllClear(int startX, int startY, int endX, int endY) const {
    int dx = (endX - startX) == 0 ? 0 : (endX - startX) / abs(endX - startX);  // Direction en X
    int dy = (endY - startY) == 0 ? 0 : (endY - startY) / abs(endY - startY);  // Direction en Y

    int x = startX + dx;
    int y = startY + dy;

    while (x != endX || y != endY) {
        Pieces* currentPiece = grid[x][y];
        if (currentPiece != nullptr) {
                return false;
        }
        x += dx;
        y += dy;
    }

    return true;
}

vector<Pieces*> Chessboard::getAllPieces() {
    vector<Pieces*> piecesList;
    for (const auto &row : grid) {
        for (const auto &cell : row) {
            if (cell != nullptr) {
                piecesList.emplace_back(cell);
            }
        }
    }
    return piecesList;
}



bool Chessboard::isEndangeredByWhite(pair<int, int> cell) {
    vector<Pieces*> piecesList = getAllPieces();
    for (const auto character : piecesList) {
        if (character->getIsWhite()) {
            for (const auto move : getValidMoves(character)) {
                if (move == cell) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Chessboard::isEndangeredByBlack(pair<int, int> cell) {
    vector<Pieces*> piecesList = getAllPieces();
    for (const auto character : piecesList) {
        if (!character->getIsWhite()) {
            for (const auto move : getValidMoves(character)) {
                if (move == cell) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Chessboard::canLittleRoque(Pieces* piece) {
    int coordX = piece->getCoordX();
    int coordY = piece->getCoordY();
    vector<pair<int, int>> coords;
    if (grid[7][7] != nullptr && piece->getIsWhite() && grid[7][7]->getIsWhite()) {
        if (piece->isKing() && piece->getIsFirstMove() && grid[7][7]->isRook() &&grid[7][7]->getIsFirstMove() && isPathAllClear(coordX, coordY + 2, piece)) {
            coords.emplace_back(coordX, coordY);
            coords.emplace_back(coordX, coordY + 1);
            coords.emplace_back(coordX, coordY + 2);
            for (const auto move : coords) {
                if (isEndangeredByBlack(move)) {
                    return false;
                }
            }
            return true;
        }
    }
    if (grid[0][7] != nullptr && !piece->getIsWhite() && !grid[0][7]->getIsWhite()) {
        if (piece->isKing() && piece->getIsFirstMove() && grid[0][7]->isRook() && grid[0][7]->getIsFirstMove() && isPathAllClear(coordX, coordY + 2, piece)) {
            coords.emplace_back(coordX, coordY);
            coords.emplace_back(coordX, coordY + 1);
            coords.emplace_back(coordX, coordY + 2);
            for (const auto move : coords) {
                if (isEndangeredByWhite(move)) {
                    return false;
                }

            }
            return true;
        }
    }
    return false;
}


bool Chessboard::canBigRoque(Pieces* piece) {
    int coordX = piece->getCoordX();
    int coordY = piece->getCoordY();
    vector<pair<int, int>>coords;
    if (grid[7][0] != nullptr && piece->getIsWhite() && grid[7][0]->getIsWhite()) {
        if (piece->isKing() && piece->getIsFirstMove() && grid[7][0]->isRook() && grid[7][0]->getIsFirstMove() && isPathAllClear(coordX,coordY - 3, piece)) {
            coords.emplace_back(coordX, coordY);
            coords.emplace_back(coordX, coordY - 1);
            coords.emplace_back(coordX, coordY - 2);
            for (const auto move : coords) {
                if (isEndangeredByBlack(move)) {
                    return false;
                }

            }
            return true;
        }
    }
    if (grid[0][0] != nullptr && !piece->getIsWhite() && !grid[0][0]->getIsWhite()) {
        if (piece->isKing() && piece->getIsFirstMove() && grid[0][0]->isRook() && grid[0][0]->getIsFirstMove() && isPathAllClear(coordX,coordY - 3, piece)) {
            coords.emplace_back(coordX, coordY);
            coords.emplace_back(coordX, coordY - 1);
            coords.emplace_back(coordX, coordY - 2);
            for (const auto move : coords) {
                if (isEndangeredByWhite(move)) {
                    return false;
                }

            }
            return true;
        }
    }
    return false;
}

bool Chessboard::bigRoque(Pieces* piece, int to_coordX, int to_coordY) {
    if (canBigRoque(piece) && !pawnMenacingBigRoque(piece)) {
        if (piece->getIsWhite() == true && to_coordX == 7 && to_coordY == 2) {
            grid[7][2] = piece ;
            grid[piece->getCoordX()][piece->getCoordY()] = nullptr;
            piece->setPosition(7, 2);
            (piece->CNTMove)++;
            piece->setHasRoqued(true);
            Pieces* ally_piece = grid[7][0];
            grid[7][3] = ally_piece;
            grid[ally_piece->getCoordX()][ally_piece->getCoordY()] = nullptr;
            ally_piece->setPosition(7, 3);
            (ally_piece->CNTMove)++;
            ally_piece->setHasRoqued(true);
            //movePiece(piece,7,2);
            //movePiece(grid[7][0],7,3);
            return true;
        }
        if (piece->getIsWhite() == false && to_coordX == 0 && to_coordY == 2) {
            grid[0][2] = piece ;
            grid[piece->getCoordX()][piece->getCoordY()] = nullptr;
            piece->setPosition(0, 2);
            (piece->CNTMove)++;
            piece->setHasRoqued(true);
            Pieces* ally_piece = grid[0][0];
            grid[0][3] = ally_piece;
            grid[ally_piece->getCoordX()][ally_piece->getCoordY()] = nullptr;
            ally_piece->setPosition(0, 3);
            (ally_piece->CNTMove)++;
            ally_piece->setHasRoqued(true);
            //movePiece(piece,0,2);
            //movePiece(grid[0][0],0,3);
            return true;
        }
    }
    return false;
}

bool Chessboard::littleRoque(Pieces* piece, int to_coordX, int to_coordY) {
    if (canLittleRoque(piece) && !pawnMenacingLittleRoque(piece)) {
        if (piece->getIsWhite() == true && to_coordX == 7 && to_coordY == 6) {
            grid[7][6] = piece ;
            grid[piece->getCoordX()][piece->getCoordY()] = nullptr;
            piece->setPosition(7, 6);
            (piece->CNTMove)++;
            piece->setHasRoqued(true);
            Pieces* ally_piece = grid[7][7];
            grid[7][5] = ally_piece;
            grid[ally_piece->getCoordX()][ally_piece->getCoordY()] = nullptr;
            ally_piece->setPosition(7, 5);
            (ally_piece->CNTMove)++;
            ally_piece->setHasRoqued(true);
            //movePiece(piece,7,6);
            //movePiece(grid[7][7],7,5);
            return true;
        }
        if (piece->getIsWhite() == false && to_coordX == 0 && to_coordY == 6) {
            grid[0][6] = piece ;
            grid[piece->getCoordX()][piece->getCoordY()] = nullptr;
            piece->setPosition(0, 6);
            (piece->CNTMove)++;
            piece->setHasRoqued(true);
            Pieces* ally_piece = grid[0][7];
            grid[0][5] = ally_piece;
            grid[ally_piece->getCoordX()][ally_piece->getCoordY()] = nullptr;
            ally_piece->setPosition(0, 5);
            (ally_piece->CNTMove)++;
            ally_piece->setHasRoqued(true);
            //movePiece(piece,0,6);
            //movePiece(grid[0][7],0,5);
            return true;
        }
    }
    return false;
}

bool Chessboard::pawnMenacingLittleRoque(Pieces* king) {
    vector<Pieces*> piecesList = getAllPieces();
    for (const auto character : piecesList) {
        if (character->isPawn()) {
            int coordX = character->getCoordX();
            int coordY = character->getCoordY();
            if (character->getIsWhite() && !king->getIsWhite() &&
                (coordX == 1 && coordY == 7
                || coordX == 1 && coordY == 6
                || coordX == 1 && coordY == 5
                || coordX == 1 && coordY == 4
                || coordX == 1 && coordY == 3)) {
                return true;

            }
            if (!character ->getIsWhite() && king->getIsWhite() &&
                (coordX == 6 && coordY == 7
                || coordX == 6 && coordY == 6
                || coordX == 6 && coordY == 5
                || coordX == 6 && coordY == 4
                || coordX == 6 && coordY == 3)){
                return true;

            }
        }
    }
    return false;
}

bool Chessboard::pawnMenacingBigRoque(Pieces* king) {
    vector<Pieces*> piecesList = getAllPieces();
    for (const auto character : piecesList) {
        if (character->isPawn()) {
            int coordX = character->getCoordX();
            int coordY = character->getCoordY();
            if (character->getIsWhite() && !king->getIsWhite() &&
                (coordX == 1 && coordY == 1
                || coordX == 1 && coordY == 2
                || coordX == 1 && coordY == 3
                || coordX == 1 && coordY == 4
                || coordX == 1 && coordY == 5)) {
                return true;

                }
            if (!character ->getIsWhite() && king->getIsWhite() &&
                (coordX == 6 && coordY == 1
                || coordX == 6 && coordY == 2
                || coordX == 6 && coordY == 3
                || coordX == 6 && coordY == 4
                || coordX == 6 && coordY == 5)){
                return true;

                }
        }
    }
    return false;
}


bool Chessboard::hasJustFirstMove(Pieces* piece) {
    if (piece->getTurnStamp() == 1)
        return true;

    else {
        //piece->setFirstMoveLastTurn(false);
        return false;
    }
}

bool Chessboard::isPassable(Pieces* piece) {
    if (piece->isPawn()) {
        if (hasJustFirstMove(piece)) {
            return true;
        }
    }
    return false;
}

bool Chessboard::notBrokenMove(Pieces* piece,Pieces* target_piece){
    if (target_piece != nullptr && piece->getIsOnAMove() && target_piece->isKing())
        return false;
    if (target_piece != nullptr && PieceHaveThisEffect(piece,IMMORTALITY) && target_piece->isKing())
        return false;
    return true;
}

vector<pair<int, int>> Chessboard::getValidMoves(Pieces* piece) const {
    vector<pair<int, int>> valid_moves;
    vector<pair<int, int>> piece_moves = piece->getMoves();
    if (piece->getOverrideMoves() != nullptr){
        //std::cout << "alors je sais pas mais la ca me casse les couilles" << std::endl;
        piece_moves = piece->getOverrideMoves()();
    }
    //cout << "moves : " <<piece_moves.size() << endl;
    if (piece->isPawn() && piece->getMoves().empty() && piece->getOverrideMoves() == nullptr) {
        int pawnDirection = piece->getIsWhite() ? -1 : 1;
        int currentX = piece->getCoordX();
        int currentY = piece->getCoordY();
        bool FirstMove = piece->getIsFirstMove();
        if (piece->getIsWhite()) {
            if (currentX - 1 >= 0 && grid[currentX - 1][currentY] == nullptr ) valid_moves.emplace_back(currentX - 1, currentY );
            if (FirstMove && currentX - 2 >= 0 && isPathClear(currentX, currentY,currentX - 2,currentY,piece)
                && grid[currentX - 2][currentY] == nullptr) valid_moves.emplace_back(currentX - 2, currentY);
            if (currentX == 3 && isInGrid(currentX,currentY-1)&& grid[currentX][currentY-1] != nullptr && grid[currentX][currentY-1]->isPawn()
                && !grid[currentX][currentY-1]->getIsWhite() && isPassable(grid[currentX][currentY-1]))
                valid_moves.emplace_back(currentX-1 , currentY-1);
            if (currentX == 3 && isInGrid(currentX,currentY+1)&& grid[currentX][currentY+1] != nullptr && grid[currentX][currentY+1]->isPawn()
                && !grid[currentX][currentY+1]->getIsWhite() && isPassable(grid[currentX][currentY+1]))
                valid_moves.emplace_back(currentX-1 , currentY+1);
        } else {
            if (currentX + 1 < 8 && grid[currentX + 1][currentY] == nullptr) valid_moves.emplace_back(currentX + 1, currentY);
            if (FirstMove && currentX + 2 < 8 && isPathClear(currentX, currentY,currentX + 2,currentY,piece)
                && grid[currentX + 2][currentY] == nullptr) valid_moves.emplace_back(currentX + 2, currentY);
            if (currentX == 4 && isInGrid(currentX,currentY-1)&& grid[currentX][currentY-1] != nullptr && grid[currentX][currentY-1]->isPawn()
                && grid[currentX][currentY-1]->getIsWhite() && isPassable(grid[currentX][currentY-1]))
                valid_moves.emplace_back(currentX+1 , currentY-1);
            if (currentX == 4 && isInGrid(currentX,currentY+1)&& grid[currentX][currentY+1] != nullptr && grid[currentX][currentY+1]->isPawn()
                && grid[currentX][currentY+1]->getIsWhite() && isPassable(grid[currentX][currentY+1]))
                valid_moves.emplace_back(currentX+1 , currentY+1);
        }
        vector<pair<int, int>> diagonalattack = {{pawnDirection, -1}, {pawnDirection, 1}};
        for (const auto& offset : diagonalattack) {
            int diagX = currentX + offset.first;
            int diagY = currentY + offset.second;
            if (isInGrid(diagX, diagY) && grid[diagX][diagY] != nullptr && !isAlly(piece, grid[diagX][diagY])) {
                valid_moves.emplace_back(diagX, diagY);
            }
        }
        return valid_moves;
    }
    for (const auto& move : piece_moves) {
        int to_coordX = move.first;
        int to_coordY = move.second;
        bool inGrid = isInGrid(to_coordX, to_coordY);
        bool emptySquare = grid[to_coordX][to_coordY] == nullptr;
        bool isTargetAlly = false;
        bool notBroken = notBrokenMove(piece,grid[to_coordX][to_coordY]);
        if (!emptySquare)
            isTargetAlly = isAlly(piece,grid[to_coordX][to_coordY]);
        bool knight = piece->isKnight();
        bool pathClear = true;
        if (!knight && piece->getMovesMode() != 3 || knight && piece->getMovesMode() != 0 && piece->getMovesMode() != 3)
            pathClear = isPathClear(to_coordX,to_coordY, piece);
        if (inGrid && (emptySquare || !isTargetAlly) && pathClear && notBroken) {
            valid_moves.emplace_back(move);
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

bool Chessboard::movePiece(Pieces* piece, int to_coordX, int to_coordY) {
    int coordX = piece->getCoordX();
    int coordY = piece->getCoordY();
    piece->addToAllMovesDoneBefore(coordX, coordY);
    if (littleRoque(piece,to_coordX,to_coordY))
        return true;
    if (bigRoque(piece,to_coordX,to_coordY))
        return true;
    if (!isMoveable(piece)) {
       // log(LOG_ERROR, "Chessboard::movePiece: move is not possible for piece");
        return false;
    }
    if (!isMovePossible(piece,to_coordX,to_coordY)) {
        //log(LOG_ERROR, "Chessboard::movePiece: move is not possible on board");
        return false;
    }
    Pieces* target_piece = grid[to_coordX][to_coordY];
    if (KillInPassing(piece,to_coordX,to_coordY))
            return true;
    if (target_piece == nullptr) {
        grid[to_coordX][to_coordY] = piece ;      // Place la pièce dans la nouvelle case
        grid[coordX][coordY] = nullptr; // Libère l'ancienne case
        piece->setPosition(to_coordX, to_coordY);
        (piece->CNTMove)++;
    } else {
        KillCheck(piece,target_piece);
    }
    PawnReachingEndOfBoard(piece);
    std::cout << (piece->getIsWhite()? "White " : "Black ")<<piece->getName()<<" moved from (" << coordX << ", " << coordY
              << ") to (" << to_coordX << ", " << to_coordY << ")." << std::endl;
    piece->activateEffect(MOVE_CHANGING);
    return true;
}


bool Chessboard::isKillable(Pieces *piece,Pieces* target_piece) {
    if (piece->isKing())
        return true;
    for (const auto& e : target_piece->getActive_effects()) {
        if (e.effect == SHIELD || e.effect == IMMORTALITY ) {
            target_piece->activateEffect(e.effect);
            return false;
        }

    }
    return true;
}

bool Chessboard::PieceHaveThisEffect(Pieces *piece, Effect_List chosenEffect) {
    for (const auto& e : piece->getActive_effects()) {
        if (e.effect == chosenEffect) {
            std::cout << "pas la aussi par pitié" << e.effect_amount << std::endl;
            return true;
        }

    }
    return false;
}

bool Chessboard::KillCheck(Pieces *piece, Pieces *target_piece) {
    int coordX1 = piece->getCoordX();
    int coordY1 = piece->getCoordY();
    int coordX2 = target_piece->getCoordX();
    int coordY2 = target_piece->getCoordY();
    if (isKillable(piece,target_piece)&& !isAlly(piece,target_piece)) {
        if (PieceHaveThisEffect(target_piece,CHANGE_CONTROL)){
            target_piece->activateEffect(CHANGE_CONTROL);
            std::cout << "je reviens dans mon camp" << std::endl;
            return true;
        }

        grid[coordX2][coordY2] = piece;
        grid[coordX1][coordY1] = nullptr;
        piece->setPosition(coordX2,coordY2);
        std::cout << (piece->getIsWhite()? "White " : "Black ")<< piece->getName() << " killed "
        << (target_piece->getIsWhite()? "White " : "Black ") <<target_piece->getName() << std::endl;
        piece->setHasJustKilled(true);
        if (target_piece->getIsWhite())
            GameEngine::getInstance()->NB_WhiteDead++;
        if (!target_piece->getIsWhite())
            GameEngine::getInstance()->NB_BlackDead++;
        addToDeadList(target_piece);
        target_piece->setIsAlive(false);
        return true;
    }
    return false;
}

bool Chessboard::KillInPassing(Pieces *piece, int to_coordX, int to_coordY) {
    int coordX1 = piece->getCoordX();
    int coordY1 = piece->getCoordY();
    if (piece->isPawn() && !PieceHaveThisEffect(piece, MOVE_CHANGING)) {
        if (piece->getIsWhite()) {
            int coordX2 = to_coordX + 1;
            int coordY2 = to_coordY;
            Pieces* realTargetPiece = grid[coordX2][coordY2];
            if (realTargetPiece != nullptr && isKillable(piece,realTargetPiece)&& !isAlly(piece,realTargetPiece) && realTargetPiece->isPawn()
                && coordX1 == 3 && coordX2 == 3 && isPassable(realTargetPiece)) {
                grid[coordX2][coordY2] = nullptr;
                grid[to_coordX][to_coordY] = piece;
                grid[coordX1][coordY1] = nullptr;
                piece->setPosition(to_coordX,to_coordY);
                (piece->CNTMove)++;
                std::cout << (piece->getIsWhite()? "White " : "Black ")<< piece->getName() << " killed "
                << (realTargetPiece->getIsWhite()? "White " : "Black ") <<realTargetPiece->getName() << std::endl;
                if (realTargetPiece->getIsWhite())
                    GameEngine::getInstance()->NB_WhiteDead++;
                if (!realTargetPiece->getIsWhite())
                    GameEngine::getInstance()->NB_BlackDead++;
                addToDeadList(realTargetPiece);
                realTargetPiece->setIsAlive(false);
                return true;
            }
        } else {
            int coordX2 = to_coordX - 1;
            int coordY2 = to_coordY;
            Pieces* realTargetPiece = grid[coordX2][coordY2];
            if (realTargetPiece != nullptr && isKillable(piece,realTargetPiece)&& !isAlly(piece,realTargetPiece) && realTargetPiece->isPawn()
                && coordX1 == 4 && coordX2 == 4 && isPassable(realTargetPiece)) {
                grid[coordX2][coordY2] = nullptr;
                grid[to_coordX][to_coordY] = piece;
                grid[coordX1][coordY1] = nullptr;
                piece->setPosition(to_coordX,to_coordY);
                (piece->CNTMove)++;
                std::cout << (piece->getIsWhite()? "White " : "Black ")<< piece->getName() << " killed "
                << (realTargetPiece->getIsWhite()? "White " : "Black ") <<realTargetPiece->getName() << std::endl;
                if (realTargetPiece->getIsWhite())
                    GameEngine::getInstance()->NB_WhiteDead++;
                if (!realTargetPiece->getIsWhite())
                    GameEngine::getInstance()->NB_BlackDead++;
                addToDeadList(realTargetPiece);
                realTargetPiece->setIsAlive(false);
                return true;
            }
        }
    }
    return false;

}

bool Chessboard::PawnReachingEndOfBoard(Pieces *piece) {
    int coordX = piece->getCoordX();
    if (piece->isPawn()) {
        if (piece->getIsWhite()) {
            if (coordX == 0){
                piece->setPiecesOrigin(QUEEN);
                piece->activateEffect(SUPP_RANGE);
                piece->deleteEffect(IMMORTALITY);
                return true;
            }
        } else {
            if (coordX == 7){
                piece->setPiecesOrigin(QUEEN);
                piece->activateEffect(SUPP_RANGE);
                piece->deleteEffect(IMMORTALITY);
                return true;
            }
        }
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

bool Chessboard::getPosInBoard(const glm::vec2 screenPos, glm::ivec2 &boardPos) const {
    auto transformed_screenPos = glm::vec2(
        screenPos.x * RenderEngine::getWindowAspectRatio(),
        screenPos.y
    );
    transformed_screenPos.x *= static_cast<float>(size);
    float offsetX = (1 - RenderEngine::getWindowInverseAspectRatio()) * static_cast<float>(size);
    offsetX += RenderEngine::getWindowAspectRatio() + 1;
    offsetX *= 0.5f;
    transformed_screenPos.x -= offsetX;
    transformed_screenPos.y *= static_cast<float>(size);
    boardPos.y = static_cast<int>(floor(transformed_screenPos.x));
    boardPos.x = static_cast<int>(floor(transformed_screenPos.y));
    if (boardPos.y < 0 || boardPos.y >= size || boardPos.x < 0 || boardPos.x >= size)
        return false;
    return true;
}