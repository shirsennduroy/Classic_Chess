#include "Board.hpp"

Board::Board(SDL_Renderer *rend) : renderer(rend), board(8, std::vector<std::string>(8)) // setting board[8][8]
{                                                                                        //, Black_king_Surrounding(3, std::vector<int>(3)), White_king_Surrounding(3, std::vector<int>(3))
    loadPieceTextures();
    setupBoard();
}
/*
bool Board::is_the_grid_danger(int r, int c, char ch) // chis king's w or b
{
    if (ch == 'w') // if king is white
    {
        if (isStrPieaceGoingToAttack(r + 1, c + 2, "bn") || isStrPieaceGoingToAttack(r - 1, c + 2, "bn") || isStrPieaceGoingToAttack(r - 1, c - 2, "bn") || isStrPieaceGoingToAttack(r + 1, c - 2, "bn") || isStrPieaceGoingToAttack(r - 2, c + 1, "bn") || isStrPieaceGoingToAttack(r - 2, c - 1, "bn") || isStrPieaceGoingToAttack(r + 2, c + 1, "bn") || isStrPieaceGoingToAttack(r + 2, c - 1, "bn"))
        {
            return true;
        }

        else if (ifPieceDominatePlusPath(r, c, 'b'))
        {
            return true;
        }
        else if (ifPieceDominateCrossPath(r, c, 'b'))
        {
            return true;
        }
        else
            return false;

        // for gride suppose x. we will apply rookpath,for rook or minister or
    }
    else
    {

        if (isStrPieaceGoingToAttack(r + 1, c + 2, "wn") || isStrPieaceGoingToAttack(r - 1, c + 2, "wn") || isStrPieaceGoingToAttack(r - 1, c - 2, "wn") || isStrPieaceGoingToAttack(r + 1, c - 2, "wn") || isStrPieaceGoingToAttack(r - 2, c + 1, "wn") || isStrPieaceGoingToAttack(r - 2, c - 1, "wn") || isStrPieaceGoingToAttack(r + 2, c + 1, "wn") || isStrPieaceGoingToAttack(r + 2, c - 1, "wn"))
        {
            return true;
        }

        else if (ifPieceDominatePlusPath(r, c, 'w'))
        {
            return true;
        }
        else if (ifPieceDominateCrossPath(r, c, 'w'))
        {
            return true;
        }
        else
            return false;
    }
    return false; // not needed
}

bool Board::ifPieceDominateCrossPath(int r, int c, char ch) // ch is attacker
{
    if (ch == 'b')
    {
        if (isStrPieaceGoingToAttack(r - 1, c + 1, "bp") || isStrPieaceGoingToAttack(r + 1, c + 1, "bp"))
        {
            return true;
        }

        for (int x = c + 1, y = r + 1; x < 8 && y < 8; x++, y++)
        {
            if (isWhite(y, x) || isStrPieaceGoingToAttack(y, x, "br") || isStrPieaceGoingToAttack(y, x, "bn"))
                break;
            else if (isStrPieaceGoingToAttack(y, x, "bb") || isStrPieaceGoingToAttack(y, x, "bq"))
                return true;
        }

        for (int x = c + 1, y = r - 1; x < 8 && y >= 0; x++, y--)
        {
            if (isWhite(y, x) || isStrPieaceGoingToAttack(y, x, "br") || isStrPieaceGoingToAttack(y, x, "bn"))
                break;
            else if (isStrPieaceGoingToAttack(y, x, "bb") || isStrPieaceGoingToAttack(y, x, "bq"))
                return true;
        }
        for (int x = c - 1, y = r - 1; x >= 0 && y >= 0; x--, y--)
        {
            if (isWhite(y, x) || isStrPieaceGoingToAttack(y, x, "br") || isStrPieaceGoingToAttack(y, x, "bn"))
                break;
            else if (isStrPieaceGoingToAttack(y, x, "bb") || isStrPieaceGoingToAttack(y, x, "bq"))
                return true;
        }
        for (int x = c - 1, y = r + 1; x >= 0 && y < 8; x--, y++)
        {
            if (isWhite(y, x) || isStrPieaceGoingToAttack(y, x, "br") || isStrPieaceGoingToAttack(y, x, "bn"))
                break;
            else if (isStrPieaceGoingToAttack(y, x, "bb") || isStrPieaceGoingToAttack(y, x, "bq"))
                return true;
        }
    }
    else
    {
        if (isStrPieaceGoingToAttack(r - 1, c - 1, "wp") || isStrPieaceGoingToAttack(r + 1, c - 1, "wp"))
        {
            return true;
        }

        for (int x = c + 1, y = r + 1; x < 8 && y < 8; x++, y++)
        {
            if (isBlack(y, x) || isStrPieaceGoingToAttack(y, x, "wr") || isStrPieaceGoingToAttack(y, x, "wn"))
                break;
            else if (isStrPieaceGoingToAttack(y, x, "wb") || isStrPieaceGoingToAttack(y, x, "wq"))
                return true;
        }

        for (int x = c + 1, y = r - 1; x < 8 && y >= 0; x++, y--)
        {
            if (isBlack(y, x) || isStrPieaceGoingToAttack(y, x, "wr") || isStrPieaceGoingToAttack(y, x, "wn"))
                break;
            else if (isStrPieaceGoingToAttack(y, x, "wb") || isStrPieaceGoingToAttack(y, x, "wq"))
                return true;
        }
        for (int x = c - 1, y = r - 1; x >= 0 && y >= 0; x--, y--)
        {
            if (isBlack(y, x) || isStrPieaceGoingToAttack(y, x, "wr") || isStrPieaceGoingToAttack(y, x, "wn"))
                break;
            else if (isStrPieaceGoingToAttack(y, x, "wb") || isStrPieaceGoingToAttack(y, x, "wq"))
                return true;
        }
        for (int x = c - 1, y = r + 1; x >= 0 && y < 8; x--, y++)
        {
            if (isBlack(y, x) || isStrPieaceGoingToAttack(y, x, "wr") || isStrPieaceGoingToAttack(y, x, "wn"))
                break;
            else if (isStrPieaceGoingToAttack(y, x, "wb") || isStrPieaceGoingToAttack(y, x, "wq"))
                return true;
        }
    }
    return false;
}

bool Board::ifPieceDominatePlusPath(int r, int c, char ch) // ch for attacker
{
    if (ch == 'b')
    {
        if (isStrPieaceGoingToAttack(r + 1, c + 1, "bk") || isStrPieaceGoingToAttack(r, c + 1, "bk") || isStrPieaceGoingToAttack(r - 1, c + 1, "bk") || isStrPieaceGoingToAttack(r - 1, c, "bk") || isStrPieaceGoingToAttack(r - 1, c - 1, "bk") || isStrPieaceGoingToAttack(r, c - 1, "bk") || isStrPieaceGoingToAttack(r + 1, c - 1, "bk") || isStrPieaceGoingToAttack(r + 1, c, "bk"))
        {
            return true;
        }
        for (int i = r + 1; i < 8; i++)
        {
            if (isWhite(i, c) || isStrPieaceGoingToAttack(i, c, "bb") || isStrPieaceGoingToAttack(i, c, "bn"))
                break;
            else if (isStrPieaceGoingToAttack(i, c, "br") || isStrPieaceGoingToAttack(i, c, "bq"))
                return true;
        }

        for (int i = r - 1; i >= 0; i--)
        {
            if (isWhite(i, c) || isStrPieaceGoingToAttack(i, c, "bb") || isStrPieaceGoingToAttack(i, c, "bn"))
                break;
            else if (isStrPieaceGoingToAttack(i, c, "br") || isStrPieaceGoingToAttack(i, c, "bq"))
                return true;
        }

        for (int i = c + 1; i < 8; i++)
        {
            if (isWhite(r, i) || isStrPieaceGoingToAttack(r, i, "bb") || isStrPieaceGoingToAttack(r, i, "bn"))
                break;
            else if (isStrPieaceGoingToAttack(r, i, "br") || isStrPieaceGoingToAttack(r, i, "bq"))
                return true;
        }

        for (int i = c - 1; i >= 0; i++)
        {
            if (isWhite(r, i) || isStrPieaceGoingToAttack(r, i, "bb") || isStrPieaceGoingToAttack(r, i, "bn"))
                break;
            else if (isStrPieaceGoingToAttack(r, i, "br") || isStrPieaceGoingToAttack(r, i, "bq"))
                return true;
        }
    }
    else if (ch == 'w')
    {
        if (isStrPieaceGoingToAttack(r + 1, c + 1, "wk") || isStrPieaceGoingToAttack(r, c + 1, "wk") || isStrPieaceGoingToAttack(r - 1, c + 1, "wk") || isStrPieaceGoingToAttack(r - 1, c, "wk") || isStrPieaceGoingToAttack(r - 1, c - 1, "wk") || isStrPieaceGoingToAttack(r, c - 1, "wk") || isStrPieaceGoingToAttack(r + 1, c - 1, "wk") || isStrPieaceGoingToAttack(r + 1, c, "wk"))
        {
            return true;
        }

        for (int i = r + 1; i < 8; i++)
        {
            if (isBlack(i, c) || isStrPieaceGoingToAttack(i, c, "wb") || isStrPieaceGoingToAttack(i, c, "wn"))
                break;
            else if (isStrPieaceGoingToAttack(i, c, "wr") || isStrPieaceGoingToAttack(i, c, "wq"))
                return true;
        }

        for (int i = r - 1; i >= 0; i--)
        {
            if (isBlack(i, c) || isStrPieaceGoingToAttack(i, c, "wb") || isStrPieaceGoingToAttack(i, c, "wn"))
                break;
            else if (isStrPieaceGoingToAttack(i, c, "wr") || isStrPieaceGoingToAttack(i, c, "wq"))
                return true;
        }

        for (int i = c + 1; i < 8; i++)
        {
            if (isBlack(r, i) || isStrPieaceGoingToAttack(r, i, "wb") || isStrPieaceGoingToAttack(r, i, "wn"))
                break;
            else if (isStrPieaceGoingToAttack(r, i, "wr") || isStrPieaceGoingToAttack(r, i, "wq"))
                return true;
        }

        for (int i = c - 1; i >= 0; i++)
        {
            if (isBlack(r, i) || isStrPieaceGoingToAttack(r, i, "wb") || isStrPieaceGoingToAttack(r, i, "wn"))
                break;
            else if (isStrPieaceGoingToAttack(r, i, "wr") || isStrPieaceGoingToAttack(r, i, "wq"))
                return true;
        }
    }

    return false;
}

bool Board::isStrPieaceGoingToAttack(int R, int C, std::string Str)
{
    if (R < 0 || C < 0 || R > 7 || C > 7)
        return false;
    else if (board[R][C] != Str)
        return false;

    return true;
}
*/
    void Board::draw() // called from game
{
    SDL_Color light{0xff, 0xff, 0xff, 0xff}; // Light tile color
    SDL_Color dark{0x4a, 0x5a, 0x5a, 0xff};  // Dark tile color
    bool isDark;

    for (int row{0}; row < 8; ++row)
    {
        for (int col{0}; col < 8; ++col)
        {
            tile.x = col * tileSize;
            tile.y = row * tileSize;
            tile.w = tile.h = tileSize;

            isDark = (row + col) & 1 ? 0 : 1;

            SDL_SetRenderDrawColor(renderer,
                                   isDark ? dark.r : light.r,
                                   isDark ? dark.g : light.g,
                                   isDark ? dark.b : light.b,
                                   0xff);
            SDL_RenderFillRect(renderer, &tile);

            //-----------------------------------------------
            // Highlight selected square
            if (selectedRow >= 0 && selectedCol >= 0) // New fourth
            {
                // if(isPawn(row,col)){highlight_paths("")}
                // I will hight light the selected pieces
                SDL_Rect highlight;
                highlight.x = selectedCol * tileSize;
                highlight.y = selectedRow * tileSize;
                highlight.w = tileSize;
                highlight.h = tileSize;

                /*
                selectedRow and selectedCol are in "Board.hpp"
                */

                SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0xa0, 0xff); // Yellow translucent

                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                // Showing the alpha feature

                SDL_RenderFillRect(renderer, &highlight);
            }
        }
    }

    drawPieces(); // new third
    // calling the drawPieces
}
//--------------------------------------

