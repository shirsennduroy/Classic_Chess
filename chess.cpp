#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <cmath>

#define MIXER_FLAGS MIX_INIT_MP3
#define IMG_Flag IMG_INIT_PNG
#define TITLE "CLASSIC_CHESS_GAME"
const int WIDTH = 1560;
const int HEIGHT = 960;

enum TURN
{
    WHITEs_TURN,
    BLACKs_TURN
};
static int Black_King_positionAtRow;
static int Black_King_positionAtCol;
static int White_King_positionAtRow;
static int White_King_positionAtCol;

class Board
{
public:
    SDL_Renderer *renderer;
    SDL_Rect tile;
    bool White_s_Turn, IsWhiteKingHaveCastlingChance, IsBlackKingHaveCastlingChance, Casting_WK_with_UpRook, Casting_WK_with_DownRook, Casting_BK_with_UpRook, Casting_BK_with_DownRook;
    bool nowYouHave_wk_caslingWithUpWR, nowYouHave_wk_caslingWithDownWR, nowYouHave_bk_caslingWithUpBR, nowYouHave_bk_caslingWithDownBR;
    Mix_Music *MoveSound;

    std::vector<std::vector<bool>> Black_king_Surrounding;
    std::vector<std::vector<bool>> White_king_Surrounding;

    const int TileSize = 120;
    // each square is 120x120 px

    std::map<std::string /*as index*/, SDL_Texture *> pieceTextures;
    // Pawn *pawn;

    void loadPieceTextures(); // one time used
    bool pieceSelected;       // It's needed or result fall segment coredump
    int selectedRow = -1;     // new fourth
    int selectedCol = -1;     // new fourth
    std::vector<std::vector<std::string>> board;
    Board(SDL_Renderer *, Mix_Music *);
    // ~Board();
    void setupBoard(); // one time used
    void draw();
    void handleClick(int, int); // new fourth for mouse controlling the
    void drawPieces();
    bool movePiece(int, int); // new 5th
    bool isWhite(int, int);
    bool isBlack(int, int);
    bool AnyPieaceAtPlusPath(int, int);
    bool AnyPieaceAtRoundPath(int, int);
    bool KnightPath(int, int);
    bool AnyPieaceAtCrossPath(int, int);
    bool QueenPath(int, int);
    bool function(int, int);
    bool ifCaslingPossible(char, bool *, bool *);
    bool CanBlackAttackThisGrid(int, int);
    bool CanWhiteAttackThisGrid(int, int);
    void ShowPosibilitiesGrid();
    void DrawSquereRing(int, int, char);
    void DrawKnightPath(int, int);
    void DrawRookPath(int, int);
    void DrawBishopPath(int, int);
    void DrawQueenPath(int, int);
    void DrawKingPath(int, int);
    void DrawRing(int, int, char);
};

class Game
{
public:
    Game();
    ~Game();
    bool isInitOkey(const char *, int, int);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool isRunning() const;
    void rest_for_a_little();
    void necessaryFunction(const char *);

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    SDL_Texture *side_text;
    TTF_Font *font;
    SDL_Rect Text_Rect;
    Mix_Music *MoveSound;
    bool running;
    int x, y;

    Board *BoardClass; // new second
};

void Board::loadPieceTextures() //---------------2.4.2
{
    // std::string base = "Pieces/";
    std::string name[12] = {"wp", "wr", "wn", "wb", "wq", "wk", "bp", "br", "bn", "bb", "bq", "bk"};

    for (int i = 0; i < 12; i++)
    {
        std::string temp = "Pieces/", png = ".png";
        temp += (name[i] + png);
        pieceTextures[name[i]] = IMG_LoadTexture(renderer, temp.c_str()); //----------------------2.4.2.1
    }
}

Board::Board(SDL_Renderer *rend, Mix_Music *Sound) : renderer(rend), board(8, std::vector<std::string>(8)), MoveSound(Sound), Black_king_Surrounding(3, std::vector<bool>(3)), White_king_Surrounding(3, std::vector<bool>(3)) //----------------2.4.1
{
    loadPieceTextures(); //----------------2.4.2
    setupBoard();        //----------------2.4.3
}

bool Board::movePiece(int targetRow, int targetCol) //-------------4.4.3_II_1
{
    if (selectedRow == targetRow && selectedCol == targetCol)
        return true;

    bool check = function(targetRow, targetCol); //-------------------4.4.3_II_2

    if (!check)
    {
        Mix_PlayMusic(MoveSound, 0);
        std::cout << "## Moved to : [" << targetRow << " , " << targetCol << "]" << std::endl; //-------------------4.4.3_II_3
    }
    return check;
}

void Board::handleClick(int mouseX, int mouseY) //------------4.4.1
{
    int row = mouseY / TileSize;
    int col = mouseX / TileSize;
    if (row > 7 || col > 7)
        return;

    if (!pieceSelected) //--------------this is clever
    {
        if (board[row][col] != "\'")
        {
            if (White_s_Turn == WHITEs_TURN && board[row][col][0] == 'w')
            {
                pieceSelected = true;
                selectedRow = row;
                selectedCol = col;
                std::cout << "# Selected: \"" << board[row][col] << "\" is at : [" << row << " , " << col << "]" << std::endl; //------------------4.4.3I
            }
            else if (White_s_Turn == BLACKs_TURN && board[row][col][0] == 'b')
            {
                pieceSelected = true;
                selectedRow = row;
                selectedCol = col;
                std::cout << "# Selected: \"" << board[row][col] << "\" is at : [" << row << " , " << col << "]" << std::endl; //------------------4.4.3I
            }
        }
    }
    else
    {
        bool check = movePiece(row, col); //--------------------4.4.3_II
        selectedRow = -1;
        selectedCol = -1; // for reseting all again
        pieceSelected = false;

        if (!check)
            White_s_Turn = !White_s_Turn;
        else
            std::cout << "No turning !\n";
    }
}

void Board::setupBoard() //--------------------2.4.3
{
    for (int j = 2; j < 6; ++j)
        for (int i = 0; i < 8; ++i)
            board[i][j] = "\'";

    // pawns
    for (int i = 0; i < 8; i++)
    {
        board[i][1] = "wp";
        board[i][6] = "bp";
    }

    // setting rooks
    board[0][0] = board[7][0] = "wr";
    board[0][7] = board[7][7] = "br";

    board[1][0] = board[6][0] = "wn";
    board[1][7] = board[6][7] = "bn";
    // setting knight

    board[2][0] = board[5][0] = "wb";
    board[2][7] = board[5][7] = "bb";
    // set the Bishop

    board[3][0] = "wq";
    board[3][7] = "bq";
    // setting the queen or minister

    board[4][0] = "wk";
    board[4][7] = "bk";
    // setting the king

    Black_King_positionAtRow = 4;
    Black_King_positionAtCol = 7;
    White_King_positionAtRow = 4;
    White_King_positionAtCol = 0;
    White_s_Turn = WHITEs_TURN;
    pieceSelected = false;
    selectedCol = -1;
    selectedRow = -1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            Black_king_Surrounding[i][j] =
                White_king_Surrounding[i][j] =
                    false;

    Black_king_Surrounding[1][1] =
        White_king_Surrounding[1][1] =
            Casting_BK_with_DownRook =
                Casting_BK_with_UpRook =
                    Casting_WK_with_DownRook =
                        Casting_WK_with_UpRook =
                            IsBlackKingHaveCastlingChance =
                                IsWhiteKingHaveCastlingChance =
                                    true;
    nowYouHave_bk_caslingWithDownBR =
        nowYouHave_bk_caslingWithUpBR =
            nowYouHave_wk_caslingWithDownWR =
                nowYouHave_wk_caslingWithUpWR = false;
    system("clear");
}

