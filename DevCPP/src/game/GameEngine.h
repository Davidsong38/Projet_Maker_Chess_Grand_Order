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
    SELECT_PIECE_GAMEMODE_WHITE_PHASE,
    SELECT_WHITE_PHASE,
    MOVING_WHITE_PHASE,
    CHECKING_WHITE_PHASE,
    END_WHITE_PHASE,
    START_BLACK_PHASE,
    SELECT_PIECE_GAMEMODE_BLACK_PHASE,
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
    [[nodiscard]] GameState getCurrentState() const;
    void clickBoardAtPos(int x, int y);
    void inputRightClick();
    int NB_Turn = 1;
    int NB_WhiteDead = 0;
    int NB_BlackDead = 0;
    int NB_WhiteDeadLastPhase = 0;
    int NB_BlackDeadLastPhase = 0;
    void setBlackKing(Pieces* piece) const;
    void setWhiteKing(Pieces* piece) const;
    [[nodiscard]] int getLastClickX() const;
    [[nodiscard]] int getLastClickY() const;
    [[nodiscard]] GameState getLastState() const;
    [[nodiscard]] Pieces* getLastPieceTouchedByEffect() const;

    void setLastPieceTouchedByEffect(Pieces* last_piece_touched_by_effect);
    void setState(GameState state);
    void setLastState(GameState state);
    GameState current_state;
    GameState last_state;
    int lastClickX{-1}, lastClickY{-1};
    bool receivedClick = false;
    bool receivedRightClick = false;
    Pieces* lastPieceTouchedByEffect;

private:



    inline static GameEngine* instance = nullptr;
    context_type* context = new context_type();
    unordered_map<GameState, function<void()>> state_handlers;


    void handleInitialisation();
    void handleStartWhitePhase();
    void handleSelectPieceGamemodeWhitePhase();
    void handleSelectWhitePhase();
    void handleMovingWhitePhase();
    void handleCheckingWhitePhase();
    void handleEndWhitePhase();
    void handleStartBlackPhase();
    void handleSelectPieceGamemodeBlackPhase();
    void handleSelectBlackPhase();
    void handleMovingBlackPhase();
    void handleCheckingBlackPhase();
    void handleEndBlackPhase();
    void handleEndGame();
    static void handleGameClose();

    //void CheckEffect();
    //void CheckKill();


};



#endif //GAMEENGINE_H
