#include "Rook.h"
#include "Board.h"

Rook::Rook() {
    
}

Rook::Rook(int color, int boardX, int boardY, Board* board)
    : Piece(color, Position(boardX, boardY).getPixelPosition(), Piece::PieceType::Rook, boardX, boardY) 
{
    this->board = board;
    std::string filePath;
    if (color == 0) {
        filePath =  "../assets/pieces/chessCom1/wr.png";
    } else {
        filePath = "../assets/pieces/chessCom1/br.png";
    }

    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error loading texture: " << filePath << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(m_position);
}

std::vector<Coordinate> Rook::getPossibleMoves() {
    std::vector<Coordinate> possibleMoves;

    // Kierunki ruchu wieży (pionowe i poziome)
    int directions[4][2] = {
        {1, 0},   // W prawo
        {-1, 0},  // W lewo
        {0, 1},   // W dół
        {0, -1}   // W górę
    };

    // Sprawdzanie wszystkich kierunków
    for (auto& direction : directions) {
        int dx = direction[0];
        int dy = direction[1];

        int x = boardPosition.x;
        int y = boardPosition.y;

        while (true) {
            x += dx;
            y += dy;

            if (board->isWithinBounds(x, y)) {
                if (board->isEmpty(x, y)) {
                    possibleMoves.push_back(Coordinate(x, y));
                } else {
                    if (board->isEnemyPieceAt(x, y, m_color)) {
                        possibleMoves.push_back(Coordinate(x, y));
                    }
                    break;  // Nie można kontynuować po napotkaniu bierki
                }
            } else {
                break;
            }
        }
    }

    return possibleMoves;
}

std::vector<Coordinate> Rook::getPossibleCaptures() {
    std::vector<Coordinate> possibleCaptures;

    // Kierunki ruchu wieży (pionowe i poziome)
    int directions[4][2] = {
        {1, 0},   // W prawo
        {-1, 0},  // W lewo
        {0, 1},   // W dół
        {0, -1}   // W górę
    };

    // Sprawdzanie wszystkich kierunków
    for (auto& direction : directions) {
        int dx = direction[0];
        int dy = direction[1];

        int x = boardPosition.x;
        int y = boardPosition.y;

        while (true) {
            x += dx;
            y += dy;

            if (board->isWithinBounds(x, y)) {
                if (board->isEnemyPieceAt(x, y, m_color)) {
                    possibleCaptures.push_back(Coordinate(x, y));
                    break;  // Nie można kontynuować po napotkaniu bierki
                } else if (!board->isEmpty(x, y)) {
                    break;  // Napotkano własną bierkę, nie można kontynuować
                }
            } else {
                break;  // Poza granicami planszy
            }
        }
    }

    return possibleCaptures;
}

bool Rook::isValidMove(int boardX, int boardY) {
    if (!board->isWithinBounds(this->boardPosition.x, this->boardPosition.y) || !board->isWithinBounds(boardX, boardY)) {
        return false;  // Ruch poza planszę
    }
    // Sprawdzenie, czy ruch jest w linii prostej (po osi X lub Y)
    if (boardX != boardPosition.x && boardY != boardPosition.y) {
        return false; // Ruch jest nieprawidłowy, bo wieża nie może poruszać się po skosie
    }

    // Sprawdzenie, czy droga jest wolna
    if (boardX == boardPosition.x) {  // Ruch w pionie
        int direction = (boardY > boardPosition.y) ? 1 : -1;
        for (int i = 1; i < abs(boardY - boardPosition.y); ++i) {
            if (!board->isEmpty(boardX, boardPosition.y + i * direction)) {
                return false;  // Jeśli droga jest zablokowana, ruch jest nieprawidłowy
            }
        }
        
    } else if (boardY == boardPosition.y) {  // Ruch w poziomie
        int direction = (boardX > boardPosition.x) ? 1 : -1;
        for (int i = 1; i < abs(boardX - boardPosition.x); ++i) {
            if (!board->isEmpty(boardPosition.x + i * direction, boardY)) {
                return false;  // Jeśli droga jest zablokowana, ruch jest nieprawidłowy
            }
        }
    }

    // Sprawdzenie, czy docelowe pole jest puste, lub znajduje się tam figura przeciwnika
    if (board->isEmpty(boardX, boardY)) {
        return true;
    } else if (board->isEnemyPieceAt(boardX, boardY, m_color)) {
        return true;
    }

    return false;
}

void Rook::move(int boardX, int boardY) {
    Piece::move(boardX, boardY);
    firstMove = false;
}
void Rook::rotatePiece() {
    Piece::rotatePiece();
}

Rook::~Rook() {}

void Rook::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