// In SDL_Rect struct data , x and y is the top left position of the rectangle and w & h is weight and height of the rectangle

void Board::DrawKnightPath(int Row, int Col)
{
    if (White_s_Turn == WHITEs_TURN)
    {
        if (isBlack(Row + 1, Col + 2))
            DrawSquereRing(Row + 1, Col + 2, 'r');
        else if (!isWhite(Row + 1, Col + 2))
            DrawSquereRing(Row + 1, Col + 2, 'g');
        if (isBlack(Row + 2, Col + 1))
            DrawSquereRing(Row + 2, Col + 1, 'r');
        else if (!isWhite(Row + 2, Col + 1))
            DrawSquereRing(Row + 2, Col + 1, 'g');
        if (isBlack(Row - 1, Col + 2))
            DrawSquereRing(Row - 1, Col + 2, 'r');
        else if (!isWhite(Row - 1, Col + 2))
            DrawSquereRing(Row - 1, Col + 2, 'g');
        if (isBlack(Row - 2, Col + 1))
            DrawSquereRing(Row - 2, Col + 1, 'r');
        else if (!isWhite(Row - 2, Col + 1))
            DrawSquereRing(Row - 2, Col + 1, 'g');
        if (isBlack(Row - 2, Col - 1))
            DrawSquereRing(Row - 2, Col - 1, 'r');
        else if (!isWhite(Row - 2, Col - 1))
            DrawSquereRing(Row - 2, Col - 1, 'g');
        if (isBlack(Row - 1, Col - 2))
            DrawSquereRing(Row - 1, Col - 2, 'r');
        else if (!isWhite(Row - 1, Col - 2))
            DrawSquereRing(Row - 1, Col - 2, 'g');
        if (isBlack(Row + 1, Col - 2))
            DrawSquereRing(Row + 1, Col - 2, 'r');
        else if (!isWhite(Row + 1, Col - 2))
            DrawSquereRing(Row + 1, Col - 2, 'g');
        if (isBlack(Row + 2, Col - 1))
            DrawSquereRing(Row + 2, Col - 1, 'r');
        else if (!isWhite(Row + 2, Col - 1))
            DrawSquereRing(Row + 2, Col - 1, 'g');
    }
    else
    {
        if (isWhite(Row + 1, Col + 2))
            DrawSquereRing(Row + 1, Col + 2, 'r');
        else if (!isBlack(Row + 1, Col + 2))
            DrawSquereRing(Row + 1, Col + 2, 'g');
        if (isWhite(Row + 2, Col + 1))
            DrawSquereRing(Row + 2, Col + 1, 'r');
        else if (!isBlack(Row + 2, Col + 1))
            DrawSquereRing(Row + 2, Col + 1, 'g');
        if (isWhite(Row - 1, Col + 2))
            DrawSquereRing(Row - 1, Col + 2, 'r');
        else if (!isBlack(Row - 1, Col + 2))
            DrawSquereRing(Row - 1, Col + 2, 'g');
        if (isWhite(Row - 2, Col + 1))
            DrawSquereRing(Row - 2, Col + 1, 'r');
        else if (!isBlack(Row - 2, Col + 1))
            DrawSquereRing(Row - 2, Col + 1, 'g');
        if (isWhite(Row - 2, Col - 1))
            DrawSquereRing(Row - 2, Col - 1, 'r');
        else if (!isBlack(Row - 2, Col - 1))
            DrawSquereRing(Row - 2, Col - 1, 'g');
        if (isWhite(Row - 1, Col - 2))
            DrawSquereRing(Row - 1, Col - 2, 'r');
        else if (!isBlack(Row - 1, Col - 2))
            DrawSquereRing(Row - 1, Col - 2, 'g');
        if (isWhite(Row + 1, Col - 2))
            DrawSquereRing(Row + 1, Col - 2, 'r');
        else if (!isBlack(Row + 1, Col - 2))
            DrawSquereRing(Row + 1, Col - 2, 'g');
        if (isWhite(Row + 2, Col - 1))
            DrawSquereRing(Row + 2, Col - 1, 'r');
        else if (!isBlack(Row + 2, Col - 1))
            DrawSquereRing(Row + 2, Col - 1, 'g');
    }
}
void Board::DrawRookPath(int Row, int Col)
{
    if (White_s_Turn == WHITEs_TURN)
    {
        for (int i = Col + 1; i < 8; i++)
        {
            if (board[Row][i] == "\'")
                DrawSquereRing(Row, i, 'g');
            else
            {
                if (board[Row][i][0] == 'b')
                    DrawSquereRing(Row, i, 'r');
                break;
            }
        }
        for (int i = Col - 1; i >= 0; i--)
        {
            if (board[Row][i] == "\'")
                DrawSquereRing(Row, i, 'g');
            else
            {
                if (board[Row][i][0] == 'b')
                    DrawSquereRing(Row, i, 'r');
                break;
            }
        }
        for (int i = Row + 1; i < 8; i++)
        {
            if (board[i][Col] == "\'")
                DrawSquereRing(i, Col, 'g');
            else
            {
                if (board[i][Col][0] == 'b')
                    DrawSquereRing(i, Col, 'r');
                break;
            }
        }
        for (int i = Row - 1; i >= 0; i--)
        {
            if (board[i][Col] == "\'")
                DrawSquereRing(i, Col, 'g');
            else
            {
                if (board[i][Col][0] == 'b')
                    DrawSquereRing(i, Col, 'r');
                break;
            }
        }
    }
    else
    {
        for (int i = Col + 1; i < 8; i++)
        {
            if (board[Row][i] == "\'")
            {
                DrawSquereRing(Row, i, 'g');
            }
            else
            {
                if (board[Row][i][0] == 'w')
                    DrawSquereRing(Row, i, 'r');
                break;
            }
        }
        for (int i = Col - 1; i >= 0; i--)
        {
            if (board[Row][i] == "\'")
            {
                DrawSquereRing(Row, i, 'g');
            }
            else
            {
                if (board[Row][i][0] == 'w')
                    DrawSquereRing(Row, i, 'r');
                break;
            }
        }
        for (int i = Row + 1; i < 8; i++)
        {
            if (board[i][Col] == "\'")
            {
                DrawSquereRing(i, Col, 'g');
            }
            else
            {
                if (board[i][Col][0] == 'w')
                    DrawSquereRing(i, Col, 'r');
                break;
            }
        }
        for (int i = Row - 1; i >= 0; i--)
        {
            if (board[i][Col] == "\'")
            {
                DrawSquereRing(i, Col, 'g');
            }
            else
            {
                if (board[i][Col][0] == 'w')
                    DrawSquereRing(i, Col, 'r');
                break;
            }
        }
    }
}
void Board::DrawBishopPath(int Row, int Col)
{
    if (White_s_Turn == WHITEs_TURN)
    {
        for (int j = Col + 1, i = Row + 1; i < 8 && j < 8; i++, j++)
        {
            if (board[i][j] == "\'")
            {
                DrawSquereRing(i, j, 'g');
            }
            else
            {
                if (board[i][j][0] == 'b')
                    DrawSquereRing(i, j, 'r');
                break;
            }
        }
        for (int j = Col - 1, i = Row + 1; i < 8 && j >= 0; i++, j--)
        {
            if (board[i][j] == "\'")
            {
                DrawSquereRing(i, j, 'g');
            }
            else
            {
                if (board[i][j][0] == 'b')
                    DrawSquereRing(i, j, 'r');
                break;
            }
        }
        for (int j = Col - 1, i = Row - 1; i >= 0 && j >= 0; i--, j--)
        {
            if (board[i][j] == "\'")
            {
                DrawSquereRing(i, j, 'g');
            }
            else
            {
                if (board[i][j][0] == 'b')
                    DrawSquereRing(i, j, 'r');
                break;
            }
        }
        for (int j = Col + 1, i = Row - 1; i >= 0 && j < 8; i--, j++)
        {
            if (board[i][j] == "\'")
            {
                DrawSquereRing(i, j, 'g');
            }
            else
            {
                if (board[i][j][0] == 'b')
                    DrawSquereRing(i, j, 'r');
                break;
            }
        }
    }
    else
    {
        for (int j = Col + 1, i = Row + 1; i < 8 && j < 8; i++, j++)
        {
            if (board[i][j] == "\'")
            {
                DrawSquereRing(i, j, 'g');
            }
            else
            {
                if (board[i][j][0] == 'w')
                    DrawSquereRing(i, j, 'r');
                break;
            }
        }
        for (int j = Col - 1, i = Row + 1; i < 8 && j >= 0; i++, j--)
        {
            if (board[i][j] == "\'")
            {
                DrawSquereRing(i, j, 'g');
            }
            else
            {
                if (board[i][j][0] == 'w')
                    DrawSquereRing(i, j, 'r');
                break;
            }
        }
        for (int j = Col - 1, i = Row - 1; i >= 0 && j >= 0; i--, j--)
        {
            if (board[i][j] == "\'")
            {
                DrawSquereRing(i, j, 'g');
            }
            else
            {
                if (board[i][j][0] == 'w')
                    DrawSquereRing(i, j, 'r');
                break;
            }
        }
        for (int j = Col + 1, i = Row - 1; i >= 0 && j < 8; i--, j++)
        {
            if (board[i][j] == "\'")
            {
                DrawSquereRing(i, j, 'g');
            }
            else
            {
                if (board[i][j][0] == 'w')
                    DrawSquereRing(i, j, 'r');
                break;
            }
        }
    }
}
void Board::DrawQueenPath(int Row, int Col)
{
    DrawBishopPath(Row, Col);
    DrawRookPath(Row, Col);
}
void Board::DrawKingPath(int Row, int Col)
{
    if (White_s_Turn == WHITEs_TURN)
    {
        if (isBlack(Row + 1, Col + 1))
            DrawSquereRing(Row + 1, Col + 1, 'r');
        else if (!isWhite(Row + 1, Col + 1))
            DrawSquereRing(Row + 1, Col + 1, 'g');
        if (isBlack(Row, Col + 1))
            DrawSquereRing(Row, Col + 1, 'r');
        else if (!isWhite(Row, Col + 1))
            DrawSquereRing(Row, Col + 1, 'g');
        if (isBlack(Row - 1, Col + 1))
            DrawSquereRing(Row - 1, Col + 1, 'r');
        else if (!isWhite(Row - 1, Col + 1))
            DrawSquereRing(Row - 1, Col + 1, 'g');
        if (isBlack(Row - 1, Col))
            DrawSquereRing(Row - 1, Col, 'r');
        else if (!isWhite(Row - 1, Col))
            DrawSquereRing(Row - 1, Col, 'g');
        if (isBlack(Row - 1, Col - 1))
            DrawSquereRing(Row - 1, Col - 1, 'r');
        else if (!isWhite(Row - 1, Col - 1))
            DrawSquereRing(Row - 1, Col - 1, 'g');
        if (isBlack(Row, Col - 1))
            DrawSquereRing(Row, Col - 1, 'r');
        else if (!isWhite(Row, Col - 1))
            DrawSquereRing(Row, Col - 1, 'g');
        if (isBlack(Row + 1, Col - 1))
            DrawSquereRing(Row + 1, Col - 1, 'r');
        else if (!isWhite(Row + 1, Col - 1))
            DrawSquereRing(Row + 1, Col - 1, 'g');
        if (isBlack(Row + 1, Col))
            DrawSquereRing(Row + 1, Col, 'r');
        else if (!isWhite(Row + 1, Col))
            DrawSquereRing(Row + 1, Col, 'g');
    }
    else
    {
        if (isWhite(Row + 1, Col + 1))
            DrawSquereRing(Row + 1, Col + 1, 'r');
        else if (!isBlack(Row + 1, Col + 1))
            DrawSquereRing(Row + 1, Col + 1, 'g');
        if (isWhite(Row, Col + 1))
            DrawSquereRing(Row, Col + 1, 'r');
        else if (!isBlack(Row, Col + 1))
            DrawSquereRing(Row, Col + 1, 'g');
        if (isWhite(Row - 1, Col + 1))
            DrawSquereRing(Row - 1, Col + 1, 'r');
        else if (!isBlack(Row - 1, Col + 1))
            DrawSquereRing(Row - 1, Col + 1, 'g');
        if (isWhite(Row - 1, Col))
            DrawSquereRing(Row - 1, Col, 'r');
        else if (!isBlack(Row - 1, Col))
            DrawSquereRing(Row - 1, Col, 'g');
        if (isWhite(Row - 1, Col - 1))
            DrawSquereRing(Row - 1, Col - 1, 'r');
        else if (!isBlack(Row - 1, Col - 1))
            DrawSquereRing(Row - 1, Col - 1, 'g');
        if (isWhite(Row, Col - 1))
            DrawSquereRing(Row, Col - 1, 'r');
        else if (!isBlack(Row, Col - 1))
            DrawSquereRing(Row, Col - 1, 'g');
        if (isWhite(Row + 1, Col - 1))
            DrawSquereRing(Row + 1, Col - 1, 'r');
        else if (!isBlack(Row + 1, Col - 1))
            DrawSquereRing(Row + 1, Col - 1, 'g');
        if (isWhite(Row + 1, Col))
            DrawSquereRing(Row + 1, Col, 'r');
        else if (!isBlack(Row + 1, Col))
            DrawSquereRing(Row + 1, Col, 'g');
    }
}
void Board::DrawSquereRing(int Row, int Colunm, char ch)
{
    if (Row < 0 || Row > 7 || Colunm < 0 || Colunm > 7)
        return;
    SDL_Rect rect1 = {Colunm * TileSize + 6, Row * TileSize + 6, 108, 6};
    SDL_Rect rect2 = {Colunm * TileSize + 6, Row * TileSize + 12, 6, 96};
    SDL_Rect rect3 = {Colunm * TileSize + 108, Row * TileSize + 12, 6, 96};
    SDL_Rect rect4 = {Colunm * TileSize + 6, Row * TileSize + 108, 108, 6};

    if (ch == 'r')
    {
        SDL_SetRenderDrawColor(renderer, 255, 10, 10, 0xff);
        SDL_RenderFillRect(renderer, &rect1);
        SDL_SetRenderDrawColor(renderer, 220, 10, 10, 0xff);
        SDL_RenderFillRect(renderer, &rect2);
        SDL_SetRenderDrawColor(renderer, 200, 10, 10, 0xff);
        SDL_RenderFillRect(renderer, &rect3);
        SDL_SetRenderDrawColor(renderer, 170, 10, 10, 0xff);
        SDL_RenderFillRect(renderer, &rect4);
    }
    else if (ch == 'g')
    {
        SDL_SetRenderDrawColor(renderer, 10, 255, 10, 0xff);
        SDL_RenderFillRect(renderer, &rect1);
        SDL_SetRenderDrawColor(renderer, 10, 220, 10, 0xff);
        SDL_RenderFillRect(renderer, &rect2);
        SDL_SetRenderDrawColor(renderer, 10, 200, 10, 0xff);
        SDL_RenderFillRect(renderer, &rect3);
        SDL_SetRenderDrawColor(renderer, 10, 170, 10, 0xff);
        SDL_RenderFillRect(renderer, &rect4);
    }
    else if (ch == 'b')
    {
        SDL_SetRenderDrawColor(renderer, 10, 10, 225, 0xff);
        SDL_RenderFillRect(renderer, &rect1);
        SDL_SetRenderDrawColor(renderer, 10, 10, 190, 0xff);
        SDL_RenderFillRect(renderer, &rect2);
        SDL_SetRenderDrawColor(renderer, 10, 10, 170, 0xff);
        SDL_RenderFillRect(renderer, &rect3);
        SDL_SetRenderDrawColor(renderer, 10, 10, 140, 0xff);
        SDL_RenderFillRect(renderer, &rect4);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 10, 10, 170, 0xff);
        SDL_RenderFillRect(renderer, &rect1);
        SDL_SetRenderDrawColor(renderer, 10, 10, 140, 0xff);
        SDL_RenderFillRect(renderer, &rect2);
        SDL_SetRenderDrawColor(renderer, 10, 10, 100, 0xff);
        SDL_RenderFillRect(renderer, &rect3);
        SDL_SetRenderDrawColor(renderer, 10, 10, 90, 0xff);
        SDL_RenderFillRect(renderer, &rect4);
    }
}

