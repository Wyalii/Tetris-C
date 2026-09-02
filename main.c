#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define PIECE_SIZE 4
#define WIDTH 30
#define HEIGHT 15

// Utility
int randomXPosition()
{
    return 1 + rand() % (WIDTH - 2);
}

// Board

// void clearBoard(char board[HEIGHT][WIDTH])
// {
//     for (int row = 1; row < HEIGHT - 1; row++)
//     {
//         for (int col = 1; col < WIDTH - 1; col++)
//         {
//             board[row][col] = ' ';
//         }
//     }
// }

void printBoard(char board[HEIGHT][WIDTH])
{
    for (int row = 0; row < HEIGHT; row++)
    {
        for (int col = 0; col < WIDTH; col++)
        {
            printf("%c", board[row][col]);
        }

        printf("\n");
    }
}

void initBoard(char board[HEIGHT][WIDTH])
{
    for (int row = 0; row < HEIGHT; row++)
    {
        for (int col = 0; col < WIDTH; col++)
        {
            board[row][col] = ' ';
            if (row == 0 || row == HEIGHT - 1)
            {
                board[row][col] = '-';
            }
        }
    }

    for (int row = 0; row < HEIGHT; row++)
    {
        board[row][0] = '|';
        board[row][29] = '|';
    }
}

// RENDER

void renderFrame(char board[HEIGHT][WIDTH])
{
    printf("\033[H\033[J");
    printBoard(board);
    usleep(99999);
}

// Objects

int checkBlock(char board[HEIGHT][WIDTH], int x, int y)
{
    if (board[y + 1][x] == '#' || board[y + 1][x] == '-')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void fillBlock(char board[HEIGHT][WIDTH], int x, int y)
{
    board[y][x] = '#';
}
void fillPiece(char board[HEIGHT][WIDTH], char piece[16], int x, int y)
{
    for (int py = 0; py < 4; py++)
    {
        for (int px = 0; px < 4; px++)
        {
            if (piece[py * 4 + px] == 'X')
            {
                board[y + py][x + px] = '#';
            }
        }
    }
}

int checkPiece(char board[HEIGHT][WIDTH], char piece[16], int x, int y)
{
    for (int py = 0; py < 4; py++)
    {
        for (int px = 0; px < 4; px++)
        {
            if (piece[py * 4 + px] == 'X')
            {
                if (board[y + py][x + px] != ' ')
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void lockPiece(char board[HEIGHT][WIDTH], char piece[16], int x, int y)
{
    fillPiece(board, piece, x, y);
}

int moveObject(char lockedBoard[HEIGHT][WIDTH], char displayBoard[HEIGHT][WIDTH], char piece[16], int x, int y)
{
    for (int row = 0; row < HEIGHT; row++)
    {
        // rebuilding display board here
        for (int r = 0; r < HEIGHT; r++)
        {
            for (int c = 0; c < WIDTH; c++)
            {
                displayBoard[r][c] = lockedBoard[r][c];
            }
        }

        // putting new piece into display board
        fillPiece(displayBoard, piece, x, y);

        // printing display board
        renderFrame(displayBoard);

        // checking if next block is avaialable for falling object
        if (checkPiece(lockedBoard, piece, x, y + 1) == 0)
        {
            y++;
        }
        else
        {
            // if object stops falling we update locked board.
            lockPiece(lockedBoard, piece, x, y);
            return 1;
        }
    }
    return 0;
}

int main()
{
    char lockedBoard[HEIGHT][WIDTH];
    char displayBoard[HEIGHT][WIDTH];
    initBoard(lockedBoard);
    char oPiece[PIECE_SIZE * PIECE_SIZE + 1] =
        "...."
        ".XX."
        ".XX."
        "....";
    int result = moveObject(lockedBoard, displayBoard, oPiece, randomXPosition(), 1);

    return 0;
}