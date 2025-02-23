#include <iostream>
#include <vector>
#include <raylib.h>
#include <map>
#include <random>
using namespace std;

/////////////////////////////////
//                             //
//                             //
//  Colours used throughout    //
//                             //
//                             //
//                             //
/////////////////////////////////
const Color grey = {124, 124, 124, 255}; // defining all the colours that are to be used.
const Color orange = {255, 165, 0, 255};
const Color red = {255, 0, 0, 255};
const Color purple = {160, 32, 240, 255};
const Color turquoise = {48, 213, 200, 255};
const Color yellow = {255, 255, 0, 255};
const Color blue = {13, 64, 216, 255};
const Color cyan = {21, 204, 209, 255};
const Color DarkBlue = {44, 44, 127, 255};

class Position
{
public:
    int x, y;
    Position(int a, int b)
    {
        x = a;
        y = b;
    }
};

vector<Color> GetColours() // allows us to use this multiple times for different block types
{
    vector<Color> colors = {grey, orange, red, yellow, turquoise, blue, purple, cyan};
    return colors;
};

/////////////////////////////////
//                             //
//                             //
//        Game Board           //
//                             //
//                             //
//                             //
/////////////////////////////////

class Board // defining the board class
{
public:
    Board()
    {
        rows = 20;
        columns = 10;
        block_size = 30;
        colors = GetColours();

        Initialise();
    };
    void drawBoard()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                int val = board[i][j];
                DrawRectangle((j * block_size) + 11, (i * block_size) + 11, block_size - 1, block_size - 1, colors[val]);
            }
        }
    };
    void Initialise()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                board[i][j] = 0;
            }
        }
    };
    bool Check(int r, int c)
    {
        if (r >= 0 && r < rows && c >= 0 && c < columns)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    int board[20][10];
    bool isEmpty(int r, int c)
    {
        if (board[r][c] == 0)
        {
            return true;
        }
        return false;
    }
    int clearRows()
    {
        int removed = 0;
        for (int i = rows - 1; i >= 0; i--)
        {
            if (isRowFull(i))
            {
                clearRow(i);
                removed++;
            }
            else if (removed > 0)
            {
                MoveRows(i, removed);
            }
        }
        return removed;
    }

private:
    bool isRowFull(int r)
    {
        for (int i = 0; i < columns; i++)
        {
            if (board[r][i] == 0)
            {
                return false;
            }
        }
        return true;
    }
    void clearRow(int r)
    {
        for (int c = 0; c < columns; c++)
        {
            board[r][c] = 0;
        }
    }

    void MoveRows(int r, int num)
    {
        for (int c = 0; c < columns; c++)
        {
            board[r + num][c] = board[r][c];
            board[r][c] = 0;
        }
    }
    vector<Color> colors;
    int rows, columns;
    int block_size;

}; // end definition board class
/////////////////////////////////
//                             //
//                             //
//    Defining the types       //
//         of blocks           //
//                             //
//                             //
/////////////////////////////////

class Block // defining blocks base class
{
public:
    Block()
    {
        cellSize = 30;
        state = 0;
        colors = GetColours();
        Orow = 0;
        Ocol = 0;
    };
    void drawBlock(int x, int y)
    {
        vector<Position> cellBlock = GetPos();
        for (auto i : cellBlock)
        {
            DrawRectangle(i.y * cellSize + x, i.x * cellSize + y, cellSize - 1, cellSize - 1, colors[type]);
        }
    }
    void Move(int a, int b)
    {
        Orow += a;
        Ocol += b;
    }
    vector<Position> GetPos()
    {
        vector<Position> cellBlock = cells[state];
        vector<Position> movedBlock;
        for (auto i : cellBlock)
        {
            Position newpos = Position(i.x + Orow, i.y + Ocol);
            movedBlock.push_back(newpos);
        }
        return movedBlock;
    }
    int type;
    map<int, vector<Position>> cells;

    void Rotate()
    {
        state++;
        if (state == cells.size())
        {
            state = 0;
        }
    }

    void reverseRotate()
    {
        state--;
        if (state == -1)
        {
            state = cells.size() - 1;
        }
    }

private:
    int cellSize;
    int state;
    vector<Color> colors;
    int Orow, Ocol;
}; // end of definition of base class

