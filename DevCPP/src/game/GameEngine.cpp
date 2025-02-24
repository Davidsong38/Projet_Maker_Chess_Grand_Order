//
// Created by sebas on 29/12/2024.
//

#include "GameEngine.h"

#include <clickables.h>
#include <cstring>

#include "effect_List.h"
#include "characters_List.h"
#include "pieces_List.h"
#include "piece_loader.h"
#include  "keys.h"
#include "log.h"
#include <iostream>
#include <set>
#include <uiElements.h>

#include "Gilgamesh.h"

GameEngine::GameEngine() : current_state(INITIALISATION), last_state(INITIALISATION) {
    ltr_log_info("Creating Game Engine");

    state_handlers[INITIALISATION]          = [this]() { handleInitialisation(); };

    state_handlers[START_WHITE_PHASE]       = [this]() { handleStartWhitePhase(); };
    state_handlers[SELECT_WHITE_PHASE]      = [this]() { handleSelectWhitePhase(); };
    state_handlers[MOVING_WHITE_PHASE]      = [this]() { handleMovingWhitePhase(); };
    state_handlers[CHECKING_WHITE_PHASE]    = [this]() { handleCheckingWhitePhase(); };
    state_handlers[END_WHITE_PHASE]         = [this]() { handleEndWhitePhase(); };

    state_handlers[START_BLACK_PHASE]       = [this]() { handleStartBlackPhase(); };
    state_handlers[SELECT_BLACK_PHASE]      = [this]() { handleSelectBlackPhase(); };
    state_handlers[MOVING_BLACK_PHASE]      = [this]() { handleMovingBlackPhase(); };
    state_handlers[CHECKING_BLACK_PHASE]    = [this]() { handleCheckingBlackPhase(); };
    state_handlers[END_BLACK_PHASE]         = [this]() { handleEndBlackPhase(); };

    state_handlers[SELECT_ANY]              = [this]() { handleSelectAny(); };

    state_handlers[END_GAME]                = [this]() { handleEndGame(); };
    state_handlers[GAME_CLOSE]              = [this]() { handleGameClose(); };

    keys_init();
}

void GameEngine::setState(const GameState state) {
    current_state = state;
}

void GameEngine::setLastState(const GameState state) {
    last_state = state;
}

GameState GameEngine::getCurrentState() const {
    return current_state;
}

GameState GameEngine::getLastState() const{
    return last_state;
}

void GameEngine::handleInitialisation() {
    srand (time(nullptr));
    loadEffectList();
    loadCharactersList();
    loadPiecesList();
    init_pieces();
    std::cout<< "---------------------------------------------------WHITE TURN "<< NB_Turn <<"---------------------------------------------------"<< std::endl;
    setLastState(current_state);
    setState(START_WHITE_PHASE);

}

void GameEngine::handleStartWhitePhase() {
    if (receivedClick) {
        receivedClick = false;
        receivedRightClick = false;
        Pieces* selectedPiece = Chessboard::getInstance()->getPieceAt(lastClickX, lastClickY);
        if (selectedPiece != nullptr && selectedPiece->getIsWhite()) {
            selectedPiece->selected = true;
            if (context->piece != nullptr) {
                context->piece->selected = false;
            }
            context->piece = selectedPiece;
            unloadPossibleMoves();
            setLastState(current_state);
            setState(SELECT_WHITE_PHASE);
        }
    }
}

void GameEngine::handleSelectWhitePhase() {
    if (context->piece == nullptr) {
        ltr_log_error("Impossible state in GameEngine::handleSelectWhitePhase()");
        setLastState(current_state);
        return;
    }

    unloadPossibleMoves();
    const vector<glm::ivec2> possible_moves = Chessboard::getInstance()->getValidMoves(context->piece);
    loadPossibleMoves(possible_moves, context->piece->getDefaultColor());

    if (!receivedClick && !receivedRightClick) {
        setLastState(current_state);
        return;
    }
    receivedClick = false;

    if (receivedRightClick){
        context->piece->setPieceGameMode(1);
        std::cout<< "Received right-click piece game mode"<< std::endl;
    }
    receivedRightClick = false;
    if (context->piece->getPieceGameMode() != 0){
        std::cout << "Unlimited BladeWorks" << std::endl;
        setLastState(current_state);
        setState((CHECKING_WHITE_PHASE));
        return;
    }
    if (context->piece->getCoordX() == lastClickX && context->piece->getCoordY() == lastClickY && !context->piece->getIsOnAMove()) {
        context->piece->selected = false;
        context->piece = nullptr;
        unloadPossibleMoves();
        setLastState(current_state);
        setState(START_WHITE_PHASE);
        return;
    }
    if (Chessboard::getInstance()->getPieceAt(lastClickX, lastClickY) != nullptr){
        context->target_piece = Chessboard::getInstance()->getPieceAt(lastClickX, lastClickY);
        //std::cout << context->target_piece->getPiecesOrigin() << std::endl;
    }
    if (Chessboard::getInstance()->movePiece(context->piece, lastClickX, lastClickY)){
        setLastState(current_state);
        setState(MOVING_WHITE_PHASE);
    }
}

