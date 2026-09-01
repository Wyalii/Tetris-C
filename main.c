#include <stdio.h>
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
void initBoard(char board[HEIGHT][WIDTH])
{
    for (int row = 0; row < HEIGHT; row++)
    {
        for (int col = 0; col < WIDTH; col++)
        {
            board[row][col] = ' ';
            if (row == HEIGHT - 1)
            {
                board[row][col] = '-';
            }
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
int main()
{
    char board[HEIGHT][WIDTH];
    initBoard(board);
    printBoard(board);

    return 0;
}