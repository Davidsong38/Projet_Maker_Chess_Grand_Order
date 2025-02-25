//
// Created by david on 24/01/2025.
//

#ifndef PIECES_H
#define PIECES_H

#include <functional>
#include <piece_moves.h>
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
    bool isWhite = false;
    bool evolved = false;
    bool isFirstMove = true;
    bool isAlive = true;
    bool canActivateEffects = false;
    bool hasJustKilled = false;
    bool isOnAMove = false;
    bool hasRoqued = false;
    int pieceGameMode = 0;
    int movesMode = 0;
    vector<glm::ivec2> AllMovesDoneBefore;

    vector<EffectInstance*> activeEffects ;
    Characters_List characters;
    Pieces_List pieces_origin;
    string name;
    function<vector<glm::ivec2>()> overrideMoves = nullptr;
    piece_move* default_piece_move = shinji_moves;
    piece_move* override_piece_move = nullptr;

    std::vector<void*> events;
public:
    bool selected = false;
    int CNTMove = 0;
    int TurnStamp = 0;
    int NB_TurnWithoutMoving = 0;

    explicit Pieces(const int startX, const int startY, const bool white, const Characters_List hero, const Pieces_List pieces_root)
    : coordX(startX), coordY(startY),isWhite(white) , characters(hero) , pieces_origin(pieces_root), name(Characters_List_to_string[characters]) {}

    ~Pieces() override = default;;

    float getSpriteX() override;
    float getSpriteY() override;
    float getSpriteRotation() override;
    glm::vec3 getFilterColor() override;
    glm::vec4 getDefaultColor() override;
    bool isHidden() override;

    [[nodiscard]] piece_move* getCurrentPieceMove() const;

    void gotUnalivedBy(Pieces* killer, int killType);
    void gotResurrectedAt(const Pieces* caster, glm::ivec2 pos);

    [[nodiscard]] bool hasThisEffect(Effect_List chosenEffect) const;
    [[nodiscard]] bool getIsFirstMove() const;
    [[nodiscard]] int getTurnStamp() const;
    [[nodiscard]] bool getCanActivateEffects() const;
    [[nodiscard]] bool getHasJustKilled() const;
    [[nodiscard]] int getPieceGameMode() const;
    [[nodiscard]] bool getIsEvolved() const;
    [[nodiscard]] bool getIsOnAMove() const;
    [[nodiscard]] int getNB_TurnWithoutMoving() const;
    [[nodiscard]] bool getHasRoqued() const;
    [[nodiscard]] vector<glm::ivec2> getAllMovesDoneBefore() const;

    void addToAllMovesDoneBefore(int lastCoordX, int lastCoordY);
    void setHasRoqued(bool has_roqued);
    void setNB_TurnWithoutMoving(int nb_turn_without_moving);
    void setIsOnAMove(bool is_on_a_move);
    virtual void setPieceGameMode(int piece_game_mode) = 0;
    void setIsWhite(bool is_white);
    void setHasJustKilled(bool has_just_killed);
    void setCanActivateEffects(bool can_activate_effects);
    void setTurnStamp(int turn_stamp);

    void setPiecesOrigin(Pieces_List pieces_origin);
    [[nodiscard]] Pieces_List getPiecesOrigin() const;
    [[nodiscard]] Characters_List getCharacters() const;

    [[nodiscard]] bool getIsWhite() const;
    [[nodiscard]] string getName() const;
    [[nodiscard]] int getCoordX() const;
    [[nodiscard]] int getCoordY() const;

    void addEffectStatus(EffectInstance* effect_instance);
    [[nodiscard]] bool hasEffectStatus (Effect_List effect) const;
    void updateEffectStatus();
    void deleteEffect(Effect_List effect);
    void activateEffect(Effect_List);
    void displayEffect();
    [[nodiscard]] vector<EffectInstance*> getActive_effects() const;

    [[nodiscard]] bool isPawn() const {return pieces_origin == PAWN;}
    [[nodiscard]] bool isKnight() const {return pieces_origin == KNIGHT;}
    [[nodiscard]] bool isBishop() const {return pieces_origin == BISHOP;}
    [[nodiscard]] bool isRook() const {return pieces_origin == ROOK;}
    [[nodiscard]] bool isQueen() const {return pieces_origin == QUEEN;}
    [[nodiscard]] bool isKing() const {return pieces_origin == KING;}

    [[nodiscard]] int getCNTMove() const;

    [[nodiscard]] virtual vector<glm::ivec2> getEffectRange(Effect_List effect) const = 0;

    [[nodiscard]] virtual bool isCheating() const {return false;}
    virtual bool passive(void* context) = 0;
    virtual bool canEvolve(void* context) = 0;
    virtual bool evolvedForm(void* context) = 0;
    virtual bool SpellActivationCheck(void* context) = 0;

    void goToPosition(int x, int y);
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
