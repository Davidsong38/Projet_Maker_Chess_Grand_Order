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
        state_handlers[SELECT_PIECE_GAMEMODE_WHITE_PHASE] = [this](){handleSelectPieceGamemodeWhitePhase(); };
        state_handlers[SELECT_WHITE_PHASE] = [this]() { handleSelectWhitePhase(); };
        state_handlers[MOVING_WHITE_PHASE] = [this]() { handleMovingWhitePhase(); };
        state_handlers[CHECKING_WHITE_PHASE] = [this]() { handleCheckingWhitePhase(); };
        state_handlers[END_WHITE_PHASE] = [this]() { handleEndWhitePhase(); };
        state_handlers[START_BLACK_PHASE] = [this]() { handleStartBlackPhase(); };
        state_handlers[SELECT_PIECE_GAMEMODE_BLACK_PHASE] = [this](){handleSelectPieceGamemodeBlackPhase(); };
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

void GameEngine::setBlackKing(Pieces *piece) const {
    context->black_king = piece;
}

void GameEngine::setWhiteKing(Pieces *piece) const {
    context->white_king = piece;
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
    std::cout<< "---------------------------------------------------WHITE TURN "<< NB_Turn <<"---------------------------------------------------"<< std::endl;
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

            setState(SELECT_PIECE_GAMEMODE_WHITE_PHASE);
        }
    }
}

void GameEngine::handleSelectPieceGamemodeWhitePhase()
{
    context->piece->setPieceGameMode();
    setState(SELECT_WHITE_PHASE);
}

void GameEngine::handleSelectWhitePhase() {
    if (!receivedClick)
        return;
    receivedClick = false;
    if (context->piece == nullptr) {
        log(LOG_ERROR,"Impossible state in GameEngine::handleSelectWhitePhase()");
        return;
    }
    //std::cout << "ababababala" << std::endl;

    if (context->piece->getCoordX() == lastClickX && context->piece->getCoordY() == lastClickY) {
        context->piece->selected = false;
        context->piece = nullptr;
        setState(START_WHITE_PHASE);
        return;
    }
    if (context->piece->getPieceGameMode() != 0) {
        setState((CHECKING_WHITE_PHASE));
        return;
    }

    if (Chessboard::getInstance()->movePiece(context->piece, lastClickX, lastClickY))
        setState(MOVING_WHITE_PHASE);
}

void GameEngine::handleMovingWhitePhase() {

    setState(CHECKING_WHITE_PHASE);
}

void GameEngine::handleCheckingWhitePhase() {
    context->piece->SpellActivationCheck(context);
    context->piece->setHasJustKilled(false);
    if (context->piece->getIsEvolved())
        std::cout << (context->piece->getIsWhite()? "White " : "Black ") << context->piece->getName() << " has evolved!" << std::endl;
    setState(END_WHITE_PHASE);
}

void GameEngine::handleEndWhitePhase() {
    if (context->black_king->isHidden()) {
        std::cout<<"White has win!" << std::endl;
        setState(END_GAME);
    } else {
        for (const auto& piece : Chessboard::getInstance()->getAllPieces()) {
            if (piece->getIsWhite()) {
                piece->updateEffectStatus();
                if (!piece->getIsFirstMove())
                    piece->setTurnStamp(piece->getTurnStamp() + 1);
            }
        }
        std::cout<< "---------------------------------------------------BLACK TURN "<< NB_Turn <<"---------------------------------------------------"<< std::endl;
        setState(START_BLACK_PHASE);
    }
}

void GameEngine::handleStartBlackPhase() {
    if (receivedClick) {
        receivedClick = false;
        Pieces* selectedPiece = Chessboard::getInstance()->getGrid()[lastClickX][lastClickY];
        if (selectedPiece != nullptr && !selectedPiece->getIsWhite()) {
            selectedPiece->selected = true;
            if (context->piece != nullptr) {
                context->piece->selected = false;
            }
            context->piece = selectedPiece;
            setState(SELECT_PIECE_GAMEMODE_BLACK_PHASE);
        }
    }
}

void GameEngine::handleSelectPieceGamemodeBlackPhase()
{
    context->piece->setPieceGameMode();
    setState(SELECT_BLACK_PHASE);
}


void GameEngine::handleSelectBlackPhase() {
    if (!receivedClick)
        return;
    receivedClick = false;
    if (context->piece == nullptr) {
        log(LOG_ERROR,"Impossible state in GameEngine::handleSelectBlackPhase()");
        return;
    }

    if (context->piece->getCoordX() == lastClickX && context->piece->getCoordY() == lastClickY) {
        context->piece->selected = false;
        context->piece = nullptr;
        setState(START_BLACK_PHASE);
        return;
    }
    if (context->piece->getPieceGameMode() != 0) {
        setState((CHECKING_BLACK_PHASE));
        return;
    }
    if (Chessboard::getInstance()->movePiece(context->piece, lastClickX, lastClickY))
        setState(MOVING_BLACK_PHASE);
}

void GameEngine::handleMovingBlackPhase() {
    setState(CHECKING_BLACK_PHASE);
}

void GameEngine::handleCheckingBlackPhase() {
    context->piece->SpellActivationCheck(context);
    context->piece->setHasJustKilled(false);
    if (context->piece->getIsEvolved())
        std::cout << (context->piece->getIsWhite()? "White " : "Black ") << context->piece->getName() << " has evolved!" << std::endl;
    setState(END_BLACK_PHASE);
}


void GameEngine::handleEndBlackPhase() {
    if (context->white_king->isHidden()) {
        std::cout<<"Black has win!" << std::endl;
        setState(END_GAME);
    } else {
        NB_Turn++;
        for (const auto& piece : Chessboard::getInstance()->getAllPieces()) {
            if (!piece->getIsWhite()) {
                piece->updateEffectStatus();
                if (!piece->getIsFirstMove())
                    piece->setTurnStamp(piece->getTurnStamp() + 1);
            }
        }
        std::cout<< "---------------------------------------------------WHITE TURN "<< NB_Turn <<"---------------------------------------------------"<< std::endl;
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

int GameEngine::getLastClickX() const {
    return lastClickX;
}

int GameEngine::getLastClickY() const {
    return lastClickY;
}