void Board::draw() // called from game ------------------6.2.1 render
{
    SDL_Color light{0xea, 0xea, 0xea, 0xff}; // Light tile color
    SDL_Color dark{0x40, 0x50, 0x50, 0xff};  // Dark tile color
    bool isDark;

    SDL_Rect Side{960, 0, 600, 960};
    SDL_SetRenderDrawColor(renderer, 0, 100, 103, 255);
    SDL_RenderFillRect(renderer, &Side);

    for (int row{0}; row < 8; ++row)
        for (int col{0}; col < 8; ++col)
        {
            tile.x = col * TileSize;
            tile.y = row * TileSize;
            tile.w = tile.h = TileSize;

            isDark = (row + col) & 1 ? 0 : 1;

            SDL_SetRenderDrawColor(renderer,
                                   isDark ? dark.r : light.r,
                                   isDark ? dark.g : light.g,
                                   isDark ? dark.b : light.b,
                                   0xff);        // -----------just making grids ------6.2.2
            SDL_RenderFillRect(renderer, &tile); //------------------6.2.3
            if (isDark)
            {
                tile.x += 5;
                tile.y += 5;
                tile.h -= 10;
                tile.w -= 10;
                SDL_SetRenderDrawColor(renderer,
                                       dark.r + 0x7,
                                       dark.g + 0x7,
                                       dark.b + 0x7,
                                       0xff);
                SDL_RenderFillRect(renderer, &tile);
                tile.x += 5;
                tile.y += 5;
                tile.h -= 10;
                tile.w -= 10;
                SDL_SetRenderDrawColor(renderer,
                                       dark.r + 0xf,
                                       dark.g + 0xf,
                                       dark.b + 0xf,
                                       0xff);
                SDL_RenderFillRect(renderer, &tile);
            }
            else
            {
                tile.x += 5;
                tile.y += 5;
                tile.h -= 10;
                tile.w -= 10;
                SDL_SetRenderDrawColor(renderer,
                                       light.r + 0xa,
                                       light.g + 0xa,
                                       light.b + 0xa,
                                       0xff);
                SDL_RenderFillRect(renderer, &tile);
                tile.x += 5;
                tile.y += 5;
                tile.h -= 10;
                tile.w -= 10;
                SDL_SetRenderDrawColor(renderer,
                                       light.r + 0x14,
                                       light.g + 0x14,
                                       light.b + 0x14,
                                       0xff);
                SDL_RenderFillRect(renderer, &tile);
            }
            if (selectedRow >= 0 && selectedCol >= 0) // so I can size the whole grid
            {
                // if(isPawn(row,col)){highlight_paths("")}
                // I will hight light the selected pieces
                SDL_Rect highlight; // for makng rect
                highlight.x = selectedCol * TileSize;
                highlight.y = selectedRow * TileSize;
                highlight.w = TileSize;
                highlight.h = TileSize;
                /*selectedRow and selectedCol are in "Board.hpp"*/
                SDL_SetRenderDrawColor(renderer, 90, 220, 123, 255); // Green translucent-----------------------6.2.4

                // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                // Showing the alpha feature

                SDL_RenderFillRect(renderer, &highlight); //-----------------6.2.5
                ShowPosibilitiesGrid();                   // the Another Critical function
            }
        }

    drawPieces(); // new third---------------------6.2.6!
    // calling the drawPieces
}