class LBlock : public Block
{
public:
    LBlock()
    {
        type = 1;
        cells[0] = {Position(0, 2), Position(1, 0), Position(1, 1), Position(1, 2)};
        cells[1] = {Position(0, 1), Position(1, 1), Position(2, 1), Position(2, 2)};
        cells[2] = {Position(1, 0), Position(1, 1), Position(1, 2), Position(2, 0)};
        cells[3] = {Position(0, 2), Position(1, 0), Position(1, 1), Position(1, 2)};
        Move(0, 3);
    }
};

class OBlock : public Block
{
public:
    OBlock()
    {
        type = 2;
        cells[0] = {Position(0, 0), Position(0, 1), Position(1, 0), Position(1, 1)};
        cells[1] = {Position(0, 0), Position(0, 1), Position(1, 0), Position(1, 1)};
        cells[2] = {Position(0, 0), Position(0, 1), Position(1, 0), Position(1, 1)};
        cells[3] = {Position(0, 0), Position(0, 1), Position(1, 0), Position(1, 1)};
        Move(0, 4);
    }
};

class ZBlock : public Block
{
public:
    ZBlock()
    {

        type = 3;
        cells[0] = {Position(0, 0), Position(0, 1), Position(1, 1), Position(1, 2)};
        cells[1] = {Position(0, 2), Position(1, 1), Position(1, 2), Position(2, 1)};
        cells[2] = {Position(1, 0), Position(1, 1), Position(2, 1), Position(2, 2)};
        cells[3] = {Position(0, 1), Position(1, 0), Position(1, 1), Position(2, 0)};
        Move(0, 3);
    }
};

class IBlock : public Block
{
public:
    IBlock()
    {

        type = 4;
        cells[0] = {Position(1, 0), Position(1, 1), Position(1, 2), Position(1, 3)};
        cells[1] = {Position(0, 2), Position(1, 2), Position(2, 2), Position(3, 2)};
        cells[2] = {Position(2, 0), Position(2, 1), Position(2, 2), Position(2, 3)};
        cells[3] = {Position(0, 1), Position(1, 1), Position(2, 1), Position(3, 1)};
        Move(-1, 3);
    }
};

class JBlock : public Block
{
public:
    JBlock()
    {
        type = 5;
        cells[0] = {Position(0, 0), Position(1, 0), Position(1, 1), Position(1, 2)};
        cells[1] = {Position(0, 1), Position(0, 2), Position(1, 1), Position(2, 1)};
        cells[2] = {Position(1, 0), Position(1, 1), Position(1, 2), Position(2, 2)};
        cells[3] = {Position(0, 1), Position(1, 1), Position(2, 0), Position(2, 1)};
        Move(0, 3);
    }
};

class SBlock : public Block
{
public:
    SBlock()
    {
        type = 6;
        cells[0] = {Position(0, 1), Position(0, 2), Position(1, 0), Position(1, 1)};
        cells[1] = {Position(0, 1), Position(1, 1), Position(1, 2), Position(2, 2)};
        cells[2] = {Position(1, 1), Position(1, 2), Position(2, 0), Position(2, 1)};
        cells[3] = {Position(0, 0), Position(1, 0), Position(1, 1), Position(2, 1)};
        Move(0, 3);
    }
};

class TBlock : public Block
{
public:
    TBlock()
    {
        type = 7;
        cells[0] = {Position(0, 1), Position(1, 0), Position(1, 1), Position(1, 2)};
        cells[1] = {Position(0, 1), Position(1, 1), Position(1, 2), Position(2, 1)};
        cells[2] = {Position(1, 0), Position(1, 1), Position(1, 2), Position(2, 1)};
        cells[3] = {Position(0, 1), Position(1, 0), Position(1, 1), Position(2, 1)};
        Move(0, 3);
    }
};

/////////////////////////////////
//                             //
//                             //
//      Resolution Size        //
//                             //
//                             //
//                             //
/////////////////////////////////

const int scrW = 500, scrH = 620; // Screen resolution

/////////////////////////////////
//                             //
//                             //
//     Central Game Class      //
//                             //
//                             //
//                             //
/////////////////////////////////

