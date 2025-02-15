//
// Created by david on 24/01/2025.
//

#ifndef PAWN_H
#define PAWN_H
#include "Pieces.h"


class Pawn : public Pieces{
    public:
        Pawn(int startX, int startY, bool white, Characters_List hero, Pieces_List pieces_root)
            : Pieces(startX, startY, white, hero, pieces_root) {
        }

        [[nodiscard]] vector<pair<int, int>> getMoves() override;

};



#endif //PAWN_H