void Board::drawPieces() // ------------------- 6.2.6.1
{
    SDL_Rect dest;
    for (int row = 0; row < 8; ++row)
    { //                         ------------6.2.6.2 ----setting all pieces
        for (int col = 0; col < 8; ++col)
        {
            if (board[row][col] == "\'")
                continue;

            dest.x = col * TileSize; //-----------------6.2.6.3 ------
            dest.y = row * TileSize;
            dest.w = dest.h = TileSize;

            SDL_RenderCopy(renderer, pieceTextures[board[row][col]], nullptr, &dest); //-----------------------6.2.6.4-----
        }
    }
}

// here to ----------------------------------------

void Board::DrawRing(int Row, int Column, char ch)
{
    if (Row < 0 || Row > 7 || Column < 0 || Column > 7)
        return;
    Uint8 r = 255, g = 255, b = 255;
    if (ch == 'g')
        r = b = 0x0;
    else if (ch == 'r')
        g = b = 0x0;
    else
        r = g = 0x0;

    int alpha = (2 * Column + 1) * TileSize / 2, bita = (2 * Row + 1) * TileSize / 2, Big_R = TileSize / 2, small_r = TileSize / 4;
    SDL_Rect rect = {0, 0, 1, 1};

    for (int x = alpha - Big_R; x <= alpha + Big_R; x += 2)
        for (int y = bita - Big_R; y <= bita + Big_R; y += 4)
            if (((x - alpha) * (x - alpha) + (y - bita) * (y - bita) <= Big_R * Big_R) && ((x - alpha) * (x - alpha) + (y - bita) * (y - bita) >= small_r * small_r))
            {
                rect.x = x;
                rect.y = y;
                SDL_SetRenderDrawColor(renderer, r, g, b, 0xff);
                SDL_RenderFillRect(renderer, &rect);
            }
}

