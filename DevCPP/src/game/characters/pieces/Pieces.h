//
// Created by david on 24/01/2025.
//

#ifndef PIECES_H
#define PIECES_H
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <SpriteTarget.h>

#include "Pieces_List.h"
#include "Characters_List.h"
#include "Effect_List.h"



using namespace std;


class Pieces : public SpriteTarget {
    protected:
        int coordX;
        int coordY;
        bool isWhite = false;
        bool evolved = false;
        vector<EffectInstance> activeEffects ;
        //vector<Character_Instance> characters ;
        Characters_List characters;
        Pieces_List pieces_origin;
        string name;

    public:


        explicit Pieces(int startX, int startY, bool white, Characters_List hero, Pieces_List pieces_root)
        : coordX(startX), coordY(startY),isWhite(white) , characters(hero) , pieces_origin(pieces_root), name(Characters_List_to_string[characters]) {}

        //explicit Pieces(string name);

        ~Pieces() override = default;;

        float getSpriteX() override;
        float getSpriteY() override;

        [[nodiscard]] string getName();
        [[nodiscard]] int getCoordX() const;
        [[nodiscard]] int getCoordY() const;
        void setPosition(int newX,int newY);

        void addEffectStatus (EffectInstance effect_instance);
        //void affectCharacter(const Character_Instance& character_instance);
        [[nodiscard]] bool hasEffectStatus (Effect_List effect) const;
        void updateEffectStatus ();
        void activateEffect(Effect_List);
        void displayEffect();

        [[nodiscard]] bool getIsWhite() const;
        [[nodiscard]] vector<EffectInstance> getActive_effects() const;
        [[nodiscard]] Characters_List getCharacters() const;
        [[nodiscard]] Pieces_List getPiecesOrigin() const;

        [[nodiscard]] bool isPawn() const;
        [[nodiscard]] bool isKnight() const;
        [[nodiscard]] bool isBishop() const;
        [[nodiscard]] bool isRook() const;
        [[nodiscard]] bool isQueen() const;
        [[nodiscard]] bool isKing() const;



        // [[nodiscard]] vector<Character_Instance> getCharacters() const {
      //     return characters;
      // }

        [[nodiscard]] virtual vector<pair<int, int>> getMoves() const = 0;
        [[nodiscard]] virtual vector<pair<int, int>> getEffectRange(Effect_List effect) const = 0;
        [[nodiscard]] virtual vector<Effect_List> getCasterEffects() const = 0;
        [[nodiscard]] virtual bool isCheating () const {return false;}
        virtual void passive(void* context) = 0;
        virtual bool canEvolve(void* context) = 0;
        virtual void evolvedForm(void* context) = 0;


};



#endif //PIECES_H
