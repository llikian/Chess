/******************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 ******************************************************************************************************/

#include "ChessGame.hpp"

#include <stdexcept>
#include <string>
#include "graphics.hpp"

ChessGame::ChessGame(unsigned int width, unsigned int height)
    : window(), renderer(), font(),
      stop(), fullscreen(), turn(),
      winSize(width, height),
      whiteSquare(255, 225, 205), blackSquare(59, 32, 18),
      selectedSquare(ChessPieceNone, ChessColorWhite) {

    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(std::string("SDL_Init failed : ") + SDL_GetError());
    }

    if(IMG_Init(IMG_INIT_PNG) <= 0) {
        throw std::runtime_error(std::string("IMG_Init failed : ") + SDL_GetError());
    }

    if(TTF_Init() != 0) {
        throw std::runtime_error(std::string("TTF_Init failed : ") + SDL_GetError());
    }

    constexpr int winFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    window = SDL_CreateWindow("Chess Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, winFlags);
    if(!window) {
        throw std::runtime_error(SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) {
        throw std::runtime_error(SDL_GetError());
    }

    font = TTF_OpenFont("data/fonts/courbd.ttf", 20);
    if(!font) {
        throw std::runtime_error(SDL_GetError());
    }

    textures[ChessPieceKing][ChessColorWhite] = createTextureFromSurface(renderer, IMG_Load("data/images/kingW.png"));
    textures[ChessPieceKing][ChessColorBlack] = createTextureFromSurface(renderer, IMG_Load("data/images/kingB.png"));
    textures[ChessPieceQueen][ChessColorWhite] = createTextureFromSurface(renderer, IMG_Load("data/images/queenW.png"));
    textures[ChessPieceQueen][ChessColorBlack] = createTextureFromSurface(renderer, IMG_Load("data/images/queenB.png"));
    textures[ChessPieceBishop][ChessColorWhite] = createTextureFromSurface(renderer, IMG_Load("data/images/bishopW.png"));
    textures[ChessPieceBishop][ChessColorBlack] = createTextureFromSurface(renderer, IMG_Load("data/images/bishopB.png"));
    textures[ChessPieceKnight][ChessColorWhite] = createTextureFromSurface(renderer, IMG_Load("data/images/knightW.png"));
    textures[ChessPieceKnight][ChessColorBlack] = createTextureFromSurface(renderer, IMG_Load("data/images/knightB.png"));
    textures[ChessPieceRook][ChessColorWhite] = createTextureFromSurface(renderer, IMG_Load("data/images/rookW.png"));
    textures[ChessPieceRook][ChessColorBlack] = createTextureFromSurface(renderer, IMG_Load("data/images/rookB.png"));
    textures[ChessPiecePawn][ChessColorWhite] = createTextureFromSurface(renderer, IMG_Load("data/images/pawnW.png"));
    textures[ChessPiecePawn][ChessColorBlack] = createTextureFromSurface(renderer, IMG_Load("data/images/pawnB.png"));

    RGB pixels[8][8];
    for(int i = 0 ; i < 8 ; ++i) {
        for(int j = 0 ; j < 8 ; ++j) {
            pixels[i][j] = ((i + j) % 2) ? blackSquare : whiteSquare;
        }
    }

    Uint32 Rmask, Gmask, Bmask, Amask;
    int bpp;
    SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_RGB24, &bpp, &Rmask, &Gmask, &Bmask, &Amask);

    chessBoard = createTextureFromSurface(renderer, SDL_CreateRGBSurfaceFrom(pixels, 8, 8, bpp, 8 * 3, Rmask, Gmask, Bmask, Amask));

    const ChessPiece boardInit[8]{
        ChessPieceRook, ChessPieceKnight, ChessPieceBishop, ChessPieceQueen,
        ChessPieceKing, ChessPieceBishop, ChessPieceKnight, ChessPieceRook
    };

    for(int i = 0 ; i < 8 ; ++i) {
        for(int j = 0 ; j < 8 ; ++j) {
            switch(i) {
                case 0:
                    board[i][j] = ChessSquare(boardInit[j], ChessColorBlack);
                    break;
                case 1:
                    board[i][j] = ChessSquare(ChessPiecePawn, ChessColorBlack);
                    break;
                case 6:
                    board[i][j] = ChessSquare(ChessPiecePawn, ChessColorWhite);
                    break;
                case 7:
                    board[i][j] = ChessSquare(boardInit[j], ChessColorWhite);
                    break;
                default:
                    board[i][j] = ChessSquare(ChessPieceNone, ChessColorWhite);
                    break;
            }
        }
    }

    updateBoardSurface();
}

