//
// Created by sebas on 29/12/2024.
//

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <Context.h>

#include "Engine.h"


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
};

struct selection_type {
    std::vector<Pieces*> white_pieces;
    std::vector<Pieces*> black_pieces;
    std::vector<glm::ivec2> empty_pieces;
};

struct selection_request_type {
    int whites;
    int blacks;
    int emptys;
};



class GameEngine final : public Engine {
public:
    GameEngine();
    ~GameEngine() override;
    static GameEngine* getInstance();
    void update(double deltaTime_ms) override;
    [[nodiscard]] GameState getCurrentState() const;
    void clickBoardAtPos(int x, int y);
    void inputRightClick();
    int NB_Turn = 1;
    int NB_WhiteDead = 0;
    int NB_BlackDead = 0;
    int NB_WhiteDeadLastPhase = 0;
    int NB_BlackDeadLastPhase = 0;
    void setBlackKing(Pieces* piece) {black_king = piece;}
    void setWhiteKing(Pieces* piece) {white_king = piece;}
    [[nodiscard]] int getLastClickX() const;
    [[nodiscard]] int getLastClickY() const;
    [[nodiscard]] GameState getLastState() const;
    [[nodiscard]] Pieces* getLastPieceTouchedByEffect() const;

    void setLastPieceTouchedByEffect(Pieces* last_piece_touched_by_effect);
    void setState(GameState state);
    void setLastState(GameState state);
    int lastClickX{-1}, lastClickY{-1};
    bool receivedClick = false;
    bool receivedRightClick = false;
    Pieces* lastPieceTouchedByEffect{nullptr};

    [[nodiscard]] int getTurnNumber() const {return NB_Turn;}

    void requestSelection(selection_request_type to_select);
    [[nodiscard]] selection_type* getSelection() const {return current_selection;}

    void addEvent(Event* event);
private:
    GameState current_state;
    GameState last_state;

    Pieces* black_king{nullptr};
    Pieces* white_king{nullptr};

    selection_type* current_selection;
    selection_request_type required_selection;
    phase_context_type current_phase_context;
    std::vector<phase_context_type> phase_contexts;


    inline static GameEngine* instance = nullptr;
    context_type* context = new context_type();
    unordered_map<GameState, function<void()>> state_handlers;

    void saveLastState() {last_state = current_state;};

    void handleInitialisation();

    void handleStartWhitePhase();
    void handleSelectWhitePhase();
    void handleMovingWhitePhase();
    void handleCheckingWhitePhase();
    void handleEndWhitePhase();

    void handleStartBlackPhase();
    void handleSelectBlackPhase();
    void handleMovingBlackPhase();
    void handleCheckingBlackPhase();
    void handleEndBlackPhase();

    void handleSelectAny();

    void handleEndGame();
    void handleGameClose();
};



#endif //GAMEENGINE_H
