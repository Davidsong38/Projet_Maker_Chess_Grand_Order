//
// Created by sebas on 29/12/2024.
//

#include "GameEngine.h"

#include "effect_List.h"
#include "characters_List.h"
#include "pieces_List.h"
#include "piece_loader.h"
#include  "keys.h"
#include "log.h"
#include <iostream>
#include <VAO.h>

GameEngine::GameEngine() : current_state(INITIALISATION) {
        state_handlers[INITIALISATION] = [this]() { handleInitialisation(); };
        state_handlers[START_WHITE_PHASE] = [this]() { handleStartWhitePhase(); };
        state_handlers[SELECT_WHITE_PHASE] = [this]() { handleSelectWhitePhase(); };
        state_handlers[MOVING_WHITE_PHASE] = [this]() { handleMovingWhitePhase(); };
        state_handlers[CHECKING_WHITE_PHASE] = [this]() { handleCheckingWhitePhase(); };
        state_handlers[END_WHITE_PHASE] = [this]() { handleEndWhitePhase(); };
        state_handlers[START_BLACK_PHASE] = [this]() { handleStartBlackPhase(); };
        state_handlers[SELECT_BLACK_PHASE] = [this]() { handleSelectBlackPhase(); };
        state_handlers[MOVING_BLACK_PHASE] = [this]() { handleMovingBlackPhase(); };
        state_handlers[CHECKING_BLACK_PHASE] = [this]() { handleCheckingBlackPhase(); };
        state_handlers[END_BLACK_PHASE] = [this]() { handleEndBlackPhase(); };
        state_handlers[END_GAME] = [this]() { handleEndGame(); };
        state_handlers[GAME_CLOSE] = [this]() { handleGameClose(); };
        keys_init();
        std::cout << "Creating Game Engine" << std::endl;

}

void GameEngine::setState(GameState state) {
    current_state = state;
}

GameState GameEngine::get_current_state() const {
    return current_state;
}

void GameEngine::handleInitialisation() {
    loadEffectList();
    loadCharactersList();
    loadPiecesList();
    context->chessboard = Chessboard::getInstance();
    init_pieces();
    setState(START_WHITE_PHASE);
}



void GameEngine::handleStartWhitePhase() {
    //std::cout << "Select White Piece to move" << std::endl;
    //int x =
    //clickDifferentOfLastPos(x,y);
    if (receivedClick) {
        receivedClick = false;
        Pieces* selectedPiece = Chessboard::getInstance()->getGrid()[lastClickX][lastClickY];
        if (selectedPiece != nullptr && selectedPiece->getIsWhite()) {
            selectedPiece->selected = true;
            if (context->piece != nullptr) {
                context->piece->selected = false;
            }
            context->piece = selectedPiece;
            setState(SELECT_WHITE_PHASE);
        }
    }
}
void GameEngine::handleSelectWhitePhase() {
    if (!receivedClick)
        return;
    receivedClick = false;
    if (context->piece == nullptr) {
        log(LOG_ERROR,"Impossible state in GameEngine::handleSelectWhitePhase()");
        return;
    }
    if (context->piece->getCoordX() == lastClickX && context->piece->getCoordY() == lastClickY) {
        context->piece->selected = false;
        context->piece = nullptr;
        setState(START_WHITE_PHASE);
        return;
    }
    Chessboard::getInstance()->movePiece(context->piece, lastClickX, lastClickY);    ///TODO Move
    //setState(MOVING_WHITE_PHASE);
}

void GameEngine::handleMovingWhitePhase() {

    setState(CHECKING_WHITE_PHASE);
}

void GameEngine::handleCheckingWhitePhase() {
    setState(END_WHITE_PHASE);
}

void GameEngine::handleEndWhitePhase() {
    if (context->chessboard->isKilled(context->target_piece) && context->target_piece->isKing() ) {
        setState(END_GAME);
    } else {
        setState(START_BLACK_PHASE);
    }
}

void GameEngine::handleStartBlackPhase() {
    setState(MOVING_BLACK_PHASE);
}

void GameEngine::handleSelectBlackPhase() {
    if (true) {
        setState(MOVING_BLACK_PHASE);
    } else {
        setState(START_BLACK_PHASE);
    }
}

void GameEngine::handleMovingBlackPhase() {
    setState(CHECKING_BLACK_PHASE);
}

void GameEngine::handleCheckingBlackPhase() {
    setState(END_BLACK_PHASE);
}


void GameEngine::handleEndBlackPhase() {
    if (context->chessboard->isKilled(context->target_piece) && context->target_piece->isKing() ) {
        setState(END_GAME);
    } else {
        setState(START_WHITE_PHASE);
    }
}

void GameEngine::handleEndGame() {
    cout << "END GAME" << endl;
    setState(GAME_CLOSE);
}

void GameEngine::handleGameClose() {
}




GameEngine::~GameEngine() {
    std::cout << "Destroying Game Engine" << std::endl;
}

GameEngine* GameEngine::getInstance() {
    if (instance == nullptr) {
        instance = new GameEngine();
    }
    return instance;
}

void GameEngine::update(double deltaTime_ms) {
    state_handlers[current_state]();
    keys_update();
}

void GameEngine::clickBoardAtPos(int x, int y) {
    receivedClick = true;
    lastClickX = x;
    lastClickY = y;
}





