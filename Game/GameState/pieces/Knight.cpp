#include "Knight.h"
#include "Board.h"

Knight::Knight() {
    
}

Knight::Knight(int color, int boardX, int boardY, Board* board)
    : Piece(color, Position(boardX, boardY).getPixelPosition(), Piece::PieceType::Knight, boardX, boardY) 
{
    this->board = board;
    
    std::filesystem::path currentPath = std::filesystem::current_path().parent_path();
    std::string filePath;

    if (color == 0) {
        filePath = (currentPath / "assets/pieces/wKnight.png").string();
    } else {
        filePath = (currentPath / "assets/pieces/bKnight.png").string();
    }

    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error loading texture: " << filePath << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(m_position);
}


std::vector<Coordinate> Knight::getPossibleMoves() {
    std::vector<Coordinate> possibleMoves;

    int directions[8][2] = {
        {2, 1},   // 2 w prawo, 1 w dół
        {1, 2},   // 1 w prawo, 2 w dół
        {-1, 2},  // 1 w lewo, 2 w dół
        {-2, 1},  // 2 w lewo, 1 w dół
        {-2, -1}, // 2 w lewo, 1 w górę
        {-1, -2}, // 1 w lewo, 2 w górę
        {1, -2},  // 1 w prawo, 2 w górę
        {2, -1}   // 2 w prawo, 1 w górę
    };

    for (auto& direction : directions) {
        int dx = direction[0];
        int dy = direction[1];

        int x = boardPosition.x + dx;
        int y = boardPosition.y + dy;

        if (board->isWithinBounds(x, y)) {
            if (board->isEmpty(x, y) || board->isEnemyPieceAt(x, y, m_color)) {
                possibleMoves.push_back(Coordinate(x, y));
            }
        }
    }

    return possibleMoves;
}

std::vector<Coordinate> Knight::getPossibleCaptures() {
    std::vector<Coordinate> possibleCaptures;

    int directions[8][2] = {
        {2, 1},   // 2 w prawo, 1 w dół
        {1, 2},   // 1 w prawo, 2 w dół
        {-1, 2},  // 1 w lewo, 2 w dół
        {-2, 1},  // 2 w lewo, 1 w dół
        {-2, -1}, // 2 w lewo, 1 w górę
        {-1, -2}, // 1 w lewo, 2 w górę
        {1, -2},  // 1 w prawo, 2 w górę
        {2, -1}   // 2 w prawo, 1 w górę
    };

    for (auto& direction : directions) {
        int dx = direction[0];
        int dy = direction[1];

        int x = boardPosition.x + dx;
        int y = boardPosition.y + dy;

        if (board->isWithinBounds(x, y) && board->isEnemyPieceAt(x, y, m_color)) {
            // Jeśli na tym polu jest wroga bierka, dodajemy do możliwych zbicia
            possibleCaptures.push_back(Coordinate(x, y));
        }
    }

    return possibleCaptures;
}


bool Knight::isValidMove(int boardX, int boardY) {

    if (!board->isWithinBounds(this->boardPosition.x, this->boardPosition.y) || !board->isWithinBounds(boardX, boardY)) {
        return false;  // Ruch poza planszę
    }

    // Obliczenie różnicy między aktualną a docelową pozycją
    int dx = abs(boardX - boardPosition.x);
    int dy = abs(boardY - boardPosition.y);

    // Sprawdzenie, czy ruch jest w kształcie "L" (2 pola w jednym kierunku, 1 pole w drugim)
    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
        // Sprawdzenie, czy na docelowej pozycji znajduje się przeciwnik lub jest puste
        if (board->isEmpty(boardX, boardY)) {
            return true;  // Ruch do pustego pola jest dozwolony
        } else if (board->isEnemyPieceAt(boardX, boardY, m_color)) {
            return true;
        }
    }
    
    return false;  // W innych przypadkach ruch jest niedozwolony
}

Knight::~Knight() {}

void Knight::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