bool Board::ifCaslingPossible(char ch, bool *up, bool *down)
{
    if (ch == 'w')
    {
        if (CanBlackAttackThisGrid(4, 0))
            return false;

        if (Casting_WK_with_DownRook && board[5][0] == "\'" && board[6][0] == "\'" && !CanBlackAttackThisGrid(5, 0) && !CanBlackAttackThisGrid(6, 0))
            *down = true;

        if (board[3][0] == board[2][0] && board[2][0] == board[1][0] && Casting_WK_with_UpRook && !CanBlackAttackThisGrid(3, 0) && !CanBlackAttackThisGrid(2, 0) && !CanBlackAttackThisGrid(1, 0))
            *up = true;
    }
    else
    {
        if (CanWhiteAttackThisGrid(4, 7))
            return false;

        if (board[5][7] == "\'" && board[6][7] == "\'" && Casting_BK_with_DownRook && !CanWhiteAttackThisGrid(5, 7) && !CanWhiteAttackThisGrid(6, 7))
            *down = true;

        if (board[3][7] == board[2][7] && board[2][7] == board[1][7] && Casting_BK_with_UpRook && !CanWhiteAttackThisGrid(3, 7) && !CanWhiteAttackThisGrid(2, 7) && !CanWhiteAttackThisGrid(1, 7))
            *up = true;
    }

    if (*up || *down)
        return true;
    else
        return false;
}

bool Board::CanBlackAttackThisGrid(int r, int c)
{
    if (c + 1 < 8 && r + 1 < 8 && r - 1 >= 0 && (board[r + 1][c + 1] == "bp" || board[r - 1][c + 1] == "bp"))
        return true;
    // knight
    for (int cfb = -2; cfb < 3; cfb++)
    {
        if (cfb == 0 || c + cfb < 0 || c + cfb > 7)
            continue;
        for (int rud = -2; rud < 3; rud++)
        {
            if (rud == 0 || r + rud < 0 || r + rud > 7 || cfb == rud || cfb * (-1) == rud)
                continue;
            if (board[r + rud][c + cfb] == "bn")
                return true;
        }
    }

    // plus and cross.................

    bool UpForw = true, UpUp = true, UpBack = true, BackBack = true, BackDown = true, DownDown = true, DownForw = true, ForwForw = true;
    for (int x = 1; x < 8; x++)
    {
        if (c + x < 8 && board[r][c + x] != "\'" && ForwForw)
        {
            if (board[r][c + x][0] == 'w')
                ForwForw = false;
            if (board[r][c + x] == "bq" || board[r][c + x] == "br")
                return true;
        }
        if (r - x >= 0 && c + x < 8 && board[r - x][c + x] != "\'" && UpForw)
        {
            if (board[r - x][c + x][0] == 'w')
                UpForw = false;
            if (board[r - x][c + x] == "bq" || board[r - x][c + x] == "bb")
                return true;
        }
        if (r - x >= 0 && board[r - x][c] != "\'" && UpUp)
        {
            if (board[r - x][c][0] == 'w')
                UpUp = false;
            if (board[r - x][c] == "bq" || board[r - x][c] == "br")
                return true;
        }
        if (r - x >= 0 && c - x >= 0 && board[r - x][c - x] != "\'" && UpBack)
        {
            if (board[r - x][c - x][0] == 'w')
                UpBack = false;
            if (board[r - x][c - x] == "bq" || board[r - x][c - x] == "bb")
                return true;
        }
        if (c - x >= 0 && board[r][c - x] != "\'" && BackBack)
        {
            if (board[r][c - x][0] == 'w')
                BackBack = false;
            if (board[r][c - x] == "bq" || board[r][c - x] == "br")
                return true;
        }
        if (r + x < 8 && c - x >= 0 && board[r + x][c - x] != "\'" && BackDown)
        {
            if (board[r + x][c - x][0] == 'w')
                BackDown = false;
            if (board[r + x][c - x] == "bq" || board[r + x][c - x] == "bb")
                return true;
        }
        if (r + x < 8 && board[r + x][c] != "\'" && DownDown)
        {
            if (board[r + x][c][0] == 'w')
                DownDown = false;
            if (board[r + x][c] == "bq" || board[r + x][c] == "br")
                return true;
        }
        if (r + x < 8 && c + x < 8 && board[r + x][c + x] != "\'" && DownForw)
        {
            if (board[r + x][c + x][0] == 'w')
                DownForw = false;
            if (board[r + x][c + x] == "bq" || board[r + x][c + x] == "bb")
                return true;
        }
    }

    return false;
}

