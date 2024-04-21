/******************************************************************************************************
 * @file  ChessGame.hpp
 * @brief Definition of the ChessGame class
 ******************************************************************************************************/

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <unordered_map>
#include "Color.hpp"
#include "Rect.hpp"
#include "Point.hpp"

/**
 * @enum ChessPiece
 * @brief Represents the different pieces of a chess game
 */
enum ChessPiece : char {
    ChessPieceKing,
    ChessPieceQueen,
    ChessPieceBishop,
    ChessPieceKnight,
    ChessPieceRook,
    ChessPiecePawn,
    ChessPieceNone
};

/**
 * @enum ChessColor
 * @brief Represents the different colors for the pieces of a chess game
 */
enum ChessColor : char {
    ChessColorWhite,
    ChessColorBlack,
};

/**
 * @struct ChessSquare
 * @brief Represents a square of the chess board
 */
struct ChessSquare {
    ChessSquare() = default;
    ChessSquare(ChessPiece piece, ChessColor color) : piece(piece), color(color) { }
    ChessPiece piece;
    ChessColor color;
};

/**
 * @class ChessGame
 * @brief
 */
class ChessGame {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    TTF_Font* font;
    int fontSize;

    bool stop, fullscreen, turn;
    SDL_Event event;
    std::unordered_map<SDL_Scancode, bool> flags;

    Point winSize;
    Rect boardSurface;
    const RGB whiteSquare, blackSquare;

    SDL_Texture* textures[6][2];
    SDL_Texture* chessBoard;

    ChessSquare board[8][8];

    ChessSquare selectedSquare;
    Point selectedIndex;

    void handleEvents();
    void handleKeyDownEvents();
    void updateBoardSurface();

    void drawBoard() const;
    void drawPieces() const;
    void drawLettersAndNumbers() const;
    void drawUI() const;

    void movePiece();
    
    bool testMoves(const Point& target);
    bool movesKing(const Point& target);
    bool movesQueen(const Point& target);
    bool movesBishop(const Point& target);
    bool movesKnight(const Point& target);
    bool movesRook(const Point& target);
    bool movesPawn(const Point& target);

public:
    ChessGame(unsigned int width, unsigned int height);
    ~ChessGame();

    void run();
};