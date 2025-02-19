//
// Created by david on 24/01/2025.
//

#ifndef BISHOP_H
#define BISHOP_H
#include "Pieces.h"


class Bishop : public Pieces{
    public:
        Bishop(int startX, int startY, bool white, Characters_List hero, Pieces_List pieces_root)
            : Pieces(startX, startY, white, hero, pieces_root) {
        }

        [[nodiscard]] vector<pair<int, int>> getMoves() override;

};



#endif //BISHOP_H