bool Board::CanWhiteAttackThisGrid(int r, int c)
{
    if (c - 1 >= 0 && r + 1 < 8 && r - 1 >= 0 && (board[r + 1][c - 1] == "wp" || board[r - 1][c - 1] == "wp"))
        return true;
    // knight

    for (int cfb = -2; cfb < 3; cfb++)
    {
        if (cfb == 0 || c + cfb < 0 || c + cfb > 7)
            continue;
        for (int rud = -2; rud < 3; rud++)
        {
            if (rud == 0 || r + rud < 0 || r + rud > 7 || cfb == rud || cfb * (-1) == rud)
                continue;
            if (board[r + rud][c + cfb] == "wn")
                return true;
        }
    }

    // plus and cross.................

    bool UpForw = true, UpUp = true, UpBack = true, BackBack = true, BackDown = true, DownDown = true, DownForw = true, ForwForw = true;
    for (int x = 1; x < 8; x++)
    {
        if (c + x < 8 && board[r][c + x] != "\'" && ForwForw)
        {
            if (board[r][c + x][0] == 'b')
                ForwForw = false;
            if (board[r][c + x] == "wq" || board[r][c + x] == "wr")
                return true;
        }
        if (r - x >= 0 && c + x < 8 && board[r - x][c + x] != "\'" && UpForw)
        {
            if (board[r - x][c + x][0] == 'b')
                UpForw = false;
            if (board[r - x][c + x] == "wq" || board[r - x][c + x] == "wb")
                return true;
        }
        if (r - x >= 0 && board[r - x][c] != "\'" && UpUp)
        {
            if (board[r - x][c][0] == 'b')
                UpUp = false;
            if (board[r - x][c] == "wq" || board[r - x][c] == "wr")
                return true;
        }
        if (r - x >= 0 && c - x >= 0 && board[r - x][c - x] != "\'" && UpBack)
        {
            if (board[r - x][c - x][0] == 'b')
                UpBack = false;
            if (board[r - x][c - x] == "wq" || board[r - x][c - x] == "wb")
                return true;
        }
        if (c - x >= 0 && board[r][c - x] != "\'" && BackBack)
        {
            if (board[r][c - x][0] == 'b')
                BackBack = false;
            if (board[r][c - x] == "wq" || board[r][c - x] == "wr")
                return true;
        }
        if (r + x < 8 && c - x >= 0 && board[r + x][c - x] != "\'" && BackDown)
        {
            if (board[r + x][c - x][0] == 'b')
                BackDown = false;
            if (board[r + x][c - x] == "wq" || board[r + x][c - x] == "wb")
                return true;
        }
        if (r + x < 8 && board[r + x][c] != "\'" && DownDown)
        {
            if (board[r + x][c][0] == 'b')
                DownDown = false;
            if (board[r + x][c] == "wq" || board[r + x][c] == "wr")
                return true;
        }
        if (r + x < 8 && c + x < 8 && board[r + x][c + x] != "\'" && DownForw)
        {
            if (board[r + x][c + x][0] == 'b')
                DownForw = false;
            if (board[r + x][c + x] == "wq" || board[r + x][c + x] == "wb")
                return true;
        }
    }

    return false;
}

bool Board::isWhite(int targetRow, int targetCol)
{
    if (targetCol < 0 || targetCol > 7 || targetRow < 0 || targetRow > 7)
        return false;
    if (board[targetRow][targetCol][0] == 'w')
        return true;
    else
        return false;
}

bool Board::isBlack(int targetRow, int targetCol)
{
    if (targetCol < 0 || targetCol > 7 || targetRow < 0 || targetRow > 7)
        return false;
    if (board[targetRow][targetCol][0] == 'b')
        return true;
    return false;
}

bool Board::AnyPieaceAtPlusPath(int r, int c)
{
    if (r == selectedRow)
    {
        if (c < selectedCol)
        {
            for (int i = c + 1; i < selectedCol; i++)
            {
                if (board[r][i] != "\'")
                    return false;
            }
        }
        else
        {
            for (int i = selectedCol + 1; i < c; i++)
            {
                if (board[r][i] != "\'")
                    return false;
            }
        }

        return true;
    }
    else if (c == selectedCol)
    {
        if (r < selectedRow)
        {
            for (int i = r + 1; i < selectedRow; i++)
            {
                if (board[i][c] != "\'")
                    return false;
            }
        }
        else
        {
            for (int i = selectedRow + 1; i < r; i++)
            {
                if (board[i][c] != "\'")
                    return false;
            }
        }

        return true;
    }

    return false;
}

bool Board::AnyPieaceAtRoundPath(int Row, int Col)
{
    if (abs(selectedRow - Row) == 1)
    {
        if (abs(selectedCol - Col) == 1 || Col == selectedCol)
        {
            if (board[selectedRow][selectedCol][0] == 'w')
            {
                White_King_positionAtCol = Col;
                White_King_positionAtRow = Row;
            }
            else
            {
                Black_King_positionAtRow = Row;
                Black_King_positionAtCol = Col;
            }

            return true;
        }
        else
            return false;
    }
    else if (selectedRow == Row)
    {
        if (abs(selectedCol - Col) == 1)
        {

            if (board[selectedRow][selectedCol][0] == 'w')
            {
                White_King_positionAtCol = Col;
                White_King_positionAtRow = Row;
            }
            else
            {
                Black_King_positionAtRow = Row;
                Black_King_positionAtCol = Col;
            }

            return true;
        }
        else
            return false;
    }
    else
        return false;
}

bool Board::KnightPath(int r, int c)
{
    if ((abs(c - selectedCol) == 2 && abs(r - selectedRow) == 1) || (abs(c - selectedCol) == 1 && abs(r - selectedRow) == 2))
        return true;
    return false;
}

bool Board::AnyPieaceAtCrossPath(int r, int c)
{
    bool no_abstruct = true;
    if (r > selectedRow && c > selectedCol)
    {
        for (int x = selectedCol + 1, y = selectedRow + 1; x < c && y < r; x++, y++)
        {
            if (board[y][x] != "\'")
            {
                no_abstruct = false;
                break;
            }
        }
    }
    else if (r > selectedRow && c < selectedCol)
    {
        for (int x = selectedCol - 1, y = selectedRow + 1; x > c && y < r; x--, y++)
        {
            if (board[y][x] != "\'")
            {
                no_abstruct = false;
                break;
            }
        }
    }
    else if (r < selectedRow && c < selectedCol)
    {

        for (int x = selectedCol - 1, y = selectedRow - 1; x > c && y > r; x--, y--)
        {
            if (board[y][x] != "\'")
            {
                no_abstruct = false;
                break;
            }
        }
    }
    else if (r < selectedRow && c > selectedCol)
    {

        for (int x = selectedCol + 1, y = selectedRow - 1; x < c && y > r; x++, y--)
        {
            if (board[y][x] != "\'")
            {
                no_abstruct = false;
                break;
            }
        }
    }
    else
    {
    }

    if (abs(r - selectedRow) == abs(c - selectedCol) && no_abstruct)
        return true;
    else
        return false;
}

bool Board::QueenPath(int targetRow, int targetCol)
{
    if (targetCol == selectedCol || targetRow == selectedRow)
        return AnyPieaceAtPlusPath(targetRow, targetCol);
    else
        return AnyPieaceAtCrossPath(targetRow, targetCol);
    return false;
}
// here under ----------------------------------------4.4.3_II_2.1