ChessGame::~ChessGame() {
    for(auto& piece: textures) {
        for(auto& texture: piece) {
            SDL_DestroyTexture(texture);
        }
    }

    SDL_DestroyTexture(chessBoard);

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void ChessGame::run() {
    while(!stop) {
        handleEvents();

        SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        drawBoard();
        drawPieces();
        drawLettersAndNumbers();
        drawUI();
    }
}

void ChessGame::handleEvents() {
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                stop = true;
            case SDL_KEYUP:
                if(flags.contains(event.key.keysym.scancode)) {
                    flags[event.key.keysym.scancode] = false;
                }
                break;
            case SDL_KEYDOWN:
                handleKeyDownEvents();
                break;
            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SDL_GetWindowSize(window, &winSize.x, &winSize.y);
                    updateBoardSurface();
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                movePiece();
                break;
            default:
                break;
        }
    }
}

void ChessGame::handleKeyDownEvents() {
    switch(event.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
            stop = true;
            break;
        case SDL_SCANCODE_F11:
            if(!flags[SDL_SCANCODE_F11]) {
                SDL_SetWindowFullscreen(window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
                fullscreen = !fullscreen;
                flags[SDL_SCANCODE_F11] = true;
            }
            break;
        default:
            break;
    }
}

void ChessGame::updateBoardSurface() {
    boardSurface.w = std::min(winSize.x, winSize.y) * 0.8f;
    boardSurface.h = boardSurface.w;
    boardSurface.x = (winSize.x - boardSurface.w) / 2;
    boardSurface.y = (winSize.y - boardSurface.h) / 2;

    fontSize = boardSurface.w / 32;
    TTF_SetFontSize(font, fontSize);
}

void ChessGame::drawBoard() const {
    SDL_RenderCopy(renderer, chessBoard, nullptr, &boardSurface);
}

void ChessGame::drawPieces() const {
    SDL_Rect surface = boardSurface;

    surface.w /= 8;
    surface.h /= 8;

    for(int i = 0 ; i < 8 ; ++i) {
        for(const auto& square: board[i]) {
            if(square.piece != ChessPieceNone) {
                SDL_RenderCopy(renderer, textures[square.piece][square.color], nullptr, &surface);
            }

            surface.x += surface.w;
        }

        surface.x = boardSurface.x;
        surface.y += surface.h;
    }

    if(selectedSquare.piece != ChessPieceNone) {
        Point mousePos;
        SDL_GetMouseState(&mousePos.x, &mousePos.y);

        surface.w = boardSurface.w / 8;
        surface.h = surface.w;
        surface.x = mousePos.x - surface.w / 2;
        surface.y = mousePos.y - surface.h / 2;

        SDL_RenderCopy(renderer, textures[selectedSquare.piece][selectedSquare.color], nullptr, &surface);
    }
}

void ChessGame::drawLettersAndNumbers() const {
    int squareSize = boardSurface.w / 8;
    Color color(255, 255, 255);
    Rect rect;
    std::string letters = "ABCDEFGH";

    for(int i = 0 ; i < 8 ; ++i) {
        rect = Rect(boardSurface.x - squareSize + squareSize / 2, boardSurface.y + squareSize * i, squareSize - squareSize / 2, squareSize);
        renderCenteredText(renderer, font, rect, color, std::to_string(8 - i));

        rect = Rect(boardSurface.x + squareSize * i, boardSurface.y + squareSize * 8, squareSize, squareSize - squareSize / 2);
        renderCenteredText(renderer, font, rect, color, letters.substr(i, 1));
    }
}

void ChessGame::drawUI() const {
    Color textColor;
    std::string text("It is ");

    if(turn == ChessColorWhite) {
        SDL_SetRenderDrawColor(renderer, whiteSquare.r, whiteSquare.g, whiteSquare.b, 255);
        textColor = blackSquare;
        text += "White's turn !";
    } else {
        SDL_SetRenderDrawColor(renderer, blackSquare.r, blackSquare.g, blackSquare.b, 255);
        textColor = whiteSquare;
        text += "Black's turn !";
    }

    Rect rect(boardSurface.x, boardSurface.y * 0.2f, boardSurface.w, boardSurface.y * 0.6f);
    SDL_RenderFillRect(renderer, &rect);

    TTF_SetFontSize(font, 2 * fontSize);
    renderCenteredText(renderer, font, rect, textColor, text);
    TTF_SetFontSize(font, fontSize);
}

void ChessGame::movePiece() {
    Point mousePos, indexes;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);

    if(event.button.state == SDL_PRESSED && event.button.button == SDL_BUTTON_LEFT) {
        indexes.x = (mousePos.y - boardSurface.y) / (boardSurface.w / 8);
        indexes.y = (mousePos.x - boardSurface.x) / (boardSurface.w / 8);

        if(indexes.x < 0 || indexes.x >= 8 || indexes.y < 0 || indexes.y >= 8) {
            return;
        }

        if(selectedSquare.piece == ChessPieceNone) {
            if(board[indexes.x][indexes.y].color != turn) {
                return;
            }

            selectedSquare = board[indexes.x][indexes.y];
            selectedIndex = indexes;

            board[indexes.x][indexes.y].piece = ChessPieceNone;
            board[indexes.x][indexes.y].color = ChessColorWhite;
        } else {
            if(board[indexes.x][indexes.y].color == turn && board[indexes.x][indexes.y].piece != ChessPieceNone) {
                return;
            } else if(!testMoves(indexes)) {
                board[selectedIndex.x][selectedIndex.y] = selectedSquare;
                selectedSquare = ChessSquare(ChessPieceNone, ChessColorWhite);
                return;
            }

            board[indexes.x][indexes.y] = selectedSquare;

            selectedSquare.piece = ChessPieceNone;
            selectedSquare.color = ChessColorWhite;

            if(indexes.x != selectedIndex.x || indexes.y != selectedIndex.y) {
                turn = turn ? ChessColorWhite : ChessColorBlack;
            }
        }
    }
}

