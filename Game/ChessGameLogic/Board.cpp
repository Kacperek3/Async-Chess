#include "Board.h"


Board::Board(GameDataRef data) : _data(data), window(window) {

}


void Board::Init() {
    // do nothing
    _data->assetManager.LoadTexture("wk", "../assets/pieces/chessCom1/wk.png");
    _data->assetManager.LoadTexture("bk", "../assets/pieces/chessCom1/bk.png");
    _data->assetManager.LoadTexture("wp", "../assets/pieces/chessCom1/wp.png");
    _data->assetManager.LoadTexture("bp", "../assets/pieces/chessCom1/bp.png");
    _data->assetManager.LoadTexture("wq", "../assets/pieces/chessCom1/wq.png");
    _data->assetManager.LoadTexture("bq", "../assets/pieces/chessCom1/bq.png");
    _data->assetManager.LoadTexture("wr", "../assets/pieces/chessCom1/wr.png");
    _data->assetManager.LoadTexture("br", "../assets/pieces/chessCom1/br.png");
    _data->assetManager.LoadTexture("wb", "../assets/pieces/chessCom1/wb.png");
    _data->assetManager.LoadTexture("bb", "../assets/pieces/chessCom1/bb.png");
    _data->assetManager.LoadTexture("wn", "../assets/pieces/chessCom1/wn.png");
    _data->assetManager.LoadTexture("bn", "../assets/pieces/chessCom1/bn.png");


    // białe bierki
    b_pieces.push_back(new Rook(BLACK, 0, 0, this, _data->assetManager.GetTexture("br")));
    b_pieces.push_back(new Knight(BLACK, 1, 0, this, _data->assetManager.GetTexture("bn")));
    b_pieces.push_back(new Bishop(BLACK, 2, 0, this, _data->assetManager.GetTexture("bb")));
    b_pieces.push_back(new Queen(BLACK, 3, 0, this, _data->assetManager.GetTexture("bq")));
    b_pieces.push_back(new King(BLACK, 4, 0, this, _data->assetManager.GetTexture("bk")));
    b_pieces.push_back(new Bishop(BLACK, 5, 0, this, _data->assetManager.GetTexture("bb")));
    b_pieces.push_back(new Knight(BLACK, 6, 0, this, _data->assetManager.GetTexture("bn")));
    b_pieces.push_back(new Rook(BLACK, 7, 0, this, _data->assetManager.GetTexture("br")));
    for(int i = 0; i < 8; i++){
        b_pieces.push_back(new Pawn(BLACK, i, 1, this, UP, _data->assetManager.GetTexture("bp")));
    }


    //czarne bierki
    b_pieces.push_back(new Rook(WHITE, 0, 7, this, _data->assetManager.GetTexture("wr")));
    b_pieces.push_back(new Knight(WHITE, 1, 7, this, _data->assetManager.GetTexture("wn")));
    b_pieces.push_back(new Bishop(WHITE, 2, 7, this, _data->assetManager.GetTexture("wb")));
    b_pieces.push_back(new Queen(WHITE, 3, 7, this, _data->assetManager.GetTexture("wq")));
    b_pieces.push_back(new King(WHITE, 4, 7, this, _data->assetManager.GetTexture("wk")));
    b_pieces.push_back(new Bishop(WHITE, 5, 7, this, _data->assetManager.GetTexture("wb")));
    b_pieces.push_back(new Knight(WHITE, 6, 7, this, _data->assetManager.GetTexture("wn")));
    b_pieces.push_back(new Rook(WHITE, 7, 7, this, _data->assetManager.GetTexture("wr")));
    for(int i = 0; i < 8; i++){
        b_pieces.push_back(new Pawn(WHITE, i, 6, this, DOWN, _data->assetManager.GetTexture("wp")));
    }



    tile = new sf::RectangleShape(sf::Vector2f(75, 75));
    recColor = new sf::Color(245, 90, 105, 128);
    circleColor = new sf::Color(183, 180, 180, 128);

    markedField = new sf::RectangleShape(sf::Vector2f(75, 75));
    circle = new sf::CircleShape(10);
}


