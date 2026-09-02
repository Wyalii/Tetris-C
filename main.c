#include <stdio.h>
#include <unistd.h>
#define WIDTH 30
#define HEIGHT 15

// Board

void clearBoard(char board[HEIGHT][WIDTH])
{
    for (int row = 1; row < HEIGHT - 1; row++)
    {
        for (int col = 1; col < WIDTH - 1; col++)
        {
            board[row][col] = ' ';
        }
    }
}
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

    printBoard(board);
}

// Objects

int checkBlock(char board[HEIGHT][WIDTH], int x, int y)
{
    if (board[y + 1][x] == '#')
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

void renderObject(char board[HEIGHT][WIDTH], int x, int y)
{
    printf("\033[H\033[J");
    fillBlock(board, x, y);
    usleep(100000);
}

void moveObject(char lockedBoard[HEIGHT][WIDTH], char displayBoard[HEIGHT][WIDTH], int x, int y)
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
        renderObject(displayBoard, x, y);

        // printing display board
        renderFrame(displayBoard);

        // checking if next block is avaialable for falling object
        if (checkBlock(lockedBoard, x, y) == 0)
        {
            y++;
        }
        else
        {
            return;
        }
    }
}

int main()
{
    char lockedBoard[HEIGHT][WIDTH];
    char displayBoard[HEIGHT][WIDTH];
    initBoard(lockedBoard);
    moveObject(lockedBoard, displayBoard, 1, 1);

    return 0;
}