bool ChessGame::testMoves(const Point& target) {
    switch(selectedSquare.piece) {
        case ChessPieceKing:
            return movesKing(target);
        case ChessPieceQueen:
            return movesQueen(target);
        case ChessPieceBishop:
            return movesBishop(target);
        case ChessPieceKnight:
            return movesKnight(target);
        case ChessPieceRook:
            return movesRook(target);
        case ChessPiecePawn:
            if(movesPawn(target)) {
                if((selectedSquare.color == ChessColorWhite && target.x == 0) || (selectedSquare.color == ChessColorBlack && target.x == 7)) {
                    selectedSquare.piece = ChessPieceQueen;
                }
                return true;
            }
            return false;
        default:
            return false;
    }
}

bool ChessGame::movesKing(const Point& target) {
    Point moves[]{Point(-1, -1),
                  Point(-1, 0),
                  Point(-1, 1),
                  Point(0, -1),
                  Point(1, -1),
                  Point(1, 0),
                  Point(1, 1),
                  Point(0, 1)};

    for(const Point& move: moves) {
        if((selectedIndex.x + move.x) == target.x && (selectedIndex.y + move.y) == target.y) {
            return true;
        }
    }

    return false;
}

bool ChessGame::movesQueen(const Point& target) {
    return movesBishop(target) || movesRook(target);
}

bool ChessGame::movesBishop(const Point& target) {
    unsigned int factor;
    Point index;
    Point moves[]{Point(-1, -1),
                  Point(-1, 1),
                  Point(1, -1),
                  Point(1, 1)};

    for(const Point& move: moves) {
        factor = 1;

        index.x = selectedIndex.x + move.x * factor;
        index.y = selectedIndex.y + move.y * factor;

        do {
            index.x = selectedIndex.x + move.x * factor;
            index.y = selectedIndex.y + move.y * factor;

            if(index.x == target.x && index.y == target.y) {
                return true;
            }

            ++factor;
        } while(index.x >= 0 && index.x < 8 && index.y >= 0 && index.y < 8 && board[index.x][index.y].piece == ChessPieceNone);

        if(index.x == target.x && index.y == target.y) {
            return true;
        }
    }

    return false;
}

bool ChessGame::movesKnight(const Point& target) {
    Point moves[8]{Point(-2, -1),
                   Point(-2, 1),
                   Point(-1, -2),
                   Point(-1, 2),
                   Point(1, -2),
                   Point(1, 2),
                   Point(2, -1),
                   Point(2, 1)};

    for(const Point& move: moves) {
        if((selectedIndex.x + move.x) == target.x && (selectedIndex.y + move.y) == target.y) {
            return true;
        }
    }

    return false;
}

bool ChessGame::movesRook(const Point& target) {
    unsigned int factor;
    Point index;
    Point moves[]{Point(-1, 0),
                  Point(1, 0),
                  Point(0, -1),
                  Point(0, 1)};

    for(const Point& move: moves) {
        factor = 1;

        do {
            index.x = selectedIndex.x + move.x * factor;
            index.y = selectedIndex.y + move.y * factor;

            if(index.x == target.x && index.y == target.y) {
                return true;
            }

            ++factor;
        } while(index.x >= 0 && index.x < 8 && index.y >= 0 && index.y < 8 && board[index.x][index.y].piece == ChessPieceNone);

        if(index.x == target.x && index.y == target.y) {
            return true;
        }
    }

    return false;
}

bool ChessGame::movesPawn(const Point& target) {
    Point index;
    Point moves[](Point(turn ? 1 : -1, 0),
                  Point(turn ? 1 : -1, -1),
                  Point(turn ? 1 : -1, 1),
                  Point(turn ? 2 : -2, 0));


    for(int i = 0 ; i < 4 ; ++i) {
        index.x = selectedIndex.x + moves[i].x;
        index.y = selectedIndex.y + moves[i].y;

        if(index.x == target.x && index.y == target.y) {
            switch(i) {
                case 0:
                    return board[index.x][index.y].piece == ChessPieceNone;
                case 1:
                case 2:
                    return board[index.x][index.y].piece != ChessPieceNone;
                case 3:
                    return (turn == ChessColorWhite && selectedIndex.x == 6) || (turn == ChessColorBlack && selectedIndex.x == 1);
            }
        }
    }

    return false;
}