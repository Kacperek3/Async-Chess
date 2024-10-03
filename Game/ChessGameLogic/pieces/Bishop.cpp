#include "Bishop.h"
#include "Board.h"

Bishop::Bishop() {
    
}

Bishop::Bishop(int color, int boardX, int boardY, Board* board)
    : Piece(color, Position(boardX, boardY).getPixelPosition(), Piece::PieceType::Bishop, boardX, boardY) 
{
    this->board = board;
    std::string filePath;
    if (color == 0) {
        filePath =  "../assets/pieces/chessCom1/wb.png";
    } else {
        filePath = "../assets/pieces/chessCom1/bb.png";
    }

    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error loading texture: " << filePath << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(m_position);
}

std::vector<Coordinate> Bishop::getPossibleMoves() {
    std::vector<Coordinate> possibleMoves;

    int directions[4][2] = {
        {1, 1},   // Przekątna w prawo w dół
        {-1, 1},  // Przekątna w lewo w dół
        {-1, -1}, // Przekątna w lewo w górę
        {1, -1}   // Przekątna w prawo w górę
    };

    for (auto& direction : directions) {
        int dx = direction[0];
        int dy = direction[1];

        int x = boardPosition.x;
        int y = boardPosition.y;

        // Przechodzimy w danym kierunku, dopóki możemy się poruszać
        while (true) {
            x += dx;
            y += dy;

            // Sprawdzenie, czy współrzędne są w granicach planszy
            if (board->isWithinBounds(x, y)) {
                if (board->isEmpty(x, y)) {
                    possibleMoves.push_back(Coordinate(x, y));
                } else {
                    // Jeśli jest to bierka przeciwnika, dodajemy ruch, ale nie możemy iść dalej
                    if (board->isEnemyPieceAt(x, y, m_color)) {
                        possibleMoves.push_back(Coordinate(x, y));
                    }
                    break; // Przerywamy pętlę, bo natrafiliśmy na bierkę
                }
            } else {
                break; // Wyszedłeś poza planszę
            }
        }
    }

    return possibleMoves;
}

std::vector<Coordinate> Bishop::getPossibleCaptures() {
    std::vector<Coordinate> possibleCaptures;

    int directions[4][2] = {
        {1, 1},   // Przekątna w prawo w dół
        {-1, 1},  // Przekątna w lewo w dół
        {-1, -1}, // Przekątna w lewo w górę
        {1, -1}   // Przekątna w prawo w górę
    };

    for (auto& direction : directions) {
        int dx = direction[0];
        int dy = direction[1];

        int x = boardPosition.x;
        int y = boardPosition.y;

        // Przechodzimy w danym kierunku, dopóki nie napotkamy figur
        while (true) {
            x += dx;
            y += dy;

            // Sprawdzenie, czy współrzędne są w granicach planszy
            if (board->isWithinBounds(x, y)) {
                if (!board->isEmpty(x, y)) {
                    // Jeśli jest to bierka przeciwnika, dodajemy ją jako możliwy ruch
                    if (board->isEnemyPieceAt(x, y, m_color)) {
                        possibleCaptures.push_back(Coordinate(x, y));
                    }
                    break; // Natrafiliśmy na bierkę, więc nie możemy dalej iść w tym kierunku
                }
            } else {
                break; // Wyszedłeś poza planszę
            }
        }
    }

    return possibleCaptures;
}

bool Bishop::isValidMove(int boardX, int boardY) {
    if (!board->isWithinBounds(this->boardPosition.x, this->boardPosition.y) || !board->isWithinBounds(boardX, boardY)) {
        return false;  // Ruch poza planszę
    }

    int dx = abs(boardX - boardPosition.x);
    int dy = abs(boardY - boardPosition.y);

    // Sprawdzenie, czy ruch jest po przekątnej (|deltaX| == |deltaY|)
    if (dx == dy) {
        // Sprawdzenie, czy droga do docelowej pozycji jest wolna
        int xDirection = (boardX - boardPosition.x > 0) ? 1 : -1;
        int yDirection = (boardY - boardPosition.y > 0) ? 1 : -1;
        
        for (int i = 1; i < dx; ++i) {  // Sprawdź każde pole na drodze
            if (!board->isEmpty(boardPosition.x + i * xDirection, boardPosition.y + i * yDirection)) {
                return false;  // Jeśli na drodze jest figura, ruch jest niedozwolony
            }
        }

        // Jeżeli docelowe pole jest puste, lub znajduje się na nim figura przeciwnika
        if (board->isEmpty(boardX, boardY)) {
            return true;
        } else if (board->isEnemyPieceAt(boardX, boardY, m_color)) {
            return true;
        }
    }

    return false;  // W innych przypadkach ruch jest niedozwolony
}

void Bishop::rotatePiece() {
    Piece::rotatePiece();
}

Bishop::~Bishop() {}

void Bishop::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
