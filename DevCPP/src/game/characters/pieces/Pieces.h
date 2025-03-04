//
// Created by david on 24/01/2025.
//

#ifndef PIECES_H
#define PIECES_H

#include <board_pattern.h>
#include <log.h>
#include <piece_moves.h>
#include <RenderEngine.h>
#include <string>
#include <utility>
#include <vector>
#include <SpriteTarget.h>

#include "Pieces_List.h"
#include "Characters_List.h"
#include "effects.h"



using namespace std;


class Pieces : public SpriteTarget{
protected:
    int coordX, coordY;
    bool isFirstMove{true}, isOnAMove{false};

    bool isWhite, evolved{false}, isAlive{true};
    int pieceGameMode{0};

    Characters_List character;
    Pieces_List pieces_origin;
    string name;

    piece_move* default_piece_move = shinji_moves;
    piece_move* override_piece_move = nullptr;

    unordered_map<Effect_List, function<board_pattern*()>> defaultEffectsRanges;


    vector<EffectInstance*> activeEffects ;
    std::vector<void*> events;
    std::function<board_pattern*()> getDefaultEffectsRanges = [this](){ return checker_1_pattern; };
public:
    bool selected = false;

    explicit Pieces(const int startX, const int startY, const bool white, const Characters_List hero, const Pieces_List pieces_root)
    : coordX(startX), coordY(startY),isWhite(white) , character(hero) , pieces_origin(pieces_root), name(Characters_List_to_string[character]) {
        defaultEffectsRanges[STUN]                      = getDefaultEffectsRanges;
        defaultEffectsRanges[CHANGE_CONTROL]            = getDefaultEffectsRanges;
        defaultEffectsRanges[CHANGE_CONTROL_ADVANCE]    = getDefaultEffectsRanges;
        defaultEffectsRanges[AOE]                       = getDefaultEffectsRanges;
        defaultEffectsRanges[GIVING_AOE]                = getDefaultEffectsRanges;
        defaultEffectsRanges[SHIELD]                    = getDefaultEffectsRanges;
        defaultEffectsRanges[IMMUNITY_EFFECT]           = getDefaultEffectsRanges;
        defaultEffectsRanges[IMMUNITY_AOE]              = getDefaultEffectsRanges;
        defaultEffectsRanges[SPAWN_PIECES]              = getDefaultEffectsRanges;
        defaultEffectsRanges[ONE_MORE_MOVE]             = getDefaultEffectsRanges;
        defaultEffectsRanges[SUPP_MOVE]                 = getDefaultEffectsRanges;
        defaultEffectsRanges[ALTERNATE_RANGE]           = getDefaultEffectsRanges;
        defaultEffectsRanges[SUPP_RANGE]                = getDefaultEffectsRanges;
        defaultEffectsRanges[MOVE_CHANGING]             = getDefaultEffectsRanges;
        defaultEffectsRanges[SWITCHING_PLACE]           = getDefaultEffectsRanges;
        defaultEffectsRanges[IMMORTALITY]               = getDefaultEffectsRanges;
        defaultEffectsRanges[SACRIFICE]                 = getDefaultEffectsRanges;
        defaultEffectsRanges[KILLING]                   = getDefaultEffectsRanges;
        defaultEffectsRanges[ALLY_TELEPORT]             = getDefaultEffectsRanges;
        defaultEffectsRanges[ENEMY_TELEPORT]            = getDefaultEffectsRanges;
        defaultEffectsRanges[EVOLVE]                    = getDefaultEffectsRanges;
        defaultEffectsRanges[SUPP_LUCK]                 = getDefaultEffectsRanges;
    }

    [[nodiscard]] unordered_map<Effect_List, function<board_pattern *()>> getDefaultEffectsRangesPattern() const {return defaultEffectsRanges;}
    void setEffectRange(Effect_List effect, std::function<board_pattern*()> newRange) {defaultEffectsRanges[effect] = std::move(newRange);}
    ~Pieces() override = default;;

    [[nodiscard]] piece_move* getCurrentPieceMove() const;
    void addOverrideMove(piece_move* new_piece_move) {override_piece_move = new_piece_move;}
    void goToPosition(int x, int y);
    void goToPosition(int x, int y, int moveType);
    [[nodiscard]] int getCoordX() const {return coordX;}
    [[nodiscard]] int getCoordY() const {return coordY;}

    void gotUnalivedBy(Pieces* killer, int killType);
    void gotResurrectedAt(const Pieces* caster, glm::ivec2 pos);

    [[nodiscard]] bool isPawn() const {return pieces_origin == PAWN;}
    [[nodiscard]] bool isKnight() const {return pieces_origin == KNIGHT;}
    [[nodiscard]] bool isBishop() const {return pieces_origin == BISHOP;}
    [[nodiscard]] bool isRook() const {return pieces_origin == ROOK;}
    [[nodiscard]] bool isQueen() const {return pieces_origin == QUEEN;}
    [[nodiscard]] bool isKing() const {return pieces_origin == KING;}

    void setPiecesOrigin(Pieces_List pieces_origin);

    void activateSpecialEffect();

    [[nodiscard]] int getPiecesOrigin() const {return pieces_origin;}
    [[nodiscard]] Characters_List getCharacter() const {return character;}
    [[nodiscard]] bool getIsWhite() const {return isWhite;}
    [[nodiscard]] string getName() const {return name;}

