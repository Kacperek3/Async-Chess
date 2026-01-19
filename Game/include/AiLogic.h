#pragma once
#include "Board.h"
#include "Game.h"
#include <vector>
#include <random>
#include <cstdint>
#include <unordered_map>
#include "Move.h"

class AiLogic {
public:

    AiLogic(GameDataRef data);
    ~AiLogic();
    float evaluate(int color);

    float negaMax(int depth, int color, float alpha, float beta);
    std::pair<Piece*, Coordinate> getBestMove(int depth, int color);
    void aiMove(int color);
    float quiescence(float alpha, float beta, int color, int qDepth);
    Board *_board;



    struct TTEntry {
        uint64_t hash;
        float score;
        int depth;
        int flag;
        int bestMoveSrcX, bestMoveSrcY;
        int bestMoveDstX, bestMoveDstY;
    };

private:


    static const int TT_SIZE = 1048576; 
    TTEntry transpositionTable[TT_SIZE];

    void clearTT();
    void storeTT(uint64_t hash, float score, int depth, int flag, Piece* bestPiece, Coordinate bestTarget);
    bool probeTT(uint64_t hash, int depth, float alpha, float beta, float& score, Piece*& bestPiece, Coordinate& bestTarget);


    float getPositionValue(Piece* piece);
    bool canBeCaptured(Piece* piece);
    int canBeCapturedRecursive(Piece* piece);
    void handleCastle(Piece* movedPiece, const Coordinate& target, Piece*& rook, Coordinate& rookOriginalPosition, Coordinate& rookTargetPosition);
    void undoCastle(Piece* movedPiece, const Coordinate& target);
    void initializeZobristTable();
    uint64_t computeZobristHash(Board* board);
    bool isPromisingMove(Piece* movedPiece, Coordinate target);
    bool isEndgamePhase();
    void getSortedMoves(int color, MoveList& moves, Piece* ttPiece, Coordinate ttTarget);
    std::pair<Piece*, Coordinate> getBestMoveIterative(int maxDepth, int color);

    GameDataRef _data;

    uint64_t zobristTable[64][12];

    const float pawnBlackPositionValues[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0.3, 0.3, 0.5, -0.5, -0.5, 0.5, 0.3, 0.3},
        {0.3, 0.1, 0.0, 0.5, 0.5, 0.0, 0.1, 0.3},
        {0, 0.1, 0.5, 1, 1, 0.5, 0.1, 0},
        {0.3, 0.5, 0.7, 1.2, 1.2, 0.7, 0.5, 0.3},
        {0.5, 0.7, 1.0, 1.5, 1.5, 1.0, 0.7, 0.5},
        {1.0, 1.0, 1.2, 1.5, 1.5, 1.2, 1.0, 1.0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

   const float kingEndgameValues[8][8] = {
        {-5, -4, -3, -2, -2, -3, -4, -5},
        {-3, -2, -1,  0,  0, -1, -2, -3},
        {-3, -1,  2,  3,  3,  2, -1, -3},
        {-3, -1,  3,  4,  4,  3, -1, -3},
        {-3, -1,  3,  4,  4,  3, -1, -3},
        {-3, -1,  2,  3,  3,  2, -1, -3},
        {-3, -3,  0,  0,  0,  0, -3, -3},
        {-5, -3, -3, -3, -3, -3, -3, -5}
    }; 
    
    const float pawnWhitePositionValues[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1.0, 1.0, 1.2, 1.5, 1.5, 1.2, 1.0, 1.0},
        {0.5, 0.7, 1.0, 1.5, 1.5, 1.0, 0.7, 0.5},
        {0.3, 0.5, 0.7, 1.2, 1.2, 0.7, 0.5, 0.3},
        {0, 0.1, 0.5, 1, 1, 0.5, 0.1, 0},
        {0.3, 0.1, 0.0, 0.5, 0.5, 0.0, 0.1, 0.3},
        {0.3, 0.3, 0.5, -0.5, -0.5, 0.5, 0.3, 0.3},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    const float knightPositionValues[8][8] = {
        {-2.5, -2.0, -1.5, -1.5, -1.5, -1.5, -2.0, -2.5},
        {-1.5, -1.0, -0.5, -0.5, -0.5, -0.5, -1.0, -1.5},
        {-1.0, 0.0, 0.5, 1.0, 1.0, 0.5, 0.0, -1.0},
        {-0.5, 0.5, 1.0, 1.5, 1.5, 1.0, 0.5, -0.5},
        {-0.5, 0.5, 1.0, 1.5, 1.5, 1.0, 0.5, -0.5},
        {-1.0, 0.0, 0.5, 1.0, 1.0, 0.5, 0.0, -1.0},
        {-1.5, -1.0, -0.5, -0.5, -0.5, -0.5, -1.0, -1.5},
        {-2.5, -2.0, -1.5, -1.5, -1.5, -1.5, -2.0, -2.5}
    };


    const float bishopPositionValues[8][8] = {
    {-1.0, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -1.0},
    {-0.5, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, -0.5},
    {-0.5, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -0.5},
    {-0.5, 0.5, 0.5, 0.7, 0.7, 0.5, 0.5, -0.5},
    {-0.5, 0.0, 0.7, 0.7, 0.7, 0.7, 0.0, -0.5},
    {-0.5, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, -0.5},
    {-0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, -0.5},
    {-1.0, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -1.0}
};


   const float rookPositionValues[8][8] = {
    {0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0},
    {0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5},
    {0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0},
    {0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0},
    {0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0},
    {0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0},
    {0.5, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.5},
    {0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0}
};


    const float queenPositionValues[8][8] = {
    {-0.5, -0.3, -0.3, -0.1, -0.1, -0.3, -0.3, -0.5},
    {-0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.3},
    {-0.3, 0.0, 0.2, 0.2, 0.2, 0.2, 0.0, -0.3},
    {-0.1, 0.0, 0.2, 0.3, 0.3, 0.2, 0.0, -0.1},
    {-0.1, 0.0, 0.2, 0.3, 0.3, 0.2, 0.0, -0.1},
    {-0.3, 0.2, 0.2, 0.2, 0.2, 0.2, 0.0, -0.3},
    {-0.3, 0.0, 0.2, 0.0, 0.0, 0.0, 0.0, -0.3},
    {-0.5, -0.3, -0.3, -0.1, -0.1, -0.3, -0.3, -0.5}
};


    const float kingPositionValuesWhite[8][8] = {
        {-3, -4, -4, -5, -5, -4, -4, -3},
        {-3, -4, -4, -5, -5, -4, -4, -3},
        {-3, -4, -4, -5, -5, -4, -4, -3},
        {-3, -4, -4, -5, -5, -4, -4, -3},
        {-2, -3, -3, -4, -4, -3, -3, -2},
        {-1, -2, -2, -2, -2, -2, -2, -1},
        {2, 2, 0, 0, 0, 0, 2, 2},
        {2, 3, 1, 0, 0, 1, 3, 2}
    };

    const float kingPositionValuesBlack[8][8] = {
        {2, 3, 1, 0, 0, 1, 3, 2},
        {2, 2, 0, 0, 0, 0, 2, 2},
        {-1, -2, -2, -2, -2, -2, -2, -1},
        {-2, -3, -3, -4, -4, -3, -3, -2},
        {-3, -4, -4, -5, -5, -4, -4, -3},
        {-3, -4, -4, -5, -5, -4, -4, -3},
        {-3, -4, -4, -5, -5, -4, -4, -3},
        {-3, -4, -4, -5, -5, -4, -4, -3}
    };

};

