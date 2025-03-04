//
// Created by sebas on 29/12/2024.
//

#include "GameEngine.h"

#include <clickables.h>
#include <cstring>
#include <game_cfg.h>

#include "effects.h"
#include "characters_List.h"
#include "pieces_List.h"
#include "piece_loader.h"
#include "keys.h"
#include "log.h"
#include <iostream>
#include <set>
#include <uiElements.h>
#include <piece_moves.h>
#include <board_pattern.h>

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

    state_handlers[START_WHITE_PHASE]       = [this]() { handleAnyStartPhase(true); };
    state_handlers[SELECT_WHITE_PHASE]      = [this]() { handleAnySelectPhase(true); };
    state_handlers[MOVING_WHITE_PHASE]      = [this]() { handleAnyMovingPhase(true); };
    state_handlers[CHECKING_WHITE_PHASE]    = [this]() { handleAnyCheckingPhase(true); };
    state_handlers[END_WHITE_PHASE]         = [this]() { handleAnyEndPhase(true); };

    state_handlers[START_BLACK_PHASE]       = [this]() { handleAnyStartPhase(false); };
    state_handlers[SELECT_BLACK_PHASE]      = [this]() { handleAnySelectPhase(false); };
    state_handlers[MOVING_BLACK_PHASE]      = [this]() { handleAnyMovingPhase(false); };
    state_handlers[CHECKING_BLACK_PHASE]    = [this]() { handleAnyCheckingPhase(false); };
    state_handlers[END_BLACK_PHASE]         = [this]() { handleAnyEndPhase(false); };

    state_handlers[SELECT_ANY]              = [this]() { handleSelectAny(); };

    state_handlers[END_GAME]                = [this]() { handleEndGame(); };
    state_handlers[GAME_CLOSE]              = [this]() { handleGameClose(); };

    keys_init();
}

GameEngine::~GameEngine() {
    ltr_log_info("Destroying Game Engine");
}

