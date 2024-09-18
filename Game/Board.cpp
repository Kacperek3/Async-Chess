#include "Board.h"

Board::Board() {
    // białe bierki
    // czarne bierki
    b_pieces.push_back(new Rook(BLACK, 0, 0, this));
    b_pieces.push_back(new Knight(BLACK, 1, 0, this));
    b_pieces.push_back(new Bishop(BLACK, 2, 0, this));
    b_pieces.push_back(new Queen(BLACK, 3, 0, this));
    b_pieces.push_back(new King(BLACK, 4, 0, this));
    b_pieces.push_back(new Bishop(BLACK, 5, 0, this));
    b_pieces.push_back(new Knight(BLACK, 6, 0, this));
    b_pieces.push_back(new Rook(BLACK, 7, 0, this));
    for (int i = 0; i < 8; i++) {
        b_pieces.push_back(new Pawn(BLACK, i, 1, this));
    }

    // białe bierki
    b_pieces.push_back(new Rook(WHITE, 0, 7, this));
    b_pieces.push_back(new Knight(WHITE, 1, 7, this));
    b_pieces.push_back(new Bishop(WHITE, 2, 7, this));
    b_pieces.push_back(new Queen(WHITE, 3, 7, this));
    b_pieces.push_back(new King(WHITE, 4, 7, this));
    b_pieces.push_back(new Bishop(WHITE, 5, 7, this));
    b_pieces.push_back(new Knight(WHITE, 6, 7, this));
    b_pieces.push_back(new Rook(WHITE, 7, 7, this));
    for (int i = 0; i < 8; i++) {
        b_pieces.push_back(new Pawn(WHITE, i, 6, this));
    }

    
}


bool Board::isEmpty(int x, int y) {
    for (auto& piece : b_pieces) {
        if (piece->getBoardPosition().x == x && piece->getBoardPosition().y == y) {
            return false;
        }
    }
    return true;
}

bool Board::isEnemyPieceAt(int boardX, int boardY, int color) const {
    for (auto& piece : b_pieces) {
        if (piece->getBoardPosition().x == boardX && piece->getBoardPosition().y == boardY && piece->getColor() != color) {
            return true;
        }
    }
    return false;
}

void Board::removePiece(int boardX, int boardY) {
    for (auto it = b_pieces.begin(); it != b_pieces.end(); ++it) {
        if ((*it)->getBoardPosition().x == boardX && (*it)->getBoardPosition().y == boardY) {
            delete *it;
            b_pieces.erase(it);
            break;
        }
    }
}



void Board::draw(sf::RenderWindow& window) {
    sf::Color lightColor(238, 238, 210); 
    sf::Color darkColor(118, 150, 86); 

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            sf::RectangleShape tile(sf::Vector2f(75, 75));
            tile.setPosition(col * 75, row * 75);
            if ((row + col) % 2 == 0) {
                    tile.setFillColor(lightColor); 
            } else {
                tile.setFillColor(darkColor);
            }
            window.draw(tile);
        }
    }
    for (auto& piece : b_pieces) {
        piece->draw(window);
    }
}
