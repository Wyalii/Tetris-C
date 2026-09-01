#include <stdio.h>
#include <unistd.h>
#define WIDTH 30
#define HEIGHT 15
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
void initBoard(char board[HEIGHT][WIDTH])
{
    for (int row = 0; row < HEIGHT; row++)
    {
        for (int col = 0; col < WIDTH; col++)
        {
            board[row][col] = ' ';
            if (row == 0)
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

void movePieces(char board[HEIGHT][WIDTH])
{
    int pieceX = 1;
    int pieceY = 1;
    for (int row = 0; row < HEIGHT; row++)
    {
        clearBoard(board);
        board[pieceY][pieceX] = '#';
        printf("\033[H\033[J");
        printBoard(board);
        pieceY++;
        usleep(100000);
    }
}

int main()
{
    char board[HEIGHT][WIDTH];
    initBoard(board);
    movePieces(board);

    return 0;
}