GameEngine* GameEngine::getInstance() {
    if (instance == nullptr)
        instance = new GameEngine();
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





// Selection

void GameEngine::requestSelection(const selection_request_type& to_select) {
    deselectAllPieces();
    goToState(SELECT_ANY);
    required_selection = to_select;
    current_selection = new selection_type();
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

void GameEngine::deselectAllPieces() {
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            auto* piece = Chessboard::getInstance()->getPieceAt(x, y);
            if (piece == nullptr)
                continue;
            piece->selected = false;
        }
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





// Mouse inputs

void GameEngine::clickBoardAtPos(int x, int y) {
    receivedClick = true;
    lastClickX = x;
    lastClickY = y;
}





// Events

void GameEngine::registerEvent(Event* event) const {
    current_phase_context->events.emplace_back(event);
    for (auto* piece : event->getAllConcernedPieces())
        piece->addEvent(event);
    ltr_log_debug(
        "Event added : ",
        event_type_to_string[event->eventType],
        ", at turn : ",
        event->eventTurn
    );
}

std::vector<Event*> GameEngine::getAllKillEvents() const {
    std::vector<Event*> selected_events;
    for (const auto& phase_context : phase_contexts) {
        for (auto* event : phase_context->events) {
            if (event == nullptr) ltr_log_error("Pieces::getAllKillEvents: Event nullptr");
            if (event->eventType == EVENT_KILL) selected_events.emplace_back(event);
        }
    }
    return selected_events;
}

int GameEngine::getKillAmount() const {
    return static_cast<int>(getAllKillEvents().size());
}

int GameEngine::getWhiteKillAmount() const {
    int nbKill = 0;
    for (auto* event : getAllKillEvents()) {
        const auto kill_event = dynamic_cast<EventKill*>(event);
        if (kill_event == nullptr) continue;
        if (kill_event->killedPiece->getIsWhite()) nbKill++;
    }
    return nbKill;
}

int GameEngine::getBlackKillAmount() const {
    int nbKill = 0;
    for (auto* event : getAllKillEvents()) {
        const auto kill_event = dynamic_cast<EventKill*>(event);
        if (kill_event == nullptr) continue;
        if (!kill_event->killedPiece->getIsWhite()) nbKill++;
    }
    return nbKill;
}

std::vector<Event*> GameEngine::getAllKillEventsThisPhase() const {
    std::vector<Event*> selected_events;
    for (auto* event : current_phase_context->events) {
        if (event == nullptr) ltr_log_error("Pieces::getAllKillEventsThisPhase: Event nullptr");
        if (event->eventType == EVENT_KILL) selected_events.emplace_back(event);
    }
    return selected_events;
}

int GameEngine::getKillAmountThisPhase() const {
    return static_cast<int>(getAllKillEventsThisPhase().size());
}

int GameEngine::getWhiteKillAmountThisPhase() const {
    int nbKill = 0;
    for (auto* event : getAllKillEventsThisPhase()) {
        const auto kill_event = dynamic_cast<EventKill*>(event);
        if (kill_event == nullptr) continue;
        if (kill_event->killedPiece->getIsWhite()) nbKill++;
    }
    return nbKill;
}

int GameEngine::getBlackKillAmountThisPhase() const {
    int nbKill = 0;
    for (auto* event : getAllKillEventsThisPhase()) {
        const auto kill_event = dynamic_cast<EventKill*>(event);
        if (kill_event == nullptr) continue;
        if (!kill_event->killedPiece->getIsWhite()) nbKill++;
    }
    return nbKill;
}

std::vector<Event*> GameEngine::getAllKillEventsLastPhase() const {
    std::vector<Event*> selected_events;
    if (static_cast<int>(phase_contexts.size()) < 1) {
        ltr_log_error("Pieces::getAllKillEventsLastPhase: Phase context empty");
        return selected_events;
    }
    for (auto* event : phase_contexts.back()->events) {
        if (event == nullptr) ltr_log_error("Pieces::getAllKillEventsThisPhase: Event nullptr");
        if (event->eventType == EVENT_KILL) selected_events.emplace_back(event);
    }
    return selected_events;
}

int GameEngine::getKillAmountLastPhase() const {
    return static_cast<int>(getAllKillEventsThisPhase().size());
}

int GameEngine::getWhiteKillAmountLastPhase() const {
    int nbKill = 0;
    for (auto* event : getAllKillEventsLastPhase()) {
        const auto kill_event = dynamic_cast<EventKill*>(event);
        if (kill_event == nullptr) continue;
        if (kill_event->killedPiece->getIsWhite()) nbKill++;
    }
    return nbKill;
}

int GameEngine::getBlackKillAmountLastPhase() const {
    int nbKill = 0;
    for (auto* event : getAllKillEventsLastPhase()) {
        const auto kill_event = dynamic_cast<EventKill*>(event);
        if (kill_event == nullptr) continue;
        if (!kill_event->killedPiece->getIsWhite()) nbKill++;
    }
    return nbKill;
}






// Private Stuff

void GameEngine::goToState(const GameState state) {
    ltr_log_debug(CONSOLE_COLOR_GRAY, "Going from : ", game_state_to_string[current_state], ", to ", game_state_to_string[state], ", main last state : ", game_state_to_string[last_main_state]);
    last_state = current_state;
    if (current_state != SELECT_ANY)
        last_main_state = current_state;
    current_state = state;
}

void GameEngine::loadLastState() {
    ltr_log_debug(CONSOLE_COLOR_GRAY, "Loading last state : ", game_state_to_string[current_state], " to ", game_state_to_string[last_state]);
    const auto tmp = current_state;
    current_state = last_state;
    last_state = tmp;
}

void GameEngine::push_phase_context() {
    if (current_phase_context->firstSelectedPiece != nullptr)
        current_phase_context->firstSelectedPiece->selected = false;
    phase_contexts.emplace_back(current_phase_context);
    current_phase_context = new phase_context_type();
    current_phase_context->phaseNumber = static_cast<int>(phase_contexts.size());
    current_phase_context->turnNumber = NB_Turn;
}





// State Handlers

void GameEngine::handleInitialisation() {
    srand(time(nullptr));
    game_state_names_Init();
    init_board_patterns();
    init_moves();
    init_event_type();
    loadEffectList();
    loadCharactersList();
    loadPiecesList();
    init_pieces();
    ltr_log(
        "---------------------------------------------------WHITE TURN ",
        NB_Turn,
        "---------------------------------------------------"
    );
    goToState(START_WHITE_PHASE);
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
    if (receivedClick == false)
        return;
    receivedClick = false;
    chessboard_cell* selectedCell = Chessboard::getInstance()->getCellAt(lastClickX, lastClickY);
    if (
        ranges::find(required_selection.banned_cells,selectedCell) != required_selection.banned_cells.end()
        || (required_selection.effect_instance != nullptr && !required_selection.effect_instance->check_condition(selectedCell))
    ) return;
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
            selectedCell->piece->selected = false;
            current_selection->whites--;
            std::erase(
                current_selection->white_pieces,
                selectedCell
            );
            ltr_log_debug("Deselecting piece at ", selectedCell->pos.x, " ", selectedCell->pos.y);
            return;
        }
        selectedCell->selected = true;
        selectedCell->piece->selected = true;
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
        selectedCell->piece->selected = false;
        current_selection->blacks--;
        std::erase(
            current_selection->black_pieces,
            selectedCell
        );
        ltr_log_debug("Deselecting piece at ", selectedCell->pos.x, " ", selectedCell->pos.y);
        return;
    }
    selectedCell->selected = true;
    selectedCell->piece->selected = true;
    current_selection->blacks++;
    current_selection->black_pieces.emplace_back(selectedCell);
    ltr_log_debug("Selecting piece at ", selectedCell->pos.x, " ", selectedCell->pos.y);
}

