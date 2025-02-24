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

void Chessboard::placePiece(const int coordX, const int coordY, Pieces* piece) {
    if (coordX >= 0 && coordX < grid.size() && coordY >= 0 && coordY < grid.size()) {
        grid[coordX][coordY].piece = piece;
    } else {
        ltr_log_error("Invalid piece coordinate : ", coordX, " ", coordY);
    }
}

Pieces* Chessboard::getPieceAt(const int coordX, const int coordY) const {
    if (coordX >= 0 && coordX < grid.size() && coordY >= 0 && coordY < grid.size()) {
        return grid[coordX][coordY].piece;
    }
    ltr_log_error("Invalid piece coordinate : ", coordX, " ", coordY);
    return nullptr;
}

vector<Pieces*> Chessboard::getDeadList() const{
    return deadList;
}

void Chessboard::addToDeadList(Pieces* piece){
    deadList.emplace_back(piece);
}

void Chessboard::removeFromDeadList(Pieces* piece){
    deadList.erase(std::remove(deadList.begin(), deadList.end(), piece));
}

void Chessboard::deletePiece (const Pieces* piece) {
    const int to_coordX = piece->getCoordX();
    const int to_coordY = piece->getCoordY();
    if (to_coordX >= 0 && to_coordX < grid.size() && to_coordY >= 0 && to_coordY < grid.size()) {
        grid[piece->getCoordX()][piece->getCoordY()].piece = nullptr;
    } else {
        ltr_log_error("Chessboard::deletePiece::Invalid piece coordinate : ", to_coordX, " ", to_coordY);
    }
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
        Pieces* currentPiece = getPieceAt(x, y);

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
        Pieces* currentPiece = getPieceAt(x, y);
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
            if (cell.piece != nullptr) {
                piecesList.emplace_back(cell.piece);
            }
        }
    }
    return piecesList;
}