void GameEngine::handleMovingWhitePhase() {
    setLastState(current_state);
    setState(CHECKING_WHITE_PHASE);
}

void GameEngine::handleCheckingWhitePhase() {
    unloadPossibleMoves();
    if (last_state == SELECT_WHITE_PHASE)
        receivedRightClick = false;
    if (!context->piece->SpellActivationCheck(context)){
        setLastState(current_state);
        receivedClick = false;
        return;
    }
    //std::cout << "SEIIIIBAAAAA" << std::endl;
    context->piece->setPieceGameMode(0);
    //std::cout<< context->piece->getPieceGameMode()<< " le pere fouettard " << std::endl;
    if (current_state == SELECT_WHITE_PHASE){
        std::cout << "CANCEL" << std::endl;
        receivedRightClick = false;
        return;
    }
    context->piece->setHasJustKilled(false);
    if (context->piece->getIsEvolved())
        std::cout << (context->piece->getIsWhite()? "White " : "Black ") << context->piece->getName() << " has evolved!" << std::endl;
    setLastState(current_state);
    setState(END_WHITE_PHASE);
}

void GameEngine::handleEndWhitePhase() {
    receivedClick = false;
    if (black_king->isHidden()) {
        std::cout<<"White has win!" << std::endl;
        setLastState(current_state);
        setState(END_GAME);
    } else {
        for (const auto& piece : Chessboard::getInstance()->getAllPieces()) {
            piece->updateEffectStatus();
            if (piece->getCharacters()== ARCEUID)
                piece->canEvolve(context);
            if (piece->getCharacters() == NITOCRIS_ALTER){
                piece->passive(context);
                piece->canEvolve(context);
            }
            if (piece->getCharacters() == GILGAMESH)
                piece->passive(context);
            if (piece != context->piece)
                piece->setNB_TurnWithoutMoving(piece->getNB_TurnWithoutMoving() + 1);
            if (piece == context->piece)
                piece->setNB_TurnWithoutMoving(0);
            if (!piece->getIsFirstMove())
                piece->setTurnStamp(piece->getTurnStamp() + 1);
        }
        NB_WhiteDeadLastPhase = NB_WhiteDead;
        NB_BlackDeadLastPhase = NB_BlackDead;
        std::cout<< "---------------------------------------------------BLACK TURN "<< NB_Turn <<"---------------------------------------------------"<< std::endl;
        setLastState(current_state);
        setState(START_BLACK_PHASE);
    }
}

void GameEngine::handleStartBlackPhase() {
    if (receivedClick) {
        receivedClick = false;
        receivedRightClick = false;
        Pieces* selectedPiece = Chessboard::getInstance()->getPieceAt(lastClickX, lastClickY);
        if (selectedPiece != nullptr && !selectedPiece->getIsWhite()) {
            selectedPiece->selected = true;
            if (context->piece != nullptr) {
                context->piece->selected = false;
            }
            context->piece = selectedPiece;
            unloadPossibleMoves();
            setLastState(current_state);
            setState(SELECT_BLACK_PHASE);
        }
    }
}

void GameEngine::handleSelectBlackPhase() {
    if (context->piece == nullptr) {
        ltr_log_error("Impossible state in GameEngine::handleSelectBlackPhase()");
        setLastState(current_state);
        return;
    }

    unloadPossibleMoves();
    const vector<glm::ivec2> possible_moves = Chessboard::getInstance()->getValidMoves(context->piece);
    loadPossibleMoves(possible_moves, context->piece->getDefaultColor());

    if (!receivedClick && !receivedRightClick) {
        setLastState(current_state);
        return;
    }
    receivedClick = false;

    if (receivedRightClick){
        context->piece->setPieceGameMode(1);
    }
    receivedRightClick = false;
    if (context->piece->getPieceGameMode() != 0) {
        setLastState(current_state);
        setState((CHECKING_BLACK_PHASE));
        return;
    }

    if (context->piece->getCoordX() == lastClickX && context->piece->getCoordY() == lastClickY && !context->piece->getIsOnAMove()) {
        context->piece->selected = false;
        context->piece = nullptr;
        unloadPossibleMoves();
        setLastState(current_state);
        setState(START_BLACK_PHASE);
        return;
    }
    if (Chessboard::getInstance()->getPieceAt(lastClickX, lastClickY) != nullptr){
        context->target_piece = Chessboard::getInstance()->getPieceAt(lastClickX, lastClickY);
        //std::cout << context->target_piece->getPiecesOrigin() << std::endl;
    }
    if (Chessboard::getInstance()->movePiece(context->piece, lastClickX, lastClickY)){

        setLastState(current_state);
        setState(MOVING_BLACK_PHASE);
    }
}