void GameEngine::handleEndGame() {
    ltr_log_info(CONSOLE_COLOR_RED, "End game");
    goToState(GAME_CLOSE);
}

void GameEngine::handleGameClose() {
    ltr_log_info(CONSOLE_COLOR_RED, "Game closing");
}



void GameEngine::handleAnyStartPhase(const bool isWhitePhase) {
    const selection_type* selection = getSelection();
    if (selection == nullptr) {
        selection_request_type request;
        request.whites = isWhitePhase ? 1 : 0;
        request.blacks = isWhitePhase ? 0 : 1;
        request.instantValidation = true;
        requestSelection(request);
        return;
    }
    Pieces *selectedPiece = isWhitePhase ? selection->white_pieces[0]->piece : selection->black_pieces[0]->piece;
    current_phase_context->firstSelectedPiece = selectedPiece;
    goToState(isWhitePhase ? SELECT_WHITE_PHASE : SELECT_BLACK_PHASE);
}

void GameEngine::handleAnySelectPhase(const bool isWhitePhase) {
    if (current_phase_context->firstSelectedPiece == nullptr) {
        ltr_log_error("Impossible state in GameEngine::handle", isWhitePhase ? "White" : "Black", "SelectPhase()");
        return;
    }
    // Display moves
    unloadPossibleMoves();
    const vector<glm::ivec2> possible_moves = Chessboard::getInstance()->getValidMoves(current_phase_context->firstSelectedPiece);
    loadPossibleMoves(possible_moves, current_phase_context->firstSelectedPiece->getDefaultColor());

    if (!receivedClick && !receivedRightClick)
        return;
    receivedClick = false;
    // Game mode
    if (receivedRightClick) {
        receivedRightClick = false;
        if (current_phase_context->firstSelectedPiece->togglePieceGameMode())
            goToState(isWhitePhase ? SELECT_WHITE_PHASE : SELECT_BLACK_PHASE);
    }
    if (current_phase_context->firstSelectedPiece->getPieceGameMode() != 0){
        goToState(isWhitePhase ? CHECKING_WHITE_PHASE : CHECKING_BLACK_PHASE);
        return;
    }
    // Deselection
    if (
        current_phase_context->firstSelectedPiece->getCoordX() == lastClickX
        && current_phase_context->firstSelectedPiece->getCoordY() == lastClickY
        && !current_phase_context->firstSelectedPiece->getIsOnAMove()
    ) {
        current_phase_context->firstSelectedPiece->selected = false;
        current_phase_context->firstSelectedPiece = nullptr;
        unloadPossibleMoves();
        goToState(isWhitePhase ? START_WHITE_PHASE : START_BLACK_PHASE);
        return;
    }
    // Movement
    if (Chessboard::getInstance()->getPieceAt(lastClickX, lastClickY) != nullptr)
        current_phase_context->mainTargetPiece = Chessboard::getInstance()->getPieceAt(lastClickX, lastClickY);
    if (Chessboard::getInstance()->movePiece(current_phase_context->firstSelectedPiece, lastClickX, lastClickY))
        goToState(isWhitePhase ? MOVING_WHITE_PHASE : MOVING_BLACK_PHASE);
}