bool Chessboard::isEndangeredByWhite(glm::ivec2 cell) {
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

bool Chessboard::isEndangeredByBlack(glm::ivec2 cell) {
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
    vector<glm::ivec2> coords;
    if (grid[7][7].piece != nullptr && piece->getIsWhite() && grid[7][7].piece->getIsWhite()) {
        if (piece->isKing() && piece->getIsFirstMove() && grid[7][7].piece->isRook() &&grid[7][7].piece->getIsFirstMove() && isPathAllClear(coordX, coordY + 2, piece)) {
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
    if (grid[0][7].piece != nullptr && !piece->getIsWhite() && !grid[0][7].piece->getIsWhite()) {
        if (piece->isKing() && piece->getIsFirstMove() && grid[0][7].piece->isRook() && grid[0][7].piece->getIsFirstMove() && isPathAllClear(coordX, coordY + 2, piece)) {
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
    vector<glm::ivec2>coords;
    if (grid[7][0].piece != nullptr && piece->getIsWhite() && grid[7][0].piece->getIsWhite()) {
        if (piece->isKing() && piece->getIsFirstMove() && grid[7][0].piece->isRook() && grid[7][0].piece->getIsFirstMove() && isPathAllClear(coordX,coordY - 3, piece)) {
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
    if (grid[0][0].piece != nullptr && !piece->getIsWhite() && !grid[0][0].piece->getIsWhite()) {
        if (piece->isKing() && piece->getIsFirstMove() && grid[0][0].piece->isRook() && grid[0][0].piece->getIsFirstMove() && isPathAllClear(coordX,coordY - 3, piece)) {
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

bool Chessboard::bigRoque(Pieces* piece, const int to_coordX, const int to_coordY) {
    if (canBigRoque(piece) && !pawnMenacingBigRoque(piece)) {
        if (piece->getIsWhite() == true && to_coordX == 7 && to_coordY == 2) {
            piece->goToPosition(7, 2);
            piece->CNTMove++;
            piece->setHasRoqued(true);
            Pieces* ally_piece = getPieceAt(7, 0);
            ally_piece->goToPosition(7, 3);
            ally_piece->CNTMove++;
            ally_piece->setHasRoqued(true);
            return true;
        }
        if (piece->getIsWhite() == false && to_coordX == 0 && to_coordY == 2) {
            piece->goToPosition(0, 2);
            (piece->CNTMove)++;
            piece->setHasRoqued(true);
            Pieces* ally_piece = getPieceAt(0, 0);
            ally_piece->goToPosition(0, 3);
            (ally_piece->CNTMove)++;
            ally_piece->setHasRoqued(true);
            return true;
        }
    }
    return false;
}

bool Chessboard::littleRoque(Pieces* piece, int to_coordX, int to_coordY) {
    if (canLittleRoque(piece) && !pawnMenacingLittleRoque(piece)) {
        if (piece->getIsWhite() == true && to_coordX == 7 && to_coordY == 6) {
            piece->goToPosition(7, 6);
            (piece->CNTMove)++;
            piece->setHasRoqued(true);
            Pieces* ally_piece = getPieceAt(7, 7);
            ally_piece->goToPosition(7, 5);
            (ally_piece->CNTMove)++;
            ally_piece->setHasRoqued(true);
            return true;
        }
        if (piece->getIsWhite() == false && to_coordX == 0 && to_coordY == 6) {
            piece->goToPosition(0, 6);
            (piece->CNTMove)++;
            piece->setHasRoqued(true);
            Pieces* ally_piece = getPieceAt(0, 7);
            ally_piece->goToPosition(0, 5);
            (ally_piece->CNTMove)++;
            ally_piece->setHasRoqued(true);
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

bool Chessboard::notBrokenMove(Pieces* piece, const Pieces* target_piece){
    if (target_piece != nullptr && piece->getIsOnAMove() && target_piece->isKing())
        return false;
    if (target_piece != nullptr && piece->hasThisEffect(IMMORTALITY) && target_piece->isKing())
        return false;
    return true;
}

vector<glm::ivec2> Chessboard::getValidMoves(Pieces* piece) const {
    vector<glm::ivec2> valid_moves;
    vector<glm::ivec2> piece_moves = piece->getMoves();
    if (piece->getOverrideMoves() != nullptr){
        piece_moves = piece->getOverrideMoves()();
    } else if (piece->isPawn() && piece->getMoves().empty()) {
        const int pawnDirection = piece->getIsWhite() ? -1 : 1;
        const int currentX = piece->getCoordX();
        const int currentY = piece->getCoordY();
        const bool FirstMove = piece->getIsFirstMove();
        const int board_size = static_cast<int>(grid.size());
        const int board_limit = piece->getIsWhite() ? 0 : board_size - 1;
        const int en_passant_pos = board_limit - 3 * pawnDirection;
        if (
            (currentX + pawnDirection) * pawnDirection <= board_limit * pawnDirection
            && getPieceAt(currentX + pawnDirection, currentY) == nullptr
        ) valid_moves.emplace_back(currentX + pawnDirection, currentY );
        if (
            FirstMove
            && (currentX + 2 * pawnDirection) * pawnDirection <= board_limit * pawnDirection
            && isPathClear(currentX, currentY,currentX + 2 * pawnDirection,currentY, piece)
            && getPieceAt(currentX + 2 * pawnDirection, currentY) == nullptr
        ) valid_moves.emplace_back(currentX + 2 * pawnDirection, currentY);
        if (
            currentX == en_passant_pos
            && isInGrid(currentX, currentY - 1)
            && getPieceAt(currentX, currentY - 1) != nullptr
            && getPieceAt(currentX, currentY - 1)->isPawn()
            && !getPieceAt(currentX, currentY - 1)->getIsWhite()
            && getPieceAt(currentX, currentY - 1)->getTurnStamp() == 1
            ) valid_moves.emplace_back(currentX + pawnDirection, currentY - 1);
        if (
            currentX == en_passant_pos
            && isInGrid(currentX, currentY + 1)
            && getPieceAt(currentX, currentY + 1) != nullptr
            && getPieceAt(currentX, currentY + 1)->isPawn()
            && !getPieceAt(currentX, currentY + 1)->getIsWhite()
            && getPieceAt(currentX, currentY + 1)->getTurnStamp() == 1
        ) valid_moves.emplace_back(currentX + pawnDirection, currentY + 1);
        for (
            const vector<glm::ivec2> diagonalAttack = {{pawnDirection, -1}, {pawnDirection, 1}};
            const auto& offset : diagonalAttack
        ) {
            const int diagX = currentX + offset.x;
            const int diagY = currentY + offset.y;
            if (
                isInGrid(diagX, diagY)
                && getPieceAt(diagX, diagY) != nullptr
                && !isAlly(piece, getPieceAt(diagX, diagY))
            ) {
                valid_moves.emplace_back(diagX, diagY);
            }
        }
        return valid_moves;
    }
    for (const auto& move : piece_moves) {
        const int to_coordX = move.x;
        const int to_coordY = move.y;
        const bool inGrid = isInGrid(to_coordX, to_coordY);
        const bool emptySquare = getPieceAt(to_coordX, to_coordY) == nullptr;
        bool isTargetAlly = false;
        const bool notBroken = notBrokenMove(piece, getPieceAt(to_coordX, to_coordY));
        if (!emptySquare)
            isTargetAlly = isAlly(piece, getPieceAt(to_coordX, to_coordY));
        const bool knight = piece->isKnight();
        bool pathClear = true;
        if (!knight && piece->getMovesMode() != 3 || knight && piece->getMovesMode() != 0 && piece->getMovesMode() != 3)
            pathClear = isPathClear(to_coordX,to_coordY, piece);
        if (inGrid && (emptySquare || !isTargetAlly) && pathClear && notBroken)
            valid_moves.emplace_back(move);
    }
    return valid_moves;
}



bool Chessboard::isMovePossible(Pieces* piece, const int to_coordX, const int to_coordY) const {
    for (const auto& moves : getValidMoves(piece)) {
        if (moves.x == to_coordX && moves.y == to_coordY)
            return true;
    }
    return false;
}

bool Chessboard::movePiece(Pieces* piece, const int to_coordX, const int to_coordY) {
    const int coordX = piece->getCoordX();
    const int coordY = piece->getCoordY();
    piece->addToAllMovesDoneBefore(coordX, coordY);
    if (littleRoque(piece,to_coordX,to_coordY))
        return true;
    if (bigRoque(piece,to_coordX,to_coordY))
        return true;
    if (!isMoveable(piece))
        return false;
    if (!isMovePossible(piece, to_coordX, to_coordY))
        return false;
    if (KillInPassing(piece,to_coordX,to_coordY))
        return true;
    Pieces* target_piece = getPieceAt(to_coordX, to_coordY);
    if (target_piece == nullptr) {
        piece->goToPosition(to_coordX, to_coordY);
        piece->CNTMove++;
    } else {
        KillCheck(piece, target_piece);
    }
    PawnReachingEndOfBoard(piece);
    piece->activateEffect(MOVE_CHANGING);
    return true;
}


bool Chessboard::isKillable(const Pieces *piece,Pieces* target_piece) {
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

bool Chessboard::KillCheck(Pieces *piece, Pieces *target_piece) {
    const int coordX2 = target_piece->getCoordX();
    const int coordY2 = target_piece->getCoordY();
    if (!isKillable(piece,target_piece) || isAlly(piece,target_piece))
        return false;
    if (target_piece->hasThisEffect(CHANGE_CONTROL)){
        target_piece->activateEffect(CHANGE_CONTROL);
        return true;
    }
    piece->goToPosition(coordX2, coordY2);
    target_piece->gotUnalivedBy(piece, KILL_NORMAL);
    return true;
}

bool Chessboard::KillInPassing(Pieces *piece, const int to_coordX, const int to_coordY) {
    const int coordX1 = piece->getCoordX();
    if (!piece->isPawn() || piece->hasThisEffect(MOVE_CHANGING))
        return false;
    const int pawnDirection = piece->getIsWhite() ? -1 : 1;
    const int board_size = static_cast<int>(grid.size());
    const int board_limit = piece->getIsWhite() ? 0 : board_size - 1;
    const int en_passant_pos = board_limit - 3 * pawnDirection;
    const int coordX2 = to_coordX - pawnDirection;
    const int coordY2 = to_coordY;
    if (
        Pieces* realTargetPiece = getPieceAt(coordX2,coordY2);
        realTargetPiece != nullptr
        && isKillable(piece,realTargetPiece)
        && !isAlly(piece,realTargetPiece)
        && realTargetPiece->isPawn()
        && coordX1 == en_passant_pos && coordX2 == en_passant_pos
        && realTargetPiece->getTurnStamp() == 1
    ) {
        deletePiece(realTargetPiece);
        piece->goToPosition(to_coordX, to_coordY);
        piece->CNTMove++;
        realTargetPiece->gotUnalivedBy(piece, KILL_EN_PASSANT);
        return true;
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

bool Chessboard::isKilled(const Pieces *piece) const {
    for (const auto &row : grid) {
        for (const auto &cell : row) {
            if (cell.piece == piece) {
                return false;
            }
        }
    }
    return true;
}

bool Chessboard::isMoveable(const Pieces* piece) {
    for (const auto& e : piece->getActive_effects()) {
        if (e.effect == STUN) {
            return false;
        }
    }
    return true;
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