void Board::deleteObjects(){
    for(auto& piece : b_pieces){
        delete piece;
    }
    
    delete tile;
    delete markedField;
    delete circle;
    delete recColor;
    delete circleColor;
    //delete lightColor;
    //delete darkColor;
    
    b_pieces.clear();
}

Board::~Board() {
    std::cout << "Usunieto board" << std::endl;
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

bool Board::isTeamPieceAt(int boardX, int boardY, int color) const {
    for (const auto& piece : playerPieces(color)) {
        if (piece->getBoardPosition().x == boardX && piece->getBoardPosition().y == boardY) {
            return true;
        }
    }
    return false;
}

void Board::removePiece(int boardX, int boardY, CapturedPieces* capturedPieces) {
    for (auto it = b_pieces.begin(); it != b_pieces.end(); ) {
        if ((*it) != nullptr && (*it)->getBoardPosition().x == boardX && (*it)->getBoardPosition().y == boardY) {
            if (capturedPieces != nullptr) {
                capturedPieces->AddCapturedPiece((*it)->getTypeName(), (*it)->getColor());
            } 
            
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

void Board::rotatePieces(){
    for(auto& piece : b_pieces){
        piece->rotatePiece();
    }
}

std::vector<Piece*> Board::playerPieces(int color) const{
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
    if(king == nullptr){
        std::cout << "Nie znaleziono krola" << std::endl;
    }
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
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;  // Ignoruj bieżącą pozycję króla

            int newX = kingPosition.x + dx;
            int newY = kingPosition.y + dy;
            
            Coordinate targetPosition(newX, newY);


            // Sprawdź, czy nowe pole jest w granicach planszy
            if (isWithinBounds(newX, newY)) {
                // Sprawdź, czy król może się tam poruszyć bez bycia szachowanym
                if (!isKingInCheckAfterMove(king, targetPosition) && !isTeamPieceAt(newX, newY, color)) {
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
    _pawnPromotion = new PawnPromotion(_data);
    _pawnPromotion->Init(pawn->getBoardPosition().x * 75, 50);
    _pawnPromotion->Draw();
    _pawnPromotion->ChoicePiece();
    delete _pawnPromotion;
    
}


// funkcje odpowiedzialne za rysowanie planszy i bierki itp

void Board::showPossibleMoves(sf::RenderWindow& window, Piece* piece){
    std::vector<Coordinate> possibleMoves = getValidMoves(piece);
    for(auto& move : possibleMoves){
        circle->setFillColor(*circleColor);
        circle->setPosition(move.x * 75 + 28, move.y * 75 + 78);
        window.draw(*circle);
    }
}

void Board::showPossibleCaptures(sf::RenderWindow& window, Piece* piece){
    std::vector<Coordinate> possibleCaptures = getValidCaptures(piece);
    for(auto& capture : possibleCaptures){
        markedField->setFillColor(*recColor);
        markedField->setPosition(capture.x * 75, capture.y * 75 + 50);
        window.draw(*markedField);
    }
}
void Board::showCheck(sf::RenderWindow& window, int color){
    if(isKingInCheck(color)){
        King* king = dynamic_cast<King*>(findKing(color));
        markedField->setFillColor(*recColor);
        markedField->setPosition(king->getBoardPosition().x * 75, king->getBoardPosition().y * 75 + 50);
        window.draw(*markedField);
    }
}

void Board::markPieceField(sf::RenderWindow& window, Piece* piece){
    sf::Color markedColor(223, 223, 103, 128);
    markedField->setFillColor(markedColor);
    markedField->setPosition(piece->getBoardPosition().x * 75, piece->getBoardPosition().y * 75 + 50);
    window.draw(*markedField);
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
    sf::Color *lightColor = new sf::Color(238, 238, 210);
    sf::Color *darkColor = new sf::Color(118, 150, 86);
    

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            
            tile->setPosition(col * 75, row * 75 + 50);

            // Kolorowanie kafelków
            if ((row + col) % 2 == 0) {
                tile->setFillColor(*lightColor); 
            } else {
                tile->setFillColor(*darkColor);
            }
            window.draw(*tile);
        }
    }
    delete lightColor;
    delete darkColor;
}

