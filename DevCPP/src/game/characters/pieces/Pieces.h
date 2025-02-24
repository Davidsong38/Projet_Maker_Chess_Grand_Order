//
// Created by david on 24/01/2025.
//

#ifndef PIECES_H
#define PIECES_H
#include <functional>
#include <string>
#include <vector>
#include <SpriteTarget.h>

#include "Pieces_List.h"
#include "Characters_List.h"
#include "Effect_List.h"



using namespace std;


class Pieces : public SpriteTarget{
    protected:
        int coordX, coordY;
        bool isWhite = false;
        bool evolved = false;
        bool isFirstMove = true;
        bool isAlive = true;
        bool firstMoveLastTurn = false;
        bool canActivateEffects = false;
        bool hasJustKilled = false;
        bool isOnAMove = false;
        bool hasRoqued = false;
        int pieceGameMode = 0;
        int movesMode = 0;
        vector<glm::ivec2> AllMovesDoneBefore;

        vector<EffectInstance> activeEffects ;
        Characters_List characters;
        Pieces_List pieces_origin;
        string name;
        function<vector<glm::ivec2>()> overrideMoves = nullptr;

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

        void gotUnalivedBy(Pieces* killer, int killType);
        void gotResurrectedAt(Pieces* caster, glm::ivec2 pos);

        [[nodiscard]] bool hasThisEffect(Effect_List chosenEffect) const;
        [[nodiscard]] bool getIsFirstMove() const;
        [[nodiscard]] bool getFirstMoveLastTurn() const;
        [[nodiscard]] int getTurnStamp() const;
        [[nodiscard]] bool getCanActivateEffects() const;
        [[nodiscard]] bool getHasJustKilled() const;
        [[nodiscard]] int getPieceGameMode() const;
        [[nodiscard]] bool getIsEvolved() const;
        [[nodiscard]] bool getIsOnAMove() const;
        [[nodiscard]] int getNB_TurnWithoutMoving() const;
        [[nodiscard]] bool getHasRoqued() const;
        [[nodiscard]] vector<glm::ivec2> getAllMovesDoneBefore() const;
        [[nodiscard]] int getMovesMode() const;
        [[nodiscard]] function<vector<glm::ivec2>()> getOverrideMoves() const;

        void setMovesMode(int moves_mode);
        void clearOverrideMoves();
        void setOverrideMoves(const function<vector<glm::ivec2>()>& override_moves);
        void addToAllMovesDoneBefore(int lastCoordX, int lastCoordY);
        void setHasRoqued(bool has_roqued);
        void setNB_TurnWithoutMoving(int nb_turn_without_moving);
        void setIsOnAMove(bool is_on_a_move);
        virtual void setPieceGameMode(int piece_game_mode) = 0;
        void setIsWhite(bool is_white);
        void setHasJustKilled(bool has_just_killed);
        void setCanActivateEffects(bool can_activate_effects);
        void setTurnStamp(int turn_stamp);
        void setFirstMoveLastTurn(bool first_move_last_turn);

        void setPiecesOrigin(Pieces_List pieces_origin);

        [[nodiscard]] string getName();
        [[nodiscard]] int getCoordX() const;
        [[nodiscard]] int getCoordY() const;
        void setPosition(int newX,int newY);

        void addEffectStatus (EffectInstance effect_instance);
        [[nodiscard]] bool hasEffectStatus (Effect_List effect) const;
        void updateEffectStatus ();
        void deleteEffect(Effect_List effect);
        void activateEffect(Effect_List);
        void displayEffect();

        [[nodiscard]] bool getIsWhite() const;
        [[nodiscard]] vector<EffectInstance> getActive_effects() const;
        [[nodiscard]] Characters_List getCharacters() const;
        [[nodiscard]] Pieces_List getPiecesOrigin() const;

        [[nodiscard]] bool isPawn() const {return pieces_origin == PAWN;}
        [[nodiscard]] bool isKnight() const {return pieces_origin == KNIGHT;}
        [[nodiscard]] bool isBishop() const {return pieces_origin == BISHOP;}
        [[nodiscard]] bool isRook() const {return pieces_origin == ROOK;}
        [[nodiscard]] bool isQueen() const {return pieces_origin == QUEEN;}
        [[nodiscard]] bool isKing() const {return pieces_origin == KING;}

        [[nodiscard]] int getCNTMove() const;

        [[nodiscard]] virtual vector<glm::ivec2> getMoves() = 0;
        [[nodiscard]] virtual vector<glm::ivec2> getEffectRange(Effect_List effect) const = 0;

        [[nodiscard]] virtual bool isCheating() const {return false;}
        virtual bool passive(void* context) = 0;
        virtual bool canEvolve(void* context) = 0;
        virtual bool evolvedForm(void* context) = 0;
        virtual bool SpellActivationCheck(void* context) = 0;

        void goToPosition(int x, int y);
};



#endif //PIECES_H
