#pragma once
struct Move {
    Piece* piece;
    int fromX, fromY;
    int toX, toY;
    int score; 

    Move() : piece(nullptr), fromX(0), fromY(0), toX(0), toY(0), score(0) {}
    Move(Piece* p, int tx, int ty) : piece(p), toX(tx), toY(ty), score(0) {
        if(p) {
            Coordinate pos = p->getBoardPosition();
            fromX = pos.x;
            fromY = pos.y;
        }
    }
};

struct MoveList {
    Move moves[256]; 
    int count = 0;

    void add(Piece* p, int tx, int ty) {
        if (count < 256) {
            moves[count] = Move(p, tx, ty);
            count++;
        }
    }
    
    Move* begin() { return &moves[0]; }
    Move* end() { return &moves[count]; }
};