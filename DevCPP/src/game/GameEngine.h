//
// Created by sebas on 29/12/2024.
//

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Engine.h"
#include "selection.h"
#include "Event.h"
#include "phase_context.h"


enum GameState{
    INITIALISATION,

    START_WHITE_PHASE,
    SELECT_WHITE_PHASE,
    MOVING_WHITE_PHASE,
    CHECKING_WHITE_PHASE,
    END_WHITE_PHASE,

    START_BLACK_PHASE,
    SELECT_BLACK_PHASE,
    MOVING_BLACK_PHASE,
    CHECKING_BLACK_PHASE,
    END_BLACK_PHASE,

    SELECT_ANY,

    END_GAME,
    GAME_CLOSE,

    GAME_STATE_COUNT
};

extern std::string game_state_to_string[GAME_STATE_COUNT];

void game_state_names_Init();



class GameEngine final : public Engine {
public:
    GameEngine();
    ~GameEngine() override;
    static GameEngine* getInstance();
    void update(double deltaTime_ms) override;

    [[nodiscard]] GameState getCurrentState() const {return current_state;}
    [[nodiscard]] GameState getLastState() const {return last_state;}

    [[nodiscard]] int getTurnNumber() const {return NB_Turn;}
    [[nodiscard]] int getPhaseNumber() const {return NB_Phase;}
    void setBlackKing(Pieces* piece) {black_king = piece;}
    void setWhiteKing(Pieces* piece) {white_king = piece;}

    [[nodiscard]] phase_context_type* getCurrentPhaseContext() const {return current_phase_context;}

    // Selection
    void requestSelection(const selection_request_type& to_select);
    void validateSelection();
    static void deselectAllPieces();
    [[nodiscard]] selection_type* getSelection();

    // Mouse inputs
    void clickBoardAtPos(int x, int y);
    void inputRightClick() {receivedRightClick = true;}
    int lastClickX{-1}, lastClickY{-1};
    bool ghostClick = false;

    // Events
    void registerEvent(Event* event) const;
    [[nodiscard]] std::vector<Event*> getAllKillEvents() const;
    [[nodiscard]] int getKillAmount() const;
    [[nodiscard]] int getWhiteKillAmount() const;
    [[nodiscard]] int getBlackKillAmount() const;
    [[nodiscard]] std::vector<Event*> getAllKillEventsThisPhase() const;
    [[nodiscard]] int getKillAmountThisPhase() const;
    [[nodiscard]] int getWhiteKillAmountThisPhase() const;
    [[nodiscard]] int getBlackKillAmountThisPhase() const;
    [[nodiscard]] std::vector<Event*> getAllKillEventsLastPhase() const;
    [[nodiscard]] int getKillAmountLastPhase() const;
    [[nodiscard]] int getWhiteKillAmountLastPhase() const;
    [[nodiscard]] int getBlackKillAmountLastPhase() const;
private:
    inline static GameEngine* instance = nullptr;
    unordered_map<GameState, function<void()>> state_handlers;

    GameState current_state{};
    GameState last_state{};
    GameState last_main_state{};

    void goToState(GameState state);
    void loadLastState();
    void push_phase_context();
    
    int NB_Turn{1}, NB_Phase{1};
    Pieces* black_king{nullptr},* white_king{nullptr};

    phase_context_type* current_phase_context = new phase_context_type();
    std::vector<phase_context_type*> phase_contexts;

    // Mouse inputs
    bool receivedClick = false;
    bool receivedRightClick = false;

    // Selection
    selection_type* current_selection{nullptr};
    selection_request_type required_selection{};
    bool selectionGotValidated{false};

    // States
    void handleInitialisation();
    void handleSelectAny();
    void handleEndGame();
    static void handleGameClose();

    void handleAnyStartPhase(bool isWhitePhase);
    void handleAnySelectPhase(bool isWhitePhase);
    void handleAnyMovingPhase(bool isWhitePhase);
    void handleAnyCheckingPhase(bool isWhitePhase);
    void handleAnyEndPhase(bool isWhitePhase);
};



#endif //GAMEENGINE_H