class Tetris
{
public:
    Tetris()
    {
        game = Board();
        allBlocks = GetBlocks();
        current = GetRandom();
        next = GetRandom();
        GameOver = false;
        score = 0;
    };
    Board game;
    Block GetRandom()
    {
        if (allBlocks.empty())
        {
            allBlocks = GetBlocks();
        }
        int random = rand() % allBlocks.size();
        Block curr = allBlocks[random];
        allBlocks.erase(allBlocks.begin() + random);
        return curr;
    }
    void DrawGame()
    {
        game.drawBoard();
        current.drawBlock(11, 11);
        next.drawBlock(270, 230);
    }
    void Update()
    {

        int key = GetKeyPressed();
        if (GameOver && key != 0)
        {
            GameOver = false;
            Reset();
        }
        switch (key)
        {
        case KEY_LEFT:
            MoveLeft();
            break;
        case KEY_RIGHT:
            MoveRight();
            break;
        case KEY_DOWN:
            MoveDown();
            break;
        case KEY_UP:
            RotateBlock();
            break;
        }
    }
    void MoveDown()
    {
        if (!GameOver)
        {
            current.Move(1, 0);
            UpdateScore(0, 1);
            if (isOut() || fits() == false)
            {
                current.Move(-1, 0);
                UpdateScore(0, -1);
                LockBlock();
            }
        }
    }
    bool GameOver;
    int score;
    Block next;

private:
    void UpdateScore(int c, int xpoints)
    {
        if (c == 1)
        {
            score += 100;
        }
        else if (c == 2)
        {
            score += 250;
        }
        else if (c == 3)
        {
            score += 500;
        }
        else if (c == 4)
        {
            score += 1000;
        }
        score += xpoints;
    }
    void Reset()
    {
        game.Initialise();
        allBlocks = GetBlocks();
        current = GetRandom();
        next = GetRandom();
    }
    void RotateBlock()
    {
        if (!GameOver)
        {
            current.Rotate();
            if (isOut() || fits() == false)
            {
                current.reverseRotate();
            }
        }
    }

    void MoveLeft()
    {
        if (!GameOver)
        {
            current.Move(0, -1);
            if (isOut() || fits() == false)
            {
                current.Move(0, 1);
            }
        }
    }
    void MoveRight()
    {
        if (!GameOver)
        {
            current.Move(0, 1);
            if (isOut() || fits() == false)
            {
                current.Move(0, -1);
            }
        }
    }
    vector<Block> allBlocks;
    vector<Block> GetBlocks()
    {
        return {LBlock(), OBlock(), ZBlock(), JBlock(), IBlock(), SBlock(), TBlock()};
    }
    Block current;

    bool fits()
    {
        vector<Position> tiles = current.GetPos();
        for (auto i : tiles)
        {
            if (game.isEmpty(i.x, i.y) == false)
            {
                return false;
            }
        }
        return true;
    }
    void LockBlock()
    {
        vector<Position> tiles = current.GetPos();
        for (auto i : tiles)
        {
            game.board[i.x][i.y] = current.type;
        }
        current = next;
        if (fits() == false)
        {
            GameOver = true;
        }
        next = GetRandom();
        int cleared = game.clearRows();
        UpdateScore(cleared, 0);
    }
    bool isOut()
    {
        vector<Position> tiles = current.GetPos();
        for (auto i : tiles)
        {
            if (game.Check(i.x, i.y))
            {
                return true;
            }
        }
        return false;
    }
};

/////////////////////////////////
//                             //
//          Timer for          //
//          movement           //
//                             //
//                             //
//                             //
/////////////////////////////////
double lastTime = 0.0;
bool event(double interval)
{
    double currTime = GetTime();
    if (currTime - lastTime >= interval)
    {
        lastTime = currTime;
        return true;
    }
    return false;
}
/////////////////////////////////
//                             //
//          Main Game          //
//                             //
//                             //
//                             //
//                             //
/////////////////////////////////

// Main Game Code
int main()
{
    InitWindow(scrW, scrH, "Tetris");
    SetTargetFPS(60);
    Tetris game;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DarkBlue);
        game.Update();

        char score[10];
        sprintf(score, "%d", game.score);
        DrawText("Score", 365, 20, 32, WHITE);
        DrawText("Next", 370, 175, 32, WHITE);

        DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, PURPLE);
        DrawRectangleRounded({320, 215, 170, 85}, 0.3, 6, PURPLE);
        DrawText(score, 390, 65, 32, WHITE);
        game.DrawGame();
        if (game.GameOver)
        {
            DrawText("Game Over", 320, 450, 32, RED);
        }
        if (event(0.2))
        {
            game.MoveDown();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}