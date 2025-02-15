//
// Created by david on 24/01/2025.
//

#ifndef KING_H
#define KING_H
#include <GameEngine.h>

#include "Pieces.h"


class King : public Pieces{
    public:
        King(int startX, int startY, bool white, Characters_List hero, Pieces_List pieces_root)
            : Pieces(startX, startY, white, hero, pieces_root) {
            if (white)
                GameEngine::getInstance()->setWhiteKing(this);
            else
                GameEngine::getInstance()->setBlackKing(this);
        }

        [[nodiscard]] vector<pair<int, int>> getMoves() override;

};



#endif //KING_H
