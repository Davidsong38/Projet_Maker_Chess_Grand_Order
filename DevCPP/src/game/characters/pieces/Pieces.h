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
#include <vector>
#include <SpriteTarget.h>

#include "Pieces_List.h"
#include "Characters_List.h"
#include "effects.h"



using namespace std;


class Pieces : public SpriteTarget{
protected:
    int coordX, coordY;
    bool isFirstMove{true}, isOnAMove{false}, hasRoqued{false};

    bool isWhite, evolved{false}, isAlive{true};
    int pieceGameMode{0};

    Characters_List character;
    Pieces_List pieces_origin;
    string name;

    piece_move* default_piece_move = shinji_moves;
    piece_move* override_piece_move = nullptr;

    vector<EffectInstance*> activeEffects ;
    std::vector<void*> events;
public:
    bool selected = false;

    explicit Pieces(const int startX, const int startY, const bool white, const Characters_List hero, const Pieces_List pieces_root)
    : coordX(startX), coordY(startY),isWhite(white) , character(hero) , pieces_origin(pieces_root), name(Characters_List_to_string[character]) {}

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
    virtual void setPieceGameMode(const int pieceGameMode) {this->pieceGameMode = pieceGameMode;}
    void displayEffects() const;
    [[nodiscard]] virtual vector<glm::ivec2> getEffectRange(Effect_List effect) {return square_pattern->get_positions(glm::ivec2(coordX,coordY));}
    [[nodiscard]] bool hasThisEffect(Effect_List chosenEffect) const;
    [[nodiscard]] vector<EffectInstance*> getActive_effects() const {return activeEffects;}
    virtual bool passive(void* context) {return false;}
    virtual bool canEvolve(void* context) {return false;}
    virtual bool evolvedForm(void* context) {return false;}
    virtual bool SpellActivationCheck(void* context) {return true;}

    void addEffectStatus(const EffectInstance* effect_instance) {activeEffects.emplace_back(new EffectInstance(*effect_instance));}
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
    [[nodiscard]] void* getFirstNormalMoveEvent();
    [[nodiscard]] int getFirstNormalMovePhase();
    [[nodiscard]] std::vector<void*> getAllKillEvents();
    [[nodiscard]] void* getLastKillKillEvent();
    [[nodiscard]] int getLastKillTurn();
    [[nodiscard]] void* getLastDeathKillEvent();
    [[nodiscard]] void* getLastKillKillEvent(int killType);
    [[nodiscard]] void* getLastDeathKillEvent(int killType);
    [[nodiscard]] std::vector<void*> getAllSpellUsedEvents();
    [[nodiscard]] void* getLastSpellUsedEvent() {return getAllSpellUsedEvents().back();}
    [[nodiscard]] void* getLastSpellUsedByMeEvent();

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
