//
// Created by david on 24/01/2025.
//

#ifndef KNIGHT_H
#define KNIGHT_H
#include "Pieces.h"


class Knight : public Pieces{
    public:
        Knight(int startX, int startY, bool white, Characters_List hero, Pieces_List pieces_root)
            : Pieces(startX, startY, white, hero, pieces_root) {
        }

        [[nodiscard]] vector<pair<int, int>> getMoves() const override;

};



#endif //KNIGHT_H
