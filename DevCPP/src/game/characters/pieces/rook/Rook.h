//
// Created by david on 24/01/2025.
//

#ifndef ROOK_H
#define ROOK_H
#include "Pieces.h"


class Rook : public Pieces{
    public:
        Rook(int startX, int startY, bool white, Characters_List hero, Pieces_List pieces_root)
            : Pieces(startX, startY, white, hero, pieces_root) {
        }

        [[nodiscard]] vector<pair<int, int>> getMoves() const override;

};



#endif //ROOK_H