bool Board::function(int targetRow, int targetCol) //-------------------4.4.3_II_2.1
{
    if (board[selectedRow][selectedCol][0] == board[targetRow][targetCol][0])
        return true;

    // forPawn()
    if (board[selectedRow][selectedCol] == "wp")                                                                                              // White pawn//-------------------4.4.3_II_2.2
        if (selectedCol == 1 && (targetCol == 3 && targetRow == selectedRow && board[selectedRow][2] == "\'" && board[targetRow][3] == "\'")) // First move
        {
            // Move forward 2
        }
        else if (targetCol == selectedCol + 1 && targetRow == selectedRow && board[targetRow][targetCol] == "\'")
        {
            // Move forward 1
        }
        else if (targetCol - 1 == selectedCol && (targetRow == selectedRow + 1 || targetRow == selectedRow - 1) && isBlack(targetRow, targetCol))
        {
            // Diagonal capture
        }
        else
            return true;

    else if (board[selectedRow][selectedCol] == "bp")                                                                                         // Black pawn//-------------------4.4.3_II_2.3
        if (selectedCol == 6 && (targetCol == 4 && targetRow == selectedRow && board[selectedRow][5] == "\'" && board[targetRow][4] == "\'")) // First move
        {
            // Move forward 2
        }
        else if (targetCol == selectedCol - 1 && targetRow == selectedRow && board[targetRow][targetCol] == "\'")
        {
            // Move forward 1
        }
        else if (targetCol == selectedCol - 1 &&
                 (targetRow == selectedRow + 1 || targetRow == selectedRow - 1) && board[targetRow][targetCol][0] == 'w') // isWhite
        {
            // Diagonal capture
        }
        else
            return true;

    // for Rook()
    else if (board[selectedRow][selectedCol][1] == 'r') //-------------------4.4.3_II_2.4
    {
        if (AnyPieaceAtPlusPath(targetRow, targetCol))
        {
            if (White_s_Turn == WHITEs_TURN)
            {
                if (Casting_WK_with_UpRook && selectedCol == 0 && selectedRow == 0)
                    Casting_WK_with_UpRook = false;
                else if (Casting_WK_with_DownRook && selectedCol == 0 && selectedRow == 7)
                    Casting_WK_with_DownRook = false;
            }
            else
            {
                if (Casting_BK_with_UpRook && selectedCol == 7 && selectedRow == 0)
                    Casting_BK_with_UpRook = false;
                else if (Casting_BK_with_DownRook && selectedCol == 7 && selectedRow == 7)
                    Casting_BK_with_DownRook = false;
            }
        }
        else
            return true;
    }
    // for Knight()
    else if (board[selectedRow][selectedCol][1] == 'n') //-------------------4.4.3_II_2.5
    {
        if (KnightPath(targetRow, targetCol))
        {
        }
        else
            return true;
    }

    // for Bishop()
    else if (board[selectedRow][selectedCol][1] == 'b') //-------------------4.4.3_II_2.6
    {
        if (AnyPieaceAtCrossPath(targetRow, targetCol))
        {
        }
        else
            return true;
    }

    // for Queen()
    else if (board[selectedRow][selectedCol][1] == 'q') //-------------------4.4.3_II_2.7
    {
        if (QueenPath(targetRow, targetCol))
        {
        }
        else
            return true;
    }

    // for King()
    else if (board[selectedRow][selectedCol][1] == 'k') //-------------------4.4.3_II_2.8
    {
        if (AnyPieaceAtRoundPath(targetRow, targetCol))
        {
            if (White_s_Turn == WHITEs_TURN)
                IsWhiteKingHaveCastlingChance = Casting_WK_with_UpRook = Casting_WK_with_DownRook = false;
            else
                IsBlackKingHaveCastlingChance = Casting_BK_with_DownRook = Casting_BK_with_UpRook = false;
        }

        else
        {
            bool flag = true;
            if (White_s_Turn == WHITEs_TURN)
            {
                if (IsWhiteKingHaveCastlingChance && targetCol == 0)
                {
                    if (nowYouHave_wk_caslingWithUpWR && targetRow == 2 && Casting_WK_with_UpRook)
                    {
                        flag = false;
                        IsWhiteKingHaveCastlingChance = Casting_WK_with_UpRook = Casting_WK_with_DownRook = false;
                        board[0][0] = "\'";
                        board[3][0] = "wr";
                    }
                    else if (nowYouHave_wk_caslingWithDownWR && targetRow == 6 && Casting_WK_with_DownRook)
                    {
                        flag = false;
                        IsWhiteKingHaveCastlingChance = Casting_WK_with_UpRook = Casting_WK_with_DownRook = false;
                        board[7][0] = "\'";
                        board[5][0] = "wr";
                    }
                }
            }

            else
            {
                if (IsBlackKingHaveCastlingChance && targetCol == 7)
                {
                    if (nowYouHave_bk_caslingWithDownBR && targetRow == 6 && Casting_BK_with_DownRook)
                    {
                        flag = false;
                        IsBlackKingHaveCastlingChance = Casting_BK_with_DownRook = Casting_BK_with_UpRook = false;
                        board[7][7] = "\'";
                        board[5][7] = "br";
                    }
                    else if (nowYouHave_bk_caslingWithUpBR && targetRow == 2 && Casting_BK_with_UpRook)
                    {
                        flag = false;
                        IsBlackKingHaveCastlingChance = Casting_BK_with_DownRook = Casting_BK_with_UpRook = false;
                        board[0][7] = "\'";
                        board[3][7] = "br";
                    }
                }
            }
            if (flag)
                return true;
        }
    }

    board[targetRow][targetCol] = board[selectedRow][selectedCol];
    board[selectedRow][selectedCol] = "\'";
    return false;
}

