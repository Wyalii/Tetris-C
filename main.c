#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
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

void fillOPiece(char board[HEIGHT][WIDTH], int x, int y)
{
    board[y][x] = '#';
    board[y][x + 1] = '#';
    board[y + 1][x] = '#';
    board[y + 1][x + 1] = '#';
}

int moveObject(char lockedBoard[HEIGHT][WIDTH], char displayBoard[HEIGHT][WIDTH], int x, int y)
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
        fillOPiece(displayBoard, x, y);

        // printing display board
        renderFrame(displayBoard);

        // checking if next block is avaialable for falling object
        if (checkBlock(lockedBoard, x, y + 1) == 0)
        {
            y++;
        }
        else
        {
            // if object stops falling we update locked board.
            printf("Landed at row: %d\n", y);
            fillBlock(lockedBoard, x, y);
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
    int result = moveObject(lockedBoard, displayBoard, randomXPosition(), 1);
    printf("Piece landed: %d\n", result);

    return 0;
}