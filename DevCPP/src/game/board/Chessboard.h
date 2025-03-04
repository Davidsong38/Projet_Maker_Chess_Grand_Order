//
// Created by david on 24/01/2025.
//

#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>

#include "effects.h"
#include "Pieces.h"

using namespace std;


struct chessboard_cell {
    Pieces* piece;
    bool selected;
    glm::ivec2 pos;
};


class Chessboard {
    vector<vector<chessboard_cell>> grid;
    vector<Pieces*> deadList;
    static Chessboard* instance;
    int size;
    vector<EffectInstance> EffectOnPieces;
public:
    explicit Chessboard(const int size) : grid(size, vector<chessboard_cell>(size)) {
        instance = this;
        this->size = size;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                grid[i][j].pos = glm::ivec2(i, j);
            }
        }
    };
    static Chessboard* getInstance();
    void placePiece(int coordX, int coordY, Pieces* piece);
    bool isMovePossible(const Pieces* piece,int to_coordX, int to_coordY) const; // vérifie si le movement est dans le plateau, qu'il n'est pas sur un allié et que la pièce possède ce movement
    vector<glm::ivec2> getValidMoves(const Pieces* piece) const;
    bool movePiece(Pieces* piece,int to_coordX,int to_coordY);
    static bool KillCheck(Pieces* piece,Pieces* target_piece);
    bool KillInPassing(Pieces *piece, int to_coordX, int to_coordY);

    static bool PawnReachingEndOfBoard(Pieces *piece);
    static bool isKillable(const Pieces* piece, Pieces* target_piece) ; // vérifie si la pièce peut être tué ( ne possède pas d'effet qui la protège)

    static bool isMoveable(const Pieces* piece) ; // vérifie si la pièce n'est pas affecté par des effets d'immobilisation
    bool isKilled(const Pieces* piece) const; // vérifie si la pièce est morte
    static bool isAlly(const Pieces* piece, const Pieces* target_piece);
    bool isPathClear(int endX, int endY, const Pieces* piece) const ;
    bool isPathClear(int startX, int startY, int endX, int endY, const Pieces* piece) const ;
    bool isPathAllClear(int endX, int endY, Pieces* piece) const ;
    [[nodiscard]] bool isPathAllClear(int startX, int startY, int endX, int endY) const ;
    [[nodiscard]] bool isInGrid(int to_coordX, int to_coordY) const;
    [[nodiscard]] Pieces* getPieceAt(int coordX, int coordY) const;
    [[nodiscard]] chessboard_cell* getCellAt(int coordX, int coordY);
    [[nodiscard]] int getSize() const {return size;}

    void deletePiece(const Pieces *piece);

    [[nodiscard]] vector<Pieces*> getDeadList() const;
    void addToDeadList(Pieces* piece);
    void removeFromDeadList(Pieces *piece);

    bool getPosInBoard(glm::vec2 screenPos, glm::ivec2 &boardPos) const;
    bool isEndangeredByWhite(glm::ivec2 cell);
    bool isEndangeredByBlack(glm::ivec2 cell);
    bool canLittleRoque(Pieces* piece);
    bool canBigRoque(Pieces* piece);
    bool bigRoque(Pieces* piece, int to_coordX, int to_coordY);
    bool littleRoque(Pieces* piece, int to_coordX, int to_coordY);
    bool pawnMenacingLittleRoque(Pieces* king);
    bool pawnMenacingBigRoque(Pieces* king);
    static bool hasJustFirstMove(Pieces* piece);
    static bool notBrokenMove(const Pieces* piece, const Pieces* target_piece);
    vector<Pieces*> getAllPieces();
};



#endif //CHESSBOARD_H