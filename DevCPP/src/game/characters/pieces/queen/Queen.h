//
// Created by david on 24/01/2025.
//

#ifndef QUEEN_H
#define QUEEN_H
#include "Pieces.h"


class Queen : public Pieces{
    public:
        Queen(int startX, int startY, bool white, Characters_List hero, Pieces_List pieces_root)
            : Pieces(startX, startY, white, hero, pieces_root) {
        }

        [[nodiscard]] vector<pair<int, int>> getMoves() const override;

};



#endif //QUEEN_H
