//
// Created by sebas on 29/12/2024.
//

#include "GameEngine.h"

#include <clickables.h>
#include <cstring>

#include "effects.h"
#include "characters_List.h"
#include "pieces_List.h"
#include "piece_loader.h"
#include  "keys.h"
#include "log.h"
#include <iostream>
#include <set>
#include <uiElements.h>

#include "Gilgamesh.h"

std::string game_state_to_string[GAME_STATE_COUNT];

void game_state_names_Init() {
    game_state_to_string[INITIALISATION] = "initialisation";
    game_state_to_string[START_WHITE_PHASE] = "start_white_phase";
    game_state_to_string[SELECT_WHITE_PHASE] = "select_white_phase";
    game_state_to_string[MOVING_WHITE_PHASE] = "moving_white_phase";
    game_state_to_string[CHECKING_WHITE_PHASE] = "checking_white_phase";
    game_state_to_string[END_WHITE_PHASE] = "end_white_phase";
    game_state_to_string[START_BLACK_PHASE] = "start_black_phase";
    game_state_to_string[SELECT_BLACK_PHASE] = "select_black_phase";
    game_state_to_string[MOVING_BLACK_PHASE] = "moving_black_phase";
    game_state_to_string[CHECKING_BLACK_PHASE] = "checking_black_phase";
    game_state_to_string[END_BLACK_PHASE] = "end_black_phase";
    game_state_to_string[SELECT_ANY] = "select_any";
    game_state_to_string[END_GAME] = "end_game";
    game_state_to_string[GAME_CLOSE] = "game_close";
}

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

    game_state_names_Init();

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
    srand(time(nullptr));
    loadEffectList();
    loadCharactersList();
    loadPiecesList();
    init_pieces();
    std::cout<< "---------------------------------------------------WHITE TURN "<< NB_Turn <<"---------------------------------------------------"<< std::endl;
    goToState(START_WHITE_PHASE);

}

void GameEngine::handleStartWhitePhase() {
    selection_type* selection = getSelection();
    if (selection == nullptr) {
        selection_request_type request;
        request.whites = 1;
        request.instantValidation = true;
        requestSelection(request);
        return;
    }
    Pieces* selectedPiece = selection->white_pieces[0]->piece;
    current_phase_context->firstSelectedPiece = selectedPiece;
    /// TODO change
    selectedPiece->selected = true;
    if (context->piece != nullptr) {
        context->piece->selected = false;
    }
    context->piece = selectedPiece;
    /// TODO change
    goToState(SELECT_WHITE_PHASE);
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
    }
    receivedRightClick = false;
    if (context->piece->getPieceGameMode() != 0){
        goToState(CHECKING_WHITE_PHASE);
        return;
    }
    if (context->piece->getCoordX() == lastClickX && context->piece->getCoordY() == lastClickY && !context->piece->getIsOnAMove()) {
        context->piece->selected = false;
        context->piece = nullptr;
        unloadPossibleMoves();
        goToState(START_WHITE_PHASE);
        return;
    }
    if (Chessboard::getInstance()->getPieceAt(lastClickX, lastClickY) != nullptr){
        context->target_piece = Chessboard::getInstance()->getPieceAt(lastClickX, lastClickY);
    }
    if (Chessboard::getInstance()->movePiece(context->piece, lastClickX, lastClickY)) {
        goToState(MOVING_WHITE_PHASE);
    }
}

void GameEngine::handleMovingWhitePhase() {
    goToState(CHECKING_WHITE_PHASE);
}

void GameEngine::handleCheckingWhitePhase() {
    unloadPossibleMoves();
    if (last_main_state == SELECT_WHITE_PHASE)
        receivedRightClick = false;
    if (!context->piece->SpellActivationCheck(context)) {
        last_main_state = current_state;
        receivedClick = false;
        return;
    }
    context->piece->setPieceGameMode(0);
    if (current_state == SELECT_WHITE_PHASE){
        ltr_log_error("\"Ne doit plus arriver\" - Dave 25/02/2025");
        receivedRightClick = false;
        return;
    }
    context->piece->setHasJustKilled(false);
    if (context->piece->getIsEvolved())
        ltr_log_info(
            context->piece->getIsWhite()? "White " : "Black ",
            context->piece->getName(),
            " has evolved!"
        );
    goToState(END_WHITE_PHASE);
}

void GameEngine::handleEndWhitePhase() {
    receivedClick = false;
    if (black_king->isHidden()) {
        std::cout << "White has win!" << std::endl;
        goToState(END_GAME);
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
        goToState(START_BLACK_PHASE);
    }
    push_phase_context();
}

