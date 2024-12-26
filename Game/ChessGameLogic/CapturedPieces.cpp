#include "CapturedPieces.h"


CapturedPieces::CapturedPieces(GameDataRef data): _data(data) {
}

void CapturedPieces::Init() {
    //_background1 = new sf::RectangleShape(sf::Vector2f(200, 700));
    // _background1->setFillColor(sf::Color( 148, 148,148, 128));
    // _background1->setPosition(600, 0);

}

void CapturedPieces::AddCapturedPiece(std::string pieceName, int color) {
    sf::Sprite sprite(_data->assetManager.GetTexture(pieceName));
    sprite.setScale(0.35, 0.35);

    if (color == WHITE) {
        _capturedWhitePieces.push_back(sprite);
    } else {
        _capturedBlackPieces.push_back(sprite);
    }
}


void CapturedPieces::Draw() {
    std::vector<std::pair<std::string, sf::Sprite>> orderedPieces;

    auto addPiecesByType = [&](const std::string& pieceType) {
        for (auto& piece : _capturedWhitePieces) {
            if (piece.getTexture() == &_data->assetManager.GetTexture(pieceType)) {
                orderedPieces.emplace_back(pieceType, piece);
            }
        }
        for(auto& piece : _capturedBlackPieces) {
            if (piece.getTexture() == &_data->assetManager.GetTexture(pieceType)) {
                orderedPieces.emplace_back(pieceType, piece);
            }
        }
    };

    addPiecesByType("wp"); 
    addPiecesByType("wn"); 
    addPiecesByType("wb"); 
    addPiecesByType("wr"); 
    addPiecesByType("wq"); 

    addPiecesByType("bp");
    addPiecesByType("bn");
    addPiecesByType("bb");
    addPiecesByType("br");
    addPiecesByType("bq");

    int x = 40; 
    int y = 23;
    std::string previousType;
    bool firstBlack = false;


    if(whoOnTop == WHITE){
        y = 673;
    }

    for (auto& [pieceType, piece] : orderedPieces) {
        if(pieceType[0] == 'b' and !firstBlack) {
            if(whoOnTop == WHITE){
                x = 40;
                y = 23;
            }
            else{
                x = 40;
                y = 673;
            }
            firstBlack = true;
        }

        if (previousType.empty() || pieceType != previousType) {
            // Jeśli zmiana typu figury, dodaj odstęp 20px
            x += 20;
        } else {
            // Jeśli ten sam typ figury, dodaj odstęp 10px
            x += 10;
        }
        piece.setPosition(x, y);
        _data->window.draw(piece);
        previousType = pieceType;
    }
}


void CapturedPieces::RotateCapturedPieces() {
    whoOnTop = !whoOnTop;
}



CapturedPieces::~CapturedPieces() {
    std::cout << "CapturedPieces destructor" << std::endl;
    //delete _background1;
}