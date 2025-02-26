//
// Created by david on 24/01/2025.
//

#ifndef PIECES_H
#define PIECES_H

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
    int NB_TurnWithoutMoving{0}, CNTMove{0}, TurnStamp{0};
    bool isFirstMove{true}, isOnAMove{false}, hasRoqued{false};

    bool isWhite, evolved{false}, isAlive{true};
    int pieceGameMode{0};
    bool canActivateEffects{false}, hasJustKilled{false};
    vector<glm::ivec2> AllMovesDoneBefore; ///TODO delete

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
    void goToPosition(int x, int y, bool increaseCountMove);
    [[nodiscard]] int getCNTMove() const {return CNTMove;}
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
    [[nodiscard]] Pieces_List getPiecesOrigin() const {return pieces_origin;}
    [[nodiscard]] Characters_List getCharacter() const {return character;}
    [[nodiscard]] bool getIsWhite() const {return isWhite;}
    [[nodiscard]] string getName() const {return name;}

    [[nodiscard]] bool getHasJustKilled() const {return hasJustKilled;} ///TODO delete
    [[nodiscard]] bool getIsFirstMove() const {return isFirstMove;}
    [[nodiscard]] int getTurnStamp() const {return TurnStamp;}
    [[nodiscard]] bool getIsEvolved() const {return evolved;}
    [[nodiscard]] bool getIsOnAMove() const {return isOnAMove;}
    [[nodiscard]] int getNB_TurnWithoutMoving() const {return NB_TurnWithoutMoving;}
    [[nodiscard]] bool getHasRoqued() const {return hasRoqued;}

    void setHasRoqued(const bool hasRoqued) {if (!this->hasRoqued) this->hasRoqued = hasRoqued;}
    void incrementNB_TurnWithoutMoving() {NB_TurnWithoutMoving++;}
    void incrementTurnStamp() {TurnStamp++;}
    void resetNB_TurnWithoutMoving() {NB_TurnWithoutMoving = 0;}
    void setIsOnAMove(const bool isOnAMove) {this->isOnAMove = isOnAMove;}
    void setIsWhite(const bool isWhite) {this->isWhite = isWhite;}
    void setHasJustKilled(const bool hasJustKilled) {this->hasJustKilled = hasJustKilled;}

    // Effects
    [[nodiscard]] int getPieceGameMode() const {return pieceGameMode;}
    virtual void setPieceGameMode(const int pieceGameMode) {this->pieceGameMode = pieceGameMode;}
    void displayEffects() const;
    [[nodiscard]] virtual vector<glm::ivec2> getEffectRange(Effect_List effect) const = 0;
    [[nodiscard]] bool hasThisEffect(Effect_List chosenEffect) const;
    [[nodiscard]] vector<EffectInstance*> getActive_effects() const {return activeEffects;}
    virtual bool passive(void* context) {return true;}
    virtual bool canEvolve(void* context) {return true;}
    virtual bool evolvedForm(void* context) {return true;}
    virtual bool SpellActivationCheck(void* context) {return true;}

    void addEffectStatus(const EffectInstance* effect_instance) {activeEffects.emplace_back(new EffectInstance(*effect_instance));}
    void updateEffectStatus();
    void deleteEffect(Effect_List effect);
    void activateEffect(Effect_List);

    // Event
    void addEvent(void* event);

    // SpriteTarget
    float getSpriteX() override {return (-0.875f + 0.25f * static_cast<float>(coordY)) * RenderEngine::getWindowInverseAspectRatio();}
    float getSpriteY() override {return 0.875f - 0.25f * static_cast<float>(coordX);}
    float getSpriteRotation() override {return (isWhite) ? 0.0f : 180.0f;}
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