void GameEngine::handleStartBlackPhase() {
    selection_type* selection = getSelection();
    if (selection == nullptr) {
        selection_request_type request;
        request.blacks = 1;
        request.instantValidation = true;
        requestSelection(request);
        return;
    }
    Pieces* selectedPiece = selection->black_pieces[0]->piece;
    current_phase_context->firstSelectedPiece = selectedPiece;
    /// TODO change
    selectedPiece->selected = true;
    if (context->piece != nullptr) {
        context->piece->selected = false;
    }
    context->piece = selectedPiece;
    /// TODO change
    goToState(SELECT_BLACK_PHASE);
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
        return;
    }
    receivedClick = false;

    if (receivedRightClick){
        context->piece->setPieceGameMode(1);
    }
    receivedRightClick = false;
    if (context->piece->getPieceGameMode() != 0) {
        goToState(CHECKING_BLACK_PHASE);
        return;
    }

    if (context->piece->getCoordX() == lastClickX && context->piece->getCoordY() == lastClickY && !context->piece->getIsOnAMove()) {
        context->piece->selected = false;
        context->piece = nullptr;
        unloadPossibleMoves();
        goToState(START_BLACK_PHASE);
        return;
    }
    if (Chessboard::getInstance()->getPieceAt(lastClickX, lastClickY) != nullptr){
        context->target_piece = Chessboard::getInstance()->getPieceAt(lastClickX, lastClickY);
    }
    if (Chessboard::getInstance()->movePiece(context->piece, lastClickX, lastClickY)){
        goToState(MOVING_BLACK_PHASE);
    }
}

void GameEngine::handleMovingBlackPhase() {
    goToState(CHECKING_BLACK_PHASE);
}

void GameEngine::handleCheckingBlackPhase() {
    unloadPossibleMoves();
    if (last_main_state == SELECT_BLACK_PHASE)
        receivedRightClick = false;
    if (!context->piece->SpellActivationCheck(context)) {
        last_main_state = current_state;
        receivedClick = false;
        return;
    }
    context->piece->setPieceGameMode(0);
    if (current_state == SELECT_BLACK_PHASE){
        ltr_log_error("\"Ne doit plus arriver\" - Dave 25/02/2025");
        receivedRightClick = false;
        return;
    }
    context->piece->setHasJustKilled(false);
    if (context->piece->getIsEvolved())
        ltr_log_info(
            context->piece->getIsWhite()? "White " : "Black ",
            context->piece->getName(),
            " has evolved!"
        );
    goToState(END_BLACK_PHASE);
}


void GameEngine::handleEndBlackPhase() {
    receivedClick = false;
    if (white_king->isHidden()) {
        std::cout << "Black has win!" << std::endl;
        goToState(END_GAME);
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
        goToState(START_WHITE_PHASE);
    }
    push_phase_context();
}

