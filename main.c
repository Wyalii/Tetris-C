#include <stdio.h>
#define WIDTH 10
#define HEIGHT 20
int main()
{
    char board[HEIGHT][WIDTH];

    for (int row = 0; row < 5; row++)
    {
        // left wall
        printf("|");
        for (int col = 0; col < 10; col++)
        {
            if (row == 0 || row == 4)
            {
                printf("-");
            }
            else
            {
                printf(" ");
            }
        }
        printf("|");
        printf("\n");

        // right walll
    }

    return 0;
}