void Board::ShowPosibilitiesGrid()
{
    int Col = selectedCol, Row = selectedRow;
    if (board[Row][Col][1] == 'p')
    {
        if (White_s_Turn == WHITEs_TURN)
        {
            if (isBlack(Row + 1, Col + 1))
                DrawSquereRing(Row + 1, Col + 1, 'r');
            if (isBlack(Row - 1, Col + 1))
                DrawSquereRing(Row - 1, Col + 1, 'r');
            if (board[Row][Col + 1] == "\'")
                DrawSquereRing(Row, Col + 1, 'g');
            if (board[Row][3] == "\'" && Col == 1 && board[Row][2] == "\'")
                DrawSquereRing(Row, 3, 'B');
        }
        else
        {
            if (isWhite(Row + 1, Col - 1))
                DrawSquereRing(Row + 1, Col - 1, 'r');
            if (isWhite(Row - 1, Col - 1))
                DrawSquereRing(Row - 1, Col - 1, 'r');
            if (board[Row][Col - 1] == "\'")
                DrawSquereRing(Row, Col - 1, 'g');
            if (board[Row][4] == "\'" && Col == 6 && board[Row][5] == "\'")
                DrawSquereRing(Row, 4, 'B');
        }
    }
    else if (board[Row][Col][1] == 'n')
        DrawKnightPath(Row, Col);
    else if (board[Row][Col][1] == 'r')
        DrawRookPath(Row, Col);
    else if (board[Row][Col][1] == 'b')
        DrawBishopPath(Row, Col);
    else if (board[Row][Col][1] == 'q')
        DrawQueenPath(Row, Col);
    else if (board[Row][Col][1] == 'k')
    {
        DrawKingPath(Row, Col);
        bool up = false, down = false;

        if (White_s_Turn == WHITEs_TURN)
        {
            if (IsWhiteKingHaveCastlingChance)
            {
                if (!ifCaslingPossible('w', &up, &down))
                {
                    nowYouHave_wk_caslingWithDownWR = down;
                    nowYouHave_wk_caslingWithUpWR = up;
                    return;
                }

                nowYouHave_wk_caslingWithDownWR = down;
                nowYouHave_wk_caslingWithUpWR = up;
                if (Casting_WK_with_DownRook && down)
                    DrawSquereRing(Row + 2, Col, 'b');
                if (Casting_WK_with_UpRook && up)
                    DrawSquereRing(Row - 2, Col, 'b');
            }
        }
        else
        {
            if (IsBlackKingHaveCastlingChance)
            {
                if (!ifCaslingPossible('b', &up, &down))
                {
                    nowYouHave_bk_caslingWithDownBR = down;
                    nowYouHave_bk_caslingWithUpBR = up;
                    return;
                }

                nowYouHave_bk_caslingWithDownBR = down;
                nowYouHave_bk_caslingWithUpBR = up;
                if (Casting_BK_with_DownRook && down)
                    DrawSquereRing(Row + 2, Col, 'b');
                if (Casting_BK_with_UpRook && up)
                    DrawSquereRing(Row - 2, Col, 'b');
            }
        }
    }
}

Game::Game() : window(nullptr), renderer(nullptr), running(false), side_text(nullptr), font(nullptr)
{
    Text_Rect = {970, 0, 125, 125};
} //------------------1.1

bool Game::isInitOkey(const char *Title, int WIDTH, int HEIGHT)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) //------2.1
    {
        std::cerr << "SDL Init Error at " << __LINE__ << ": " << SDL_GetError() << std::endl;
        return true;
    }

    window = SDL_CreateWindow(Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) //-----2.2
    {
        std::cerr << "CreateWindow Error at " << __LINE__ << ": " << SDL_GetError() << std::endl;
        return true;
    }

    if ((IMG_Init(IMG_Flag) & IMG_Flag) != IMG_Flag)
    {
        std::cerr << "Img Load Error! at " << __LINE__ << ": " << IMG_GetError() << std::endl;
        return true;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //---------2.3
    if (!renderer)
    {
        std::cerr << "CreateRenderer Error at " << __LINE__ << ": " << SDL_GetError() << std::endl;
        return true;
    }

    if (TTF_Init())
    {
        std::cerr << "TTF Error at " << __LINE__ << ": " << TTF_GetError() << std::endl;
        return true;
    }

    if ((Mix_Init(MIXER_FLAGS) & MIXER_FLAGS) != MIXER_FLAGS)
    {
        std::cerr << "Error initializing SDL_mixer: " << Mix_GetError() << std::endl;
        return true;
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048)) // AUDIO MIX INIT
    {
        std::cerr << "Error Opening Audio: " << Mix_GetError() << std::endl;
        return true;
    }

    MoveSound = Mix_LoadMUS("Sound/chessy.mp3");
    if (!MoveSound)
    {
        fprintf(stderr, "Error loading Music: %s\n", Mix_GetError());
        return true;
    }

    necessaryFunction("Hello, This is a Classic Chess game (ENJOY!)");

    BoardClass = new Board(renderer, MoveSound); //----------2.4

    running = true; //--------2.5
    return false;
}

void Game::handleEvents() //-------------4.1
{
    while (SDL_PollEvent(&event)) //----------------4.2
    {
        if (event.type == SDL_QUIT || event.key.keysym.scancode == SDL_SCANCODE_ESCAPE || ((event.key.keysym.mod & (KMOD_CTRL)) && ((event.key.keysym.scancode == SDL_SCANCODE_W) || (event.key.keysym.scancode == SDL_SCANCODE_D))))
            running = false;

        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
            BoardClass->setupBoard(); // reset all board again

        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            SDL_GetMouseState(&x, &y);     //----------------4.3
            BoardClass->handleClick(x, y); //----------------4.4
        }
    }
}

void Game::update() //----------------5.1
{
}

void Game::render() //-------------6.1
{
    // SDL_SetRenderDrawColor(renderer, 0x0, 0xdf, 0xdf, 0xff);

    SDL_RenderClear(renderer);
    BoardClass->draw(); //-----------------6.2
    SDL_RenderCopy(renderer, side_text, nullptr, &Text_Rect);

    SDL_RenderPresent(renderer); //-------------------6.3
}

void Game::clean()
{
    if (BoardClass)
        delete BoardClass;

    Mix_HaltChannel(-1);
    if (MoveSound)
        Mix_FreeMusic(MoveSound);
    Mix_CloseAudio();

    if (side_text)
        SDL_DestroyTexture(side_text);

    if (font)
        TTF_CloseFont(font);

    if (renderer)
        SDL_DestroyRenderer(renderer);

    if (window)
        SDL_DestroyWindow(window);

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool Game::isRunning() const //-------------------3.1
{
    return running;
}

void Game::rest_for_a_little() //------------------7
{
    SDL_Delay(16); // ~60 FPS
}

Game::~Game()
{
    clean();
}

void Game::necessaryFunction(const char *string)
{
    SDL_Color colour = {55, 234, 123, 255};
    font = TTF_OpenFont("Font/Bold.ttf", 30);
    SDL_Surface *temp = TTF_RenderText_Blended(font, string, colour); //*If You want to exit, type escape or cross icon\n *If you want to Reset all the game again, just type the \" \" (Space) button\n-->>ENJOY!\n", colour);
    Text_Rect.h = 45;
    Text_Rect.w = temp->w;
    side_text = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
}

int main(int argc, char *argv[])
{
    Game *game = new Game; //----1

    if (game->isInitOkey(TITLE, WIDTH, HEIGHT)) //-----2
    {
        delete game; //------III (if something wrong)
        // destructor will automatically delete the game object at heap
        return -1;
    }
    std::cout << "So here we go" << std::endl;

    while (game->isRunning()) //-------------3
    {
        game->handleEvents();      //------------4
        game->update();            //------------5
        game->render();            //------------6
        game->rest_for_a_little(); //------------7
    }
    std::cout << "BYE!" << std::endl;
    delete game; //---------------8
    return 0;    //-----------------9
}
