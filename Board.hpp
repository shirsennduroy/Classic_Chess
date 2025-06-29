#include "TextureManager.hpp" //new third

#ifndef BOARD_HPP
#define BOARD_HPP // AS SECOND

static int Black_King_positionAtRow = 4;
static int Black_King_positionAtCol = 7;
static int White_King_positionAtRow = 4;
static int White_King_positionAtCol = 0;
class Board
{
private:
    SDL_Renderer *renderer;
    SDL_Rect tile;

    std::vector<std::vector<std::string>> board;
    // std::vector<std::vector<int>> Black_king_Surrounding;
    // std::vector<std::vector<int>> White_king_Surrounding;

    const int tileSize = 120;
    // each square is 110x110 px

    std::map<std::string /*as index*/, SDL_Texture *> pieceTextures;
    // Pawn *pawn;

    bool pieceSelected = false;
    int selectedRow = -1; // new fourth
    int selectedCol = -1; // new fourth

    void loadPieceTextures();              // one time used
    void setupBoard();                     // one time used
    // void update_Black_kings_Surrounding(); // one time used
    // void update_White_kings_Surrounding(); // one time used

public:
    Board(SDL_Renderer *renderer);

    void draw();
    void handleClick(int mouseX, int mouseY); // new fourth for mouse controlling the
    void drawPieces();
    void movePiece(int targetRow, int targetCol); // new 5th
    bool isWhite(int RowPos, int ColPos);
    bool isBlack(int RowPos, int ColPos);

    // bool isStrPieaceGoingToAttack(int ,int, std::string);
    // bool ifPieceDominateCrossPath(int, int, char);
    // bool ifPieceDominatePlusPath(int ,int ,char);

    bool AnyPieaceAtPlusPath(int, int);
    bool AnyPieaceAtRoundPath(int, int);

    bool KnightPath(int, int);

    bool AnyPieaceAtCrossPath(int, int);
    bool QueenPath(int, int);
    void function(int targetRow, int targetCol);
    // bool is_the_grid_danger(int, int, char);
};

#endif
