//
// Created by david on 24/01/2025.
//

#include "Chessboard.h"

#include <log.h>
#include <event_type.h>

#include "game_cfg.h"
#include "RenderEngine.h"
#include "GameEngine.h"

Chessboard* Chessboard::instance = nullptr;

Chessboard* Chessboard::getInstance() {
    if (!instance)
        return new Chessboard(BOARD_SIZE);
    return instance;
}

void Chessboard::placePiece(const int coordX, const int coordY, Pieces* piece) {
    if (coordX >= 0 && coordX < grid.size() && coordY >= 0 && coordY < grid.size()) {
        grid[coordX][coordY].piece = piece;
    } else {
        ltr_log_error("Chessboard::placePiece: Invalid piece coordinate : ", coordX, " ", coordY);
    }
}

Pieces* Chessboard::getPieceAt(const int coordX, const int coordY) const {
    if (coordX >= 0 && coordX < grid.size() && coordY >= 0 && coordY < grid.size()) {
        return grid[coordX][coordY].piece;
    }
    ltr_log_error("Chessboard::getPieceAt: Invalid piece coordinate : ", coordX, " ", coordY);
    return nullptr;
}

chessboard_cell* Chessboard::getCellAt(const int coordX, const int coordY) {
    if (coordX >= 0 && coordX < grid.size() && coordY >= 0 && coordY < grid.size()) {
        return &grid[coordX][coordY];
    }
    ltr_log_fatal("Chessboard::getCellAt: Invalid cell coordinates : ", coordX, " ", coordY);
    return &grid[0][0];
}

vector<Pieces*> Chessboard::getDeadList() const{
    return deadList;
}

void Chessboard::addToDeadList(Pieces* piece){
    deadList.emplace_back(piece);
    ltr_log_info(CONSOLE_COLOR_GREEN, "Added dead piece ", piece->getName());
}

void Chessboard::removeFromDeadList(Pieces* piece){
    std::erase(deadList, piece);
    ltr_log_info(CONSOLE_COLOR_RED, "Removed dead piece ", piece->getName());
}

void Chessboard::deletePiece (const Pieces* piece) {
    const int to_coordX = piece->getCoordX();
    const int to_coordY = piece->getCoordY();
    if (to_coordX >= 0 && to_coordX < grid.size() && to_coordY >= 0 && to_coordY < grid.size()) {
        grid[piece->getCoordX()][piece->getCoordY()].piece = nullptr;
    } else {
        ltr_log_error("Chessboard::deletePiece: Invalid piece coordinate : ", to_coordX, " ", to_coordY);
    }
}