// Now this is new 5th

void Board::handleClick(int mouseX, int mouseY) // new 5th
{

    int row = mouseY / tileSize;
    int col = mouseX / tileSize;

    // First click ----- select a piece
    if (!pieceSelected)
    {
        if (board[row][col] != "\'")
        {
            pieceSelected = true;
            selectedRow = row;
            selectedCol = col;
            std::cout << "Selected: " << board[row][col] << " at " << row << "," << col << "]" << std::endl;
        }
    }
    // Second click - move the selected piece
    else
    {
        movePiece(row, col); // from board class at Board.hpp
        /// check , check ,whitw for , black for ,casling all will be done here
        pieceSelected = false;
        selectedRow = -1;
        selectedCol = -1;
    }
}

//-----------------------------------------  one time used
void Board::loadPieceTextures() // new third //One time used at initialiesed first
{
    // std::string base = "assets/pieces/";
    std::string names[] = {"wp", "wr", "wn", "wb", "wq", "wk", "bp", "br", "bn", "bb", "bq", "bk"};

    for (const auto &name : names)
    {
        pieceTextures[name] = TextureManager::LoadTexture( name + ".png", renderer); // Calling TexureManager.hpp
    }
}
void Board::setupBoard() // new third //One time used at initialiesed first
{
    // Initialize all empty
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
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
}
//-------------------------------------------