void GameEngine::handleMovingBlackPhase() {
    setLastState(current_state);
    setState(CHECKING_BLACK_PHASE);
}

void GameEngine::handleCheckingBlackPhase() {
    unloadPossibleMoves();
    if (last_state == SELECT_BLACK_PHASE)
        receivedRightClick = false;
    if (!context->piece->SpellActivationCheck(context)){
        setLastState(current_state);
        receivedClick = false;
        return;
    }
    std::cout << "SEIIIIBAAAAA" << std::endl;
    context->piece->setPieceGameMode(0);
    if (current_state == SELECT_BLACK_PHASE){
        receivedRightClick = false;
        return;
    }
    context->piece->setHasJustKilled(false);
    if (context->piece->getIsEvolved())
        std::cout << (context->piece->getIsWhite()? "White " : "Black ") << context->piece->getName() << " has evolved!" << std::endl;
    setLastState(current_state);
    setState(END_BLACK_PHASE);
}


void GameEngine::handleEndBlackPhase() {
    receivedClick = false;
    if (white_king->isHidden()) {
        std::cout<<"Black has win!" << std::endl;
        setLastState(current_state);
        setState(END_GAME);
    } else {
        NB_Turn++;
        for (const auto& piece : Chessboard::getInstance()->getAllPieces()) {
            piece->updateEffectStatus();
            if (piece->getCharacters()== ARCEUID)
                piece->canEvolve(context);
            if (piece->getCharacters() == NITOCRIS_ALTER){
                piece->passive(context);
                piece->canEvolve(context);
            }
            if (piece->getCharacters() == GILGAMESH)
                piece->passive(context);
            if (piece != context->piece)
                piece->setNB_TurnWithoutMoving(piece->getNB_TurnWithoutMoving() + 1);
            if (piece == context->piece)
                piece->setNB_TurnWithoutMoving(0);
            if (!piece->getIsFirstMove())
                piece->setTurnStamp(piece->getTurnStamp() + 1);
        }
        NB_WhiteDeadLastPhase = NB_WhiteDead;
        NB_BlackDeadLastPhase = NB_BlackDead;
        std::cout<< "---------------------------------------------------WHITE TURN "<< NB_Turn <<"---------------------------------------------------"<< std::endl;
        setLastState(current_state);
        setState(START_WHITE_PHASE);
    }
}

void GameEngine::handleSelectAny() {
    if (receivedClick == false)
        return;
    receivedClick = false;
    Pieces* selectedPiece = Chessboard::getInstance()->getPieceAt(lastClickX, lastClickY);
    if (selectedPiece == nullptr) {

        if (required_selection.emptys == 0 || required_selection.emptys == current_selection->empty_pieces.size())
            return;
    }
    if (selectedPiece != nullptr && !selectedPiece->getIsWhite()) {
        selectedPiece->selected = true;
        if (context->piece != nullptr) {
            context->piece->selected = false;
        }
        context->piece = selectedPiece;
        unloadPossibleMoves();
        setLastState(current_state);
        setState(SELECT_BLACK_PHASE);
    }
}

void GameEngine::handleEndGame() {
    cout << "END GAME" << endl;
    saveLastState();
    setState(GAME_CLOSE);
}

void GameEngine::handleGameClose() {

}

void GameEngine::requestSelection(const selection_request_type to_select) {
    saveLastState();
    current_state = SELECT_ANY;
    required_selection = to_select;
    current_selection = new selection_type();
}


GameEngine::~GameEngine() {
    ltr_log_info("Destroying Game Engine");
}

GameEngine* GameEngine::getInstance() {
    if (instance == nullptr) {
        instance = new GameEngine();
    }
    return instance;
}

void GameEngine::update(double deltaTime_ms) {
    state_handlers[current_state]();
    if (get_key(KEY_P)->didKeyGetPressed())
        screen_blocker->toggle();
    keys_update();
}

void GameEngine::clickBoardAtPos(int x, int y) {
    receivedClick = true;
    lastClickX = x;
    lastClickY = y;
}
void GameEngine::inputRightClick(){
    receivedRightClick = true;

}



int GameEngine::getLastClickX() const {
    return lastClickX;
}

int GameEngine::getLastClickY() const {
    return lastClickY;
}

Pieces* GameEngine::getLastPieceTouchedByEffect() const {
    return lastPieceTouchedByEffect;
}

void GameEngine::setLastPieceTouchedByEffect(Pieces* last_piece_touched_by_effect){
    lastPieceTouchedByEffect = last_piece_touched_by_effect;
}

void GameEngine::addEvent(Event* event) {
    current_phase_context.events.emplace_back(event);
    ltr_log_debug("Event added : ", event->eventType, ", at turn : ", event->eventTurn);
}