    [[nodiscard]] bool getIsFirstMove() const {return isFirstMove;}
    [[nodiscard]] bool getIsEvolved() const {return evolved;}
    [[nodiscard]] bool getIsOnAMove() const {return isOnAMove;}

    void setIsOnAMove(const bool isOnAMove) {this->isOnAMove = isOnAMove;}
    void setIsWhite(const bool isWhite) {this->isWhite = isWhite;}

    // Effects
    [[nodiscard]] int getPieceGameMode() const {return pieceGameMode;}
    virtual bool togglePieceGameMode() {return false;}
    void displayEffects() const;
    [[nodiscard]] virtual board_pattern *getEffectRange(Effect_List effect) {return square_pattern;}
    [[nodiscard]] bool hasThisEffect(Effect_List chosenEffect) const;
    [[nodiscard]] vector<EffectInstance*> getActive_effects() const {return activeEffects;}
    virtual bool passive() {return false;}
    virtual bool canEvolve() {return false;}
    virtual bool evolvedForm() {return false;}
    virtual bool SpellActivationCheck() {return true;}

    void addEffectStatus(const EffectInstance* effect_instance) {deleteEffect(effect_instance->effect); activeEffects.emplace_back(new EffectInstance(*effect_instance));}
    void updateEffectStatus();
    void deleteEffect(Effect_List effect);
    void activateEffect(Effect_List);

    // Event
    void addEvent(void* event);
    [[nodiscard]] std::vector<void*> getAllEvents() const {return events;}
    [[nodiscard]] std::vector<void*> getAllMoveEvents();
    [[nodiscard]] void* getLastMoveEvent() {return getAllMoveEvents().back();}
    [[nodiscard]] void* getLastNormalMoveEvent();
    [[nodiscard]] int getLastNormalMovePhase();
    [[nodiscard]] int getLastNormalMoveEventType();
    [[nodiscard]] void* getSecondLastNormalMoveEvent();
    [[nodiscard]] int getSecondLastNormalMovePhase();
    [[nodiscard]] int getSecondLastNormalMoveEventType();
    [[nodiscard]] void* getFirstNormalMoveEvent();
    [[nodiscard]] int getFirstNormalMovePhase();
    [[nodiscard]] std::vector<void*> getAllKillEvents();
    [[nodiscard]] void* getLastKillKillEvent();
    [[nodiscard]]void* getSecondLastKillKillEvent();
    [[nodiscard]] int getLastKillTurn();
    [[nodiscard]] bool gotKillAtTurn(int turn);
    [[nodiscard]] void* getLastDeathKillEvent();
    [[nodiscard]] void* getSecondLastDeathKillEvent();
    [[nodiscard]] void* getLastKillKillEvent(int killType);
    [[nodiscard]] void* getLastDeathKillEvent(int killType);
    [[nodiscard]] std::vector<void*> getAllSpellUsedEvents();
    [[nodiscard]] void* getLastSpellUsedEvent() {return getAllSpellUsedEvents().back();}
    [[nodiscard]] void* getLastSpellUsedByMeEvent();

    std::vector<void *> getAllEffectAppliedEvents();

    std::vector<void *> getAllEffectAppliedCastedByMeEvent();

    [[nodiscard]] std::vector<void*> getAllEffectUpdateEvents();
    [[nodiscard]] std::vector<void*> getAllEffectUpdateCastedByMeEvent();

    std::vector<void *> getAllDeathWithEffectCastedByMe();

    // SpriteTarget
    float getSpriteX() override {return (-0.875f + 0.25f * static_cast<float>(coordY)) * RenderEngine::getWindowInverseAspectRatio();}
    float getSpriteY() override {return 0.875f - 0.25f * static_cast<float>(coordX);}
    float getSpriteRotation() override {return isWhite ? 0.0f : 180.0f;}
    glm::vec3 getFilterColor() override;
    glm::vec4 getDefaultColor() override;
    bool isHidden() override {return !isAlive;}
};

class Bishop : public Pieces{
public:
    Bishop(const int startX, const int startY, const bool white, const Characters_List hero)
        : Pieces(startX, startY, white, hero, BISHOP) {
        this->default_piece_move = bishop_default_moves;
    }
};

class King : public Pieces{
public:
    King(const int startX, const int startY, const bool white, const Characters_List hero)
        : Pieces(startX, startY, white, hero, KING) {
        this->default_piece_move = king_default_moves;
    }
};

class Knight : public Pieces{
public:
    Knight(const int startX, const int startY, const bool white, const Characters_List hero)
        : Pieces(startX, startY, white, hero, KNIGHT) {
        this->default_piece_move = knight_default_moves;
    }
};

class Pawn : public Pieces{
public:
    Pawn(const int startX, const int startY, const bool white, const Characters_List hero)
        : Pieces(startX, startY, white, hero, PAWN) {
        this->default_piece_move = pawn_default_moves;
    }
};

class Queen : public Pieces{
public:
    Queen(const int startX, const int startY, const bool white, const Characters_List hero)
        : Pieces(startX, startY, white, hero, QUEEN) {
        this->default_piece_move = queen_default_moves;
    }
};

class Rook : public Pieces{
public:
    Rook(const int startX, const int startY, const bool white, const Characters_List hero)
        : Pieces(startX, startY, white, hero, ROOK) {
        this->default_piece_move = rook_default_moves;
    }
};

#endif //PIECES_H