void GameEngine::handleAnyMovingPhase(const bool isWhitePhase) {
    current_phase_context->firstSelectedPiece->activateSpecialEffect();
    goToState(isWhitePhase ? CHECKING_WHITE_PHASE : CHECKING_BLACK_PHASE);
}

void GameEngine::handleAnyCheckingPhase(const bool isWhitePhase) {
    unloadPossibleMoves();
    if (last_main_state == (isWhitePhase ? SELECT_WHITE_PHASE : SELECT_BLACK_PHASE))
        receivedRightClick = false;
    if (!current_phase_context->firstSelectedPiece->SpellActivationCheck()) {
        last_main_state = current_state;
        receivedClick = false;
        return;
    }
    if (current_phase_context->firstSelectedPiece->getIsOnAMove()) {
        receivedRightClick = false;
        goToState(isWhitePhase ? SELECT_WHITE_PHASE : SELECT_BLACK_PHASE);
        return;
    }
    if (current_phase_context->firstSelectedPiece->getIsEvolved())
        ltr_log_info(
            current_phase_context->firstSelectedPiece->getIsWhite()? "White " : "Black ",
            current_phase_context->firstSelectedPiece->getName(),
            " has evolved!"
        );
    if (current_state == SELECT_ANY)
        return;
    goToState(isWhitePhase ? END_WHITE_PHASE : END_BLACK_PHASE);
}

void GameEngine::handleAnyEndPhase(const bool isWhitePhase) {
    receivedClick = false;
    if ((isWhitePhase ? black_king : white_king)->isHidden()) {
        ltr_log_info(CONSOLE_COLOR_RED, isWhitePhase ? "White" : "Black", " has win!");
        goToState(END_GAME);
        push_phase_context();
        return;
    }
    NB_Phase++;
    if (!isWhitePhase)
        NB_Turn++;
    for (const auto& piece : Chessboard::getInstance()->getAllPieces()) {
        piece->updateEffectStatus();
        if (piece->getCharacter() == ARCEUID)
            piece->canEvolve();
        if (piece->getCharacter() == NITOCRIS_ALTER){
            piece->passive();
            piece->canEvolve();
        }
        if (piece->getCharacter() == TAMAMO_NO_MAE)
            piece->canEvolve();
    }
    ltr_log(
        "---------------------------------------------------",
        isWhitePhase ? "BLACK" : "WHITE",
        " TURN ", NB_Turn,
        "---------------------------------------------------"
    );
    goToState(isWhitePhase ? START_BLACK_PHASE : START_WHITE_PHASE);
    push_phase_context();
}







