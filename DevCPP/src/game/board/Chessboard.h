//
// Created by david on 24/01/2025.
//

#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <iostream>
#include <Pawn.h>
#include <vec2.hpp>
#include <vector>

#include "Effect_List.h"
#include "Pieces.h"

using namespace std;




class Chessboard {
    vector<vector<Pieces*>> grid;
    vector<Pieces*> deadList;
    static Chessboard* instance;
    int size;
    vector<EffectInstance> EffectOnPieces;
public:
    explicit Chessboard(const int size) : grid(size, vector<Pieces*>(size, nullptr)) {
        instance = this;
        this->size = size;
    };
    static Chessboard* getInstance();
    void placePiece(int coordX, int coordY, Pieces* piece);
    bool isMovePossible(Pieces* piece,int to_coordX, int to_coordY) const; // vérifie si le movement est dans le plateau, qu'il n'est pas sur un allié et que la pièce possède ce movement
    vector<pair<int, int>> getValidMoves(Pieces* piece) const;
    bool movePiece(Pieces* piece,int to_coordX,int to_coordY);
    bool KillCheck(Pieces* piece,Pieces* target_piece);
    bool KillInPassing(Pieces *piece, int to_coordX, int to_coordY);

    static bool PawnReachingEndOfBoard(Pieces *piece);
    void displayBoard() const;
    static bool isKillable(Pieces* piece, Pieces* target_piece) ; // vérifie si la pièce peut être tué ( ne possède pas d'effet qui la protège)
    static bool PieceHaveThisEffect(Pieces *piece, Effect_List chosenEffect);

    static bool isMoveable(Pieces* piece) ; // vérifie si la pièce n'est pas affecté par des effets d'immobilisation
    bool isKilled(Pieces* piece) const; // vérifie si la pièce est morte
    static bool isAlly(Pieces* piece, Pieces* target_piece);
    bool isPathClear(int endX, int endY, Pieces* piece) const ;
    bool isPathClear(int startX, int startY, int endX, int endY, Pieces* piece) const ;
    bool isPathAllClear(int endX, int endY, Pieces* piece) const ;
    [[nodiscard]] bool isPathAllClear(int startX, int startY, int endX, int endY) const ;
    [[nodiscard]] bool isInGrid(int to_coordX, int to_coordY) const;
    [[nodiscard]] vector<vector<Pieces *>> getGrid() const;
    vector<vector<Pieces*>>* getGrid_ptr();

    void deletePiece(Pieces *piece);

    [[nodiscard]] vector<Pieces*> getDeadList() const;
    void addToDeadList(Pieces* piece);
    bool getPosInBoard(glm::vec2 screenPos, glm::ivec2 &boardPos) const;
    bool isEndangeredByWhite(pair<int, int> cell);
    bool isEndangeredByBlack(pair<int, int> cell);
    bool canLittleRoque(Pieces* piece);
    bool canBigRoque(Pieces* piece);
    bool bigRoque(Pieces* piece, int to_coordX, int to_coordY);
    bool littleRoque(Pieces* piece, int to_coordX, int to_coordY);
    bool pawnMenacingLittleRoque(Pieces* king);
    bool pawnMenacingBigRoque(Pieces* king);
    static bool hasJustFirstMove(Pieces* piece);
    static bool isPassable(Pieces* piece);
    static bool notBrokenMove(Pieces* piece, Pieces* target_piece);
    vector<Pieces*> getAllPieces();
};



#endif //CHESSBOARD_H