#include "Board.h"

Board::Board(sf::RenderWindow* window) : window(window) {
    
    // białe bierki
    b_pieces.push_back(new Rook(BLACK, 0, 0, this));
    b_pieces.push_back(new Knight(BLACK, 1, 0, this));
    b_pieces.push_back(new Bishop(BLACK, 2, 0, this));
    b_pieces.push_back(new Queen(BLACK, 3, 0, this));
    b_pieces.push_back(new King(BLACK, 4, 0, this));
    b_pieces.push_back(new Bishop(BLACK, 5, 0, this));
    b_pieces.push_back(new Knight(BLACK, 6, 0, this));
    b_pieces.push_back(new Rook(BLACK, 7, 0, this));
    for(int i = 0; i < 8; i++){
        b_pieces.push_back(new Pawn(BLACK, i, 1, this, UP));
    }


    //czarne bierki
    b_pieces.push_back(new Rook(WHITE, 0, 7, this));
    b_pieces.push_back(new Knight(WHITE, 1, 7, this));
    b_pieces.push_back(new Bishop(WHITE, 2, 7, this));
    b_pieces.push_back(new Queen(WHITE, 3, 7, this));
    b_pieces.push_back(new King(WHITE, 4, 7, this));
    b_pieces.push_back(new Bishop(WHITE, 5, 7, this));
    b_pieces.push_back(new Knight(WHITE, 6, 7, this));
    b_pieces.push_back(new Rook(WHITE, 7, 7, this));
    for(int i = 0; i < 8; i++){
        b_pieces.push_back(new Pawn(WHITE, i, 6, this, DOWN));
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
    for (const auto& piece : enemyPieces(color)) {
        if (piece->getBoardPosition().x == boardX && piece->getBoardPosition().y == boardY) {
            return true;
        }
    }
    return false;
}

void Board::removePiece(int boardX, int boardY) {
    for (auto it = b_pieces.begin(); it != b_pieces.end(); ) {
        if ((*it) != nullptr && (*it)->getBoardPosition().x == boardX && (*it)->getBoardPosition().y == boardY) {
            delete *it;                // Usunięcie obiektu
            it = b_pieces.erase(it);    // Usuń element z wektora i zaktualizuj iterator
        } else {
            ++it; // Przejdź do następnego elementu tylko wtedy, gdy nie usuwasz elementu
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


Piece* Board::getPieceAt(int x, int y) {
    for (auto& piece : b_pieces) {
        if (piece->getBoardPosition().x == x && piece->getBoardPosition().y == y) {
            return piece;
        }
    }
    return nullptr;
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

std::vector<Piece*> Board::enemyPieces(int color) const{
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
std::vector<Coordinate> Board::getValidCaptures(Piece* piece) {
    std::vector<Coordinate> validCaptures;
    std::vector<Coordinate> allCaptures = piece->getPossibleCaptures();  // Get all possible moves for the piece

    for (auto& capture : allCaptures) {
        if (isWithinBounds(capture.x, capture.y) && !isKingInCheckAfterMove(piece, capture)) {
            validCaptures.push_back(capture);
        }
    }

    return validCaptures;
}

void Board::undoMove(Piece* piece, Coordinate originalPosition, Piece* capturedPiece) {
    // przywrocenie oryginalnej pozycji
    piece->simulateMove(originalPosition.x, originalPosition.y);

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
        capturedPiece->simulateMove(-1, -1);
    }

    movedPiece->simulateMove(targetPosition.x, targetPosition.y);
    bool isCheck = isKingInCheck(movedPiece->getColor());

    movedPiece->simulateMove(originalPosition.x, originalPosition.y);

    if (capturedPiece != nullptr) {
        capturedPiece->simulateMove(targetPosition.x, targetPosition.y);
    }

    return isCheck;
}


bool Board::isKingInCheck(int color) {
    King* king = dynamic_cast<King*>(findKing(color));
    Coordinate kingPosition = king->getBoardPosition();

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

            if(!isKingInCheckAfterMove(piece, move)){
                return true;
            }
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



bool Board::isStalemate(int color) {
    if (isKingInCheck(color)) {
        return false;
    }

    if (canKingMove(color)) {
        return false;
    }

    // Sprawdź, czy jakakolwiek inna figura może się ruszyć lub zapobiec szachowi
    for (Piece* piece : playerPieces(color)) {
        std::vector<Coordinate> validMoves = piece->getPossibleMoves();
        for (const auto& move : validMoves) {
            // Sprawdz czy twoja bierka nie jest zwiazana
            if (!isKingInCheckAfterMove(piece, move)) {
                return false;  // jesli nie jest to znaczy ze moze sie ruszyc i jest to pat
            }
        }
    }

    return true;
}


void Board::promotePawn(Piece* pawn) {
    // Tworzenie nowego okna dla interfejsu promocji
    sf::RenderWindow promotionWindow(sf::VideoMode(270, 200), "Pawn Promotion");

    sf::Texture queenTexture, rookTexture, bishopTexture, knightTexture;
    if(pawn->getColor() == WHITE){
        if(!queenTexture.loadFromFile("../assets/pieces/chessCom1/wq.png") ||
           !rookTexture.loadFromFile("../assets/pieces/chessCom1/wr.png") ||
           !bishopTexture.loadFromFile("../assets/pieces/chessCom1/wb.png") ||
           !knightTexture.loadFromFile("../assets/pieces/chessCom1/wn.png")) {
            return; 
        }
    } else {
        if (!queenTexture.loadFromFile("../assets/pieces/chessCom1/bq.png") ||
            !rookTexture.loadFromFile("../assets/pieces/chessCom1/br.png") ||
            !bishopTexture.loadFromFile("../assets/pieces/chessCom1/bb.png") ||
            !knightTexture.loadFromFile("../assets/pieces/chessCom1/bn.png")) {
            return;
        }
    }

    // Utwórz sprite'y dla każdego typu figury
    sf::Sprite queenSprite(queenTexture), rookSprite(rookTexture), bishopSprite(bishopTexture), knightSprite(knightTexture);

    // Ustaw pozycje sprite'ów w nowym oknie
    queenSprite.setPosition(50, 15);
    rookSprite.setPosition(150, 15);
    bishopSprite.setPosition(50, 105);
    knightSprite.setPosition(150, 105);

    // Pętla promocji
    Piece* newPiece = nullptr;
    bool promotionSelected = false;

    while (promotionWindow.isOpen() && !promotionSelected) {
        sf::Event event;
        while (promotionWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                promotionWindow.close();
            } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(promotionWindow);

                // Sprawdź, który sprite został kliknięty
                if (queenSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    newPiece = new Queen(pawn->getColor(), pawn->getBoardPosition().x, pawn->getBoardPosition().y, this);
                    promotionSelected = true;
                } else if (rookSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    newPiece = new Rook(pawn->getColor(), pawn->getBoardPosition().x, pawn->getBoardPosition().y, this);
                    promotionSelected = true;
                } else if (bishopSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    newPiece = new Bishop(pawn->getColor(), pawn->getBoardPosition().x, pawn->getBoardPosition().y, this);
                    promotionSelected = true;
                } else if (knightSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    newPiece = new Knight(pawn->getColor(), pawn->getBoardPosition().x, pawn->getBoardPosition().y, this);
                    promotionSelected = true;
                }
            }
        }

        // Rysowanie nowego okna
        promotionWindow.clear();
        promotionWindow.draw(queenSprite);
        promotionWindow.draw(rookSprite);
        promotionWindow.draw(bishopSprite);
        promotionWindow.draw(knightSprite);
        promotionWindow.display();
    }

    // Zamknij okno promocji po dokonaniu wyboru
    promotionWindow.close();

    // Zamień pionka na nową figurę
    if (newPiece) {
        b_pieces.push_back(newPiece);
    }
}


// funkcje odpowiedzialne za rysowanie planszy i bierki itp

void Board::showPossibleMoves(sf::RenderWindow& window, Piece* piece){
    std::vector<Coordinate> possibleMoves = getValidMoves(piece);
    sf::Color circleColor(183, 180, 180, 128);
    for(auto& move : possibleMoves){
        sf::CircleShape circle(10);
        circle.setFillColor(circleColor);
        circle.setPosition(move.x * 75 + 28, move.y * 75 + 28);
        window.draw(circle);
    }
}

void Board::showPossibleCaptures(sf::RenderWindow& window, Piece* piece){
    sf::Color recColor(245, 90, 105, 128);
    std::vector<Coordinate> possibleCaptures = getValidCaptures(piece);
    for(auto& capture : possibleCaptures){
        sf::RectangleShape markedField(sf::Vector2f(75, 75));
        markedField.setFillColor(recColor);
        markedField.setPosition(capture.x * 75, capture.y * 75);
        window.draw(markedField);
    }
}
void Board::showCheck(sf::RenderWindow& window, int color){
    if(isKingInCheck(color)){
        sf::Color recColor(245, 90, 105, 128);
        King* king = dynamic_cast<King*>(findKing(color));
        sf::RectangleShape markedField(sf::Vector2f(75, 75));
        markedField.setFillColor(recColor);
        markedField.setPosition(king->getBoardPosition().x * 75, king->getBoardPosition().y * 75);
        window.draw(markedField);
    }
}

void Board::markPieceField(sf::RenderWindow& window, Piece* piece){
    sf::Color recColor(223, 223, 103, 128);
    sf::RectangleShape markedField(sf::Vector2f(75, 75));
    markedField.setFillColor(recColor);
    markedField.setPosition(piece->getBoardPosition().x * 75, piece->getBoardPosition().y * 75);
    window.draw(markedField);
}

void Board::drawPieces(sf::RenderWindow& window, Piece* draggedPiece) {
    for (auto& piece : b_pieces) {
        if (piece != draggedPiece) {
            piece->draw(window);
        }
    }
    if (draggedPiece != nullptr) {
        draggedPiece->draw(window);
    }
}


void Board::drawBoard(sf::RenderWindow& window, bool showCoordinates) {
    sf::Color lightColor(238, 238, 210); 
    sf::Color darkColor(118, 150, 86); 

    // Font do wyświetlania koordynatów
    sf::Font font;
    if (!font.loadFromFile("../assets/fonts/Poppins-Thin.ttf")) {
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

            // Rysowanie koordynatów, jeśli showCoordinates jest true
            if (showCoordinates) {
                sf::Text coordinates;
                coordinates.setFont(font);
                coordinates.setString(std::to_string(col) + " " + std::to_string(row));
                coordinates.setCharacterSize(20);  // Rozmiar tekstu
                coordinates.setFillColor(sf::Color::Black);  // Kolor tekstu

                // Pozycjonowanie tekstu na środku pola
                coordinates.setPosition(col * 75 + 20, row * 75 + 20); 

                // Rysowanie tekstu
                window.draw(coordinates);
            }
        }
    }
}


void Board::rotatePieces(){
    for(auto& piece : b_pieces){
        piece->rotatePiece();
    }
}