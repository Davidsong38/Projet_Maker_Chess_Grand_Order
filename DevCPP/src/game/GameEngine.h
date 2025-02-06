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
    END_GAME,
    GAME_CLOSE,
};



class GameEngine final : public Engine {
public:
    GameEngine();
    ~GameEngine() override;
    static GameEngine* getInstance();
    void update(double deltaTime_ms) override;
    void run(void * arg);
    [[nodiscard]] GameState get_current_state() const;
    void clickBoardAtPos(int x, int y);
private:
    int lastClickX{-1}, lastClickY{-1};
    bool receivedClick = false;
    int NB_Turn = 0;
    GameState current_state;
    inline static GameEngine* instance = nullptr;
    context_type* context = new context_type();
    unordered_map<GameState, function<void()>> state_handlers;

    void setState(GameState state);
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
    void handleEndGame();
    static void handleGameClose();

    void CheckEffect();
    void CheckKill();


};



#endif //GAMEENGINE_H