//-------------------------------------------
void Board::drawPieces() // new third   // the main drawing in this function
{
    SDL_Rect dest;
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            std::string id = board[row][col];
            if (id == "\'")
                continue;

            dest.x = col * tileSize;
            dest.y = row * tileSize;
            dest.w = dest.h = tileSize;

            SDL_RenderCopy(renderer, pieceTextures[id], nullptr, &dest); // drawing the pieces
        }
    }
}

void Board::movePiece(int targetRow, int targetCol) // new 5th //this is the very important part in chess
{
    if (selectedRow == targetRow && selectedCol == targetCol)
        return; // No move

    function(targetRow, targetCol);

    std::cout << "Moved to " << targetRow << "," << targetCol << "]\n"
              << std::endl;
}

bool Board::isWhite(int targetRow, int targetCol)
{
    if (board[targetRow][targetCol][0] == 'w')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Board::isBlack(int targetRow, int targetCol)
{
    if (board[targetRow][targetCol][0] == 'b')
    {
        return true;
    }
    else
    {
        return false;
    }
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
        {
            return false;
        }
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
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool Board::KnightPath(int r, int c)
{
    if ((abs(c - selectedCol) == 2 && abs(r - selectedRow) == 1) || (abs(c - selectedCol) == 1 && abs(r - selectedRow) == 2))
    {
        return true;
    }
    else
    {
        return false;
    }
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
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Board::QueenPath(int targetRow, int targetCol)
{
    if (targetCol == selectedCol || targetRow == selectedRow)
    {
        return AnyPieaceAtPlusPath(targetRow, targetCol);
    }
    else
    {
        return AnyPieaceAtCrossPath(targetRow, targetCol);
    }
    return false;
}

void Board::function(int targetRow, int targetCol) // the update will now goes on there
{
    char ch = board[selectedRow][selectedCol][0];
    char ch_ = board[targetRow][targetCol][0];
    if (ch == ch_)
    {
        return;
    }
    else
    {
        // forPawn()

        if (board[selectedRow][selectedCol] == "wp") // White pawn
        {
            if (selectedCol == 1) // First move
            {
                if (targetCol == selectedCol + 2 && targetRow == selectedRow &&
                    board[selectedRow][selectedCol + 1] == "\'" && board[targetRow][targetCol] == "\'")
                {
                    // Move forward 2
                }
                else if (targetCol == selectedCol + 1 && targetRow == selectedRow &&
                         board[targetRow][targetCol] == "\'")
                {
                    // Move forward 1
                }
                else if (targetCol - 1 == selectedCol &&
                         (targetRow == selectedRow + 1 || targetRow == selectedRow - 1) &&
                         isBlack(targetRow, targetCol))
                {
                    // Diagonal capture
                }
                else
                    return;
            }
            else
            {
                if (targetCol == selectedCol + 1 && targetRow == selectedRow &&
                    board[targetRow][targetCol] == "\'")
                {
                    // Move forward 1
                }
                else if (targetCol == selectedCol + 1 &&
                         (targetRow == selectedRow + 1 || targetRow == selectedRow - 1) &&
                         isBlack(targetRow, targetCol))
                {
                    // Diagonal capture
                }
                else
                    return;
            }
        }
        else if (board[selectedRow][selectedCol] == "bp") // Black pawn
        {
            if (selectedCol == 6) // First move
            {
                if (targetCol == selectedCol - 2 && targetRow == selectedRow &&
                    board[selectedRow][selectedCol - 1] == "\'" && board[targetRow][targetCol] == "\'")
                {
                    // Move forward 2
                }
                else if (targetCol == selectedCol - 1 && targetRow == selectedRow &&
                         board[targetRow][targetCol] == "\'")
                {
                    // Move forward 1
                }
                else if (targetCol == selectedCol - 1 &&
                         (targetRow == selectedRow + 1 || targetRow == selectedRow - 1) &&
                         board[targetRow][targetCol][0] == 'w') // isWhite
                {
                    // Diagonal capture
                }
                else
                    return;
            }
            else
            {
                if (targetCol == selectedCol - 1 && targetRow == selectedRow &&
                    board[targetRow][targetCol] == "\'")
                {
                    // Move forward 1
                }
                else if (targetCol == selectedCol - 1 &&
                         (targetRow == selectedRow + 1 || targetRow == selectedRow - 1) &&
                         board[targetRow][targetCol][0] == 'w') // isWhite
                {
                    // Diagonal capture
                }
                else
                    return;
            }
        }
        // forRook()
        else if (board[selectedRow][selectedCol][1] == 'r')
        {
            if (AnyPieaceAtPlusPath(targetRow, targetCol))
            {
            }
            else
            {
                return;
            }
        }
        // forKnight()
        else if (board[selectedRow][selectedCol][1] == 'n')
        {
            if (KnightPath(targetRow, targetCol))
            {
            }
            else
            {
                return;
            }
        }

        // forBishop()
        else if (board[selectedRow][selectedCol][1] == 'b')
        {
            if (AnyPieaceAtCrossPath(targetRow, targetCol))
            {
            }
            else
            {
                return;
            }
        }

        // forQueen()
        else if (board[selectedRow][selectedCol][1] == 'q')
        {
            if (QueenPath(targetRow, targetCol))
            {
            }
            else
            {
                return;
            }
        }

        // forKing()
        else if (board[selectedRow][selectedCol] == "wk")
        {

            if (AnyPieaceAtRoundPath(targetRow, targetCol))
            {
            }
            else
            {
                return;
            }
        }
        else if (board[selectedRow][selectedCol] == "bk")
        {

            if (AnyPieaceAtRoundPath(targetRow, targetCol))
            {
            }
            else
            {
                return;
            }
        }

        board[targetRow][targetCol] = board[selectedRow][selectedCol];
        board[selectedRow][selectedCol] = "\'";
    }
}