bool Chessboard::isAlly(const Pieces *piece, const Pieces *target_piece) {
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

bool Chessboard::isPathClear(int endX, int endY, const Pieces* piece) const {
    return isPathClear(piece->getCoordX(), piece->getCoordY(), endX, endY, piece);
}

bool Chessboard::isPathClear(int startX, int startY, int endX, int endY, const Pieces* piece) const {
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



bool Chessboard::isEndangeredByWhite(const glm::ivec2 cell) {
    for (vector<Pieces*> piecesList = getAllPieces(); const auto character : piecesList) {
        if (!character->getIsWhite())
            continue;
        for (const auto move : getValidMoves(character))
            if (move == cell)
                return true;
    }
    return false;
}

bool Chessboard::isEndangeredByBlack(const glm::ivec2 cell) {
    for (vector<Pieces*> piecesList = getAllPieces(); const auto character : piecesList) {
        if (character->getIsWhite())
            continue;
        for (const auto move : getValidMoves(character))
            if (move == cell)
                return true;
    }
    return false;
}

bool Chessboard::hasJustFirstMove(Pieces* piece) {
    if (GameEngine::getInstance()->getPhaseNumber() - piece->getFirstNormalMovePhase() < 3)
        return true;
    return false;
}

bool Chessboard::notBrokenMove(const Pieces* piece, const Pieces* target_piece){
    if (target_piece != nullptr && piece->hasThisEffect(ONE_MORE_MOVE) && target_piece->isKing())
        return false;
    if (target_piece != nullptr && piece->hasThisEffect(IMMORTALITY) && target_piece->isKing())
        return false;
    return true;
}

vector<glm::ivec2> Chessboard::getValidMoves(const Pieces* piece) const {
    const piece_move* pieceMove = piece->getCurrentPieceMove();
    vector<glm::ivec2> moves_list = pieceMove->get_positions(glm::ivec2(piece->getCoordX(), piece->getCoordY()));
    if (pieceMove->pathAlwaysClear)
        return moves_list;
    vector<glm::ivec2> valid_moves;
    for (const auto& move : moves_list) {
        const int to_coordX = move.x;
        const int to_coordY = move.y;
        if (!isInGrid(to_coordX, to_coordY))
            ltr_log_fatal("Chessboard::getValidMoves: move is not in grid : ", to_coordX, to_coordY);
        const auto* target_piece = getPieceAt(to_coordX, to_coordY);
        const bool emptySquare = target_piece == nullptr;
        bool isTargetAlly = false;
        bool isTargetKing = false;
        if (!emptySquare) {
            isTargetAlly = isAlly(piece, target_piece);
            isTargetKing = target_piece->isKing();
        }
        bool pathClear = true;
        if (!pieceMove->ignoresObstacles)
            pathClear = isPathClear(to_coordX,to_coordY, piece);
        if (
            (emptySquare || (!isTargetAlly && pieceMove->canKill))
            && pathClear
            && (!isTargetKing || pieceMove->canKillKing)
            && notBrokenMove(piece, target_piece)
        ) valid_moves.emplace_back(move);
    }
    return valid_moves;
}



bool Chessboard::isMovePossible(const Pieces* piece, const int to_coordX, const int to_coordY) const {
    for (const auto& moves : getValidMoves(piece)) {
        if (moves.x == to_coordX && moves.y == to_coordY)
            return true;
    }
    return false;
}

bool Chessboard::movePiece(Pieces* piece, const int to_coordX, const int to_coordY) {
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
    if (Pieces* target_piece = getPieceAt(to_coordX, to_coordY); target_piece == nullptr)
        piece->goToPosition(to_coordX, to_coordY, piece->getIsOnAMove() ? MOVE_SUPPLEMENTARY : MOVE_NORMAL);
    else
        KillCheck(piece, target_piece);
    PawnReachingEndOfBoard(piece);
    piece->activateEffect(MOVE_CHANGING);
    piece->CheckEffectAmount(MOVE_CHANGING);
    return true;
}


bool Chessboard::isKillable(const Pieces *piece,Pieces* target_piece) {
    if (piece->isKing())
        return true;
    for (const auto& e : target_piece->getActive_effects()) {
        if (e->effect == SHIELD || e->effect == IMMORTALITY ) {
            target_piece->activateEffect(e->effect);
            target_piece->CheckEffectAmount(e->effect);
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
        target_piece->CheckEffectAmount(CHANGE_CONTROL);
        return true;
    }
    piece->goToPosition(coordX2, coordY2, piece->getIsOnAMove() ? MOVE_SUPPLEMENTARY : MOVE_NORMAL);
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
        && GameEngine::getInstance()->getPhaseNumber() - realTargetPiece->getFirstNormalMovePhase() < 3
    ) {
        deletePiece(realTargetPiece);
        piece->goToPosition(to_coordX, to_coordY, piece->getIsOnAMove() ? MOVE_SUPPLEMENTARY_EN_PASSANT : MOVE_EN_PASSANT);
        realTargetPiece->gotUnalivedBy(piece, KILL_EN_PASSANT);
        return true;
    }
    return false;
}

bool Chessboard::PawnReachingEndOfBoard(Pieces *piece) {
    const int coordX = piece->getCoordX();
    if (!piece->isPawn())
        return false;
    if (piece->getIsWhite()) {
        if (coordX == 0){
            ltr_log_info(CONSOLE_COLOR_RED, "Pawn reached end of board !!!");
            piece->setPiecesOrigin(QUEEN);
            piece->activateEffect(SUPP_RANGE);
            piece->CheckEffectAmount(SUPP_RANGE);
            piece->deleteEffect(IMMORTALITY);
            return true;
        }
    } else {
        if (coordX == BOARD_SIZE - 1){
            ltr_log_info(CONSOLE_COLOR_RED, "Pawn reached end of board !!!");
            piece->setPiecesOrigin(QUEEN);
            piece->activateEffect(SUPP_RANGE);
            piece->CheckEffectAmount(SUPP_RANGE);
            piece->deleteEffect(IMMORTALITY);
            return true;
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
        if (e->effect == STUN) {
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
            piece->goToPosition(7, 2, MOVE_ROQUED);
            Pieces* ally_piece = getPieceAt(7, 0);
            ally_piece->goToPosition(7, 3, MOVE_ROQUED);
            return true;
        }
        if (piece->getIsWhite() == false && to_coordX == 0 && to_coordY == 2) {
            piece->goToPosition(0, 2, MOVE_ROQUED);
            Pieces* ally_piece = getPieceAt(0, 0);
            ally_piece->goToPosition(0, 3, MOVE_ROQUED);
            return true;
        }
    }
    return false;
}

bool Chessboard::littleRoque(Pieces* piece, int to_coordX, int to_coordY) {
    if (canLittleRoque(piece) && !pawnMenacingLittleRoque(piece)) {
        if (piece->getIsWhite() == true && to_coordX == 7 && to_coordY == 6) {
            piece->goToPosition(7, 6, MOVE_ROQUED);
            Pieces* ally_piece = getPieceAt(7, 7);
            ally_piece->goToPosition(7, 5, MOVE_ROQUED);
            return true;
        }
        if (piece->getIsWhite() == false && to_coordX == 0 && to_coordY == 6) {
            piece->goToPosition(0, 6, MOVE_ROQUED);
            Pieces* ally_piece = getPieceAt(0, 7);
            ally_piece->goToPosition(0, 5, MOVE_ROQUED);
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

