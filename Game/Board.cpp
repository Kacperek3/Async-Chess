#include "Board.h"

Board::Board() {
    // białe bierki
    // czarne bierki
    
    b_pieces.push_back(new Queen(BLACK, 3, 0, this));
    
    //b_pieces.push_back(new Pawn(WHITE, 3, 7, this));
    b_pieces.push_back(new Pawn(WHITE, 4, 7, this));
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








//-------------------------------------------------------------------------------------------
// logika matowania krola


Piece* Board::findKing(int color) {
    for (auto& piece : b_pieces) {
        if (piece->getType() == Piece::PieceType::King && piece->getColor() == color) {
            return piece;
        }
    }
    return nullptr; // nigdy nie powinno dojść do tego miejsca w normalnej grze
}

bool Board::isWithinBounds(int x, int y) {
    return (x >= 0 && x < 8 && y >= 0 && y < 8);
}

std::vector<Piece*> Board::playerPieces(int color) {
    std::vector<Piece*> playerPieces;
    for (auto& piece : b_pieces) {
        if (piece->getColor() == color) {
            playerPieces.push_back(piece);
        }
    }
    return playerPieces;
}

std::vector<Piece*> Board::enemyPieces(int color) {
    std::vector<Piece*> enemyPieces;
    for (auto& piece : b_pieces) {
        if (piece->getColor() != color) {
            enemyPieces.push_back(piece);
        }
    }
    return enemyPieces;
}

std::vector<Coordinate> Board::getValidMoves(Piece* piece) {
    std::vector<Coordinate> validMoves;
    std::vector<Coordinate> allMoves = piece->getPossibleMoves();  // Get all possible moves for the piece

    for (auto& move : allMoves) {
        if (isWithinBounds(move.x, move.y) && !isKingInCheckAfterMove(piece, move)) {
            validMoves.push_back(move);
        }
    }

    return validMoves;
}

void Board::simulateMove(Piece* piece, Coordinate targetPosition) {
    Coordinate originalPosition = piece->getBoardPosition();
    piece->move(targetPosition.x, targetPosition.y);
}

void Board::undoMove(Piece* piece, Coordinate originalPosition, Piece* capturedPiece) {
    // przywrocenie oryginalnej pozycji
    piece->move(originalPosition.x, originalPosition.y);

    // jesli byla zbita jakas figura to dodaj ja z powrotem
    if (capturedPiece) {
        b_pieces.push_back(capturedPiece);
    }
}


bool Board::isKingInCheckAfterMove(Piece* movedPiece, Coordinate targetPosition) {
    Coordinate originalPosition = movedPiece->getBoardPosition();
    Piece* capturedPiece = nullptr;

    for (auto& piece : b_pieces) {
        if (piece->getBoardPosition().x == targetPosition.x && piece->getBoardPosition().y == targetPosition.y) {
            capturedPiece = piece;
            break;
        }
    }

    if (capturedPiece != nullptr) {
        removePiece(targetPosition.x, targetPosition.y);
    }

    movedPiece->move(targetPosition.x, targetPosition.y);
    bool isCheck = isKingInCheck(movedPiece->getColor());

    movedPiece->move(originalPosition.x, originalPosition.y);
    if (capturedPiece != nullptr) {
        b_pieces.push_back(capturedPiece);
    }

    return isCheck;
}


bool Board::isKingInCheck(int color) {
    King* king = dynamic_cast<King*>(findKing(color));
    Coordinate kingPosition = king->getBoardPosition();

    // mozliwe ze bedzie to zabijac krola i wtedy isValidMove do naprawy
    for (auto& piece : b_pieces) {
        if (piece->getColor() != color) {
            if (piece->isValidMove(kingPosition.x, kingPosition.y)) {
                return true;
            }
        }
    }
    return false;
}


bool Board::canKingMove(int color) {
    King* king = dynamic_cast<King*>(findKing(color));
    Coordinate kingPosition = king->getBoardPosition();
    
    // Sprawdź wszystkie sąsiadujące pola wokół króla
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;  // Ignoruj bieżącą pozycję króla

            int newX = kingPosition.x + dx;
            int newY = kingPosition.y + dy;
            
            Coordinate targetPosition(newX, newY);


            // Sprawdź, czy nowe pole jest w granicach planszy
            if (isWithinBounds(newX, newY)) {
                // Sprawdź, czy król może się tam poruszyć bez bycia szachowanym
                if (!isKingInCheckAfterMove(king, targetPosition)) {
                    return true;  // Król ma bezpieczny ruch
                }
            }
        }
    }
    return false;
}

bool Board::canPreventCheck(int color) {
    for (Piece* piece : playerPieces(color)) {
        std::vector<Coordinate> validMoves = piece->getPossibleMoves();
        Coordinate originalPosition = piece->getBoardPosition();
        
        for (const auto& move : validMoves) {
            // Symuluj ruch
            simulateMove(piece, move);
            
            // Sprawdź, czy król nadal jest w szachu po tym ruchu
            if (!isKingInCheck(color)) {
                undoMove(piece,originalPosition,nullptr);  // Cofnij ruch
                return true;  // Ruch zapobiega szachowi
            }

            undoMove(piece,originalPosition,nullptr);  // Cofnij ruch
        }
    }
    return false;


    // to do, co jesli przeciwnik może ochronić króla przed matem bijąc jakąś figure
}


bool Board::isCheckmate(int color) {
    if (!isKingInCheck(color)) {
        return false;  // Król nie jest w szachu
    }

    if (canKingMove(color)) {
        return false;  // Król może się poruszać
    }

    if (canPreventCheck(color)) {
        return false;  // Gracz może zapobiec szachowi
    }

    return true;  // Gracz jest w szachu mat
}


void Board::draw(sf::RenderWindow& window) {
    sf::Color lightColor(238, 238, 210); 
    sf::Color darkColor(118, 150, 86); 

    // Font do wyświetlania koordynatów
    sf::Font font;
    if (!font.loadFromFile("/home/kacper/Pulpit/chess/assets/fonts/Poppins-Thin.ttf")) {
        // Obsługa błędu ładowania fontu
        return;
    }

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            sf::RectangleShape tile(sf::Vector2f(75, 75));
            tile.setPosition(col * 75, row * 75);

            // Kolorowanie kafelków
            if ((row + col) % 2 == 0) {
                tile.setFillColor(lightColor); 
            } else {
                tile.setFillColor(darkColor);
            }
            window.draw(tile);

            // Tworzenie tekstu z koordynatami
            sf::Text coordinates;
            coordinates.setFont(font);
            coordinates.setString(std::to_string(row) + " " + std::to_string(col));
            coordinates.setCharacterSize(20);  // Rozmiar tekstu
            coordinates.setFillColor(sf::Color::Black);  // Kolor tekstu

            // Pozycjonowanie tekstu na środku pola
            coordinates.setPosition(col * 75 + 20, row * 75 + 20); 

            // Rysowanie tekstu
            window.draw(coordinates);
        }
    }

    for (auto& piece : b_pieces) {
        piece->draw(window);
    }
}