void GameEngine::handleSelectAny() {
    updateSelectionSpriteGroup();
    if (current_selection == nullptr) {
        ltr_log_error("No current_selection");
        return;
    }
    if (
        (required_selection.instantValidation || selectionGotValidated)
        && required_selection.whites == current_selection->whites
        && required_selection.blacks == current_selection->blacks
        && required_selection.emptys == current_selection->emptys
    ) {
        ltr_log_debug("Selection complete");
        loadLastState();
        selectionGotValidated = false;
        if (required_selection.effect_instance != nullptr) {
            ltr_log_debug("Selection was from effect_instance");
            for (const auto& cell : current_selection->white_pieces)
                required_selection.effect_instance->target_pieces.emplace_back(cell->piece);
            for (const auto& cell : current_selection->black_pieces)
                required_selection.effect_instance->target_pieces.emplace_back(cell->piece);
            for (const auto& cell : current_selection->empty_pieces)
                required_selection.effect_instance->target_cells.emplace_back(cell);
            for (const auto& sel : current_selection->white_pieces)
                sel->selected = false;
            for (const auto& sel : current_selection->black_pieces)
                sel->selected = false;
            for (const auto& sel : current_selection->empty_pieces)
                sel->selected = false;
            delete current_selection;
            current_selection = nullptr;
        }
        return;
    }
    if (receivedClick == false && ghostClick == false)
        return;
    receivedClick = false;
    ghostClick = false;
    chessboard_cell* selectedCell = Chessboard::getInstance()->getCellAt(lastClickX, lastClickY);
    if (ranges::find(required_selection.banned_cells,selectedCell) != required_selection.banned_cells.end())
        return;
    if (selectedCell->piece == nullptr) {
        if (
            required_selection.emptys <= current_selection->emptys
            && !selectedCell->selected
        ) return;
        if (selectedCell->selected) {
            selectedCell->selected = false;
            current_selection->emptys--;
            std::erase(
                current_selection->empty_pieces,
                selectedCell
            );
            ltr_log_debug("Deselecting cell at ", selectedCell->pos.x, " ", selectedCell->pos.y);
            return;
        }
        selectedCell->selected = true;
        current_selection->emptys++;
        current_selection->empty_pieces.emplace_back(selectedCell);
        ltr_log_debug("Selecting cell at ", selectedCell->pos.x, " ", selectedCell->pos.y);
        return;
    }
    if (selectedCell->piece->getIsWhite()) {
        if (
            required_selection.whites <= current_selection->whites
            && !selectedCell->selected
        ) return;
        if (selectedCell->selected) {
            selectedCell->selected = false;
            current_selection->whites--;
            std::erase(
                current_selection->white_pieces,
                selectedCell
            );
            ltr_log_debug("Deselecting piece at ", selectedCell->pos.x, " ", selectedCell->pos.y);
            return;
        }
        selectedCell->selected = true;
        current_selection->whites++;
        current_selection->white_pieces.emplace_back(selectedCell);
        ltr_log_debug("Selecting piece at ", selectedCell->pos.x, " ", selectedCell->pos.y);
        return;
    }
    if (
        required_selection.blacks <= current_selection->blacks
        && !selectedCell->selected
    ) return;
    if (selectedCell->selected) {
        selectedCell->selected = false;
        current_selection->blacks--;
        std::erase(
            current_selection->black_pieces,
            selectedCell
        );
        ltr_log_debug("Deselecting piece at ", selectedCell->pos.x, " ", selectedCell->pos.y);
        return;
    }
    selectedCell->selected = true;
    current_selection->blacks++;
    current_selection->black_pieces.emplace_back(selectedCell);
    ltr_log_debug("Selecting piece at ", selectedCell->pos.x, " ", selectedCell->pos.y);
}

void GameEngine::handleEndGame() {
    cout << "END GAME" << endl;
    goToState(GAME_CLOSE);
}

void GameEngine::handleGameClose() {

}

void GameEngine::requestSelection(const selection_request_type& to_select) {
    goToState(SELECT_ANY);
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
    if (get_key(KEY_O)->didKeyGetPressed())
        ltr_log_debug(
            "Current State: ", current_state,
            ", Last State: ", last_state,
            "\n\trequested_selection.whites: ", required_selection.whites,
            "\n\trequested_selection.blacks: ", required_selection.blacks,
            "\n\trequested_selection.emptys: ", required_selection.emptys,
            "\n\tcurrent_selection.whites: ", (current_selection == nullptr) ? -1 : current_selection->whites,
            "\n\tcurrent_selection.blacks: ", (current_selection == nullptr) ? -1 : current_selection->blacks,
            "\n\tcurrent_selection.emptys: ", (current_selection == nullptr) ? -1 : current_selection->emptys
            );
    if (get_key(KEY_SPACE)->didKeyGetPressed())
        validateSelection();
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

void GameEngine::addEvent(Event* event) const {
    current_phase_context->events.emplace_back(event);
    ltr_log_debug("Event added : ", event->eventType, ", at turn : ", event->eventTurn);
}

void GameEngine::push_phase_context() {
    phase_contexts.emplace_back(current_phase_context);
    current_phase_context = new phase_context_type();
    current_phase_context->phaseNumber = static_cast<int>(phase_contexts.size());
    current_phase_context->turnNumber = NB_Turn;
}

void GameEngine::validateSelection() {
    if (
        required_selection.whites == current_selection->whites
        && required_selection.blacks == current_selection->blacks
        && required_selection.emptys == current_selection->emptys
    ) {
        selectionGotValidated = true;
    } else {
        ltr_log_warn(
            "Invalid Selection, please select : ",
            required_selection.whites,
            " whites, ",
            required_selection.blacks,
            " blacks and ",
            required_selection.emptys,
            " empty cells"
        );
    }
}

selection_type* GameEngine::getSelection() {
    if (current_selection == nullptr)
        return nullptr;
    for (const auto& sel : current_selection->white_pieces)
        sel->selected = false;
    for (const auto& sel : current_selection->black_pieces)
        sel->selected = false;
    for (const auto& sel : current_selection->empty_pieces)
        sel->selected = false;
    selection_type* output = current_selection;
    current_selection = nullptr;
    return output;
}