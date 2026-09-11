#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#define PIECE_SIZE 4
#define WIDTH 11
#define HEIGHT 21
#define NUM_PIECES 7

// Utility
struct object
{
    int x;
    int y;
};
static struct termios orig_termios;
int isRunning = 1;

int randomXPosition()
{
    return 1 + rand() % (WIDTH - 2 - PIECE_SIZE);
}
int randomPiece()
{
    return rand() % NUM_PIECES;
}

char pieces[NUM_PIECES][PIECE_SIZE * PIECE_SIZE + 1] = {
    // + for for null terminator '\0'
    "...."
    ".XX."
    ".XX."
    "....", // index 0: O piece

    "...."
    ".X.."
    "XXX."
    "....", // index 1: T piece

    "...."
    "..X."
    "..X."
    "..X.", // 2: I piece

    "...."
    ".XX."
    "..X."
    "..X.", // 3: S piece

    "...."
    "..X."
    ".XX."
    ".X..", // 4: Z piece

    "...."
    ".X.."
    ".X.."
    ".XX.", // 5: L piece

    "...."
    "..X."
    "..X."
    ".XX.", // 6: J piece
};

// Board

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
        board[row][WIDTH - 1] = '|';
    }
}

void checkBoardRows(char lockedBoard[HEIGHT][WIDTH])
{
    // so we start outer loop that equals 13 with i--
    // create isfull variable
    // inner loop that equals one and is less than width - 1
    // on each iteration we check if cells fo board euqls #
    // if they dont we make is full 0
    // we have another check after row finishes that checks if is full is 1
    // if it is 1 we have another double loop that iterates board from top to bottom again
    // on each iteration of inner col loop we make current row equal to row below it
    for (int i = HEIGHT - 2; i > 1; i--)
    {
        int isFull = 1;
        for (int j = 1; j < WIDTH - 1; j++)
        {
            if (lockedBoard[i][j] != '#')
            {
                isFull = 0;
            }
        }
        if (isFull == 1)
        {
            for (int row = i; row > 1; row--)
            {
                for (int col = 1; col < WIDTH - 1; col++)
                {
                    lockedBoard[row][col] = lockedBoard[row - 1][col];
                }
            }
        }
    }
}
// Objects

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
                    return -1;
                }
            }
        }
    }
    return 1;
}

int objectFall(char lockedBoard[HEIGHT][WIDTH], char piece[16], struct object *obj)
{
    // checking if next block is avaialable for falling object
    if (checkPiece(lockedBoard, piece, obj->x, obj->y + 1) == 1)
    {

        obj->y++;

        return 1;
    }
    else
    {
        // if object stops falling we update locked board.

        fillPiece(lockedBoard, piece, obj->x, obj->y);
        return -1;
    }
}
int objectRight(char lockedBoard[HEIGHT][WIDTH], char piece[16], struct object *obj)
{
    if (checkPiece(lockedBoard, piece, obj->x + 1, obj->y) == 1)
    {

        obj->x++;
        return 1;
    }
    else
    {
        // if object stops falling we update locked board.
        fillPiece(lockedBoard, piece, obj->x, obj->y);
        return -1;
    }
}
int objectLeft(char lockedBoard[HEIGHT][WIDTH], char piece[16], struct object *obj)
{
    if (checkPiece(lockedBoard, piece, obj->x - 1, obj->y) == 1)
    {

        obj->x--;
        return 1;
    }
    else
    {
        // if object stops falling we update locked board.
        fillPiece(lockedBoard, piece, obj->x, obj->y);
        return -1;
    }
}

int rotate90(char piece[16])
{
    char tempPiece[16] = "...............";
    // loop throu piece find where Xs are and generate new index with 90 degree formula.
    // creates temp array
    for (int py = 0; py < 4; py++)
    {
        for (int px = 0; px < 4; px++)
        {
            if (piece[py * 4 + px] == 'X')
            {
                int newIndex = px * 4 + (3 - py);
                tempPiece[newIndex] = piece[py * 4 + px];
            }
        }
    }
    // ovveriting old array with new elements
    for (int i = 0; i < 16; i++)
    {
        piece[i] = tempPiece[i];
    }

    return 1;
}

// RENDER

void rebuildDisplayBoard(char lockedBoard[HEIGHT][WIDTH], char displayBoard[HEIGHT][WIDTH])
{
    for (int r = 0; r < HEIGHT; r++)
    {
        for (int c = 0; c < WIDTH; c++)
        {
            displayBoard[r][c] = lockedBoard[r][c];
        }
    }
}
void renderFrame(char lockedBoard[HEIGHT][WIDTH], char displayBoard[HEIGHT][WIDTH], char piece[16], struct object *obj)
{
    // rebuilding display board here
    rebuildDisplayBoard(lockedBoard, displayBoard);

    // putting new piece into display board
    fillPiece(displayBoard, piece, obj->x, obj->y);

    printf("\033[H\033[J");
    printBoard(displayBoard);
    usleep(99999);
}

// TERMINAL

char checkUserInput(char c)
{
    if (c == 'a')
    {
        return 'a';
    }
    if (c == 'd')
    {
        return 'd';
    }
    if (c == 'w')
    {
        return 'w';
    }
    if (c == 's')
    {
        return 's';
    }
    return ' ';
}

void enableRawSettings()
{
    struct termios raw = orig_termios;
    /* input modes - clear indicated ones giving: no break, no CR to NL,
       no parity check, no strip char, no start/stop output (sic) control */
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    /* control modes - set 8 bit chars */
    raw.c_cflag |= (CS8);

    /* local modes - clear giving: echoing off, canonical off (no erase with
       backspace, ^U,...),  no extended functions, no signal chars (^Z,^C) */
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    /* control chars - set return condition: min number of bytes and timer */
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 5; /* after a byte or .8 seconds */

    /* put terminal in raw mode after flushing */
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0)
        error("can't set raw mode");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) != -1)
    {
        enableRawSettings();
    }
    else
    {
        error("error while setting original termios");
    }
}

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) != -1)
    {
        printf("SAYONARA <33\n");
    }
    else
    {
        error("error on disable raw mode function.");
    }
}

int userControllPanel(char lockedBoard[HEIGHT][WIDTH], char displayBoard[HEIGHT][WIDTH], char userInput, int pieceIndex, struct object *obj)
{
    for (int row = 0; row < HEIGHT; row++)
    {
        renderFrame(lockedBoard, displayBoard, pieces[pieceIndex], obj);
        checkBoardRows(lockedBoard);
        if (objectFall(lockedBoard, pieces[pieceIndex], obj) == -1)
        {
            break;
        }

        if (read(STDIN_FILENO, &userInput, 1) != 0)
        {
            if (userInput == 'q')
            {
                disableRawMode();
                isRunning = 0;
                return -1;
            }
            char key = checkUserInput(userInput);
            switch (key)
            {
            case 'a':
                objectLeft(lockedBoard, pieces[pieceIndex], obj);
                break;
            case 'd':
                objectRight(lockedBoard, pieces[pieceIndex], obj);
                break;

            case 's':
                objectFall(lockedBoard, pieces[pieceIndex], obj);
                break;
            case 'w':
                rotate90(pieces[pieceIndex]);
                break;

            default:
                return -1;
                break;
            }
        }
    }

    return 1;
}

int main()
{

    char lockedBoard[HEIGHT][WIDTH];
    char displayBoard[HEIGHT][WIDTH];
    initBoard(lockedBoard);
    srand(time(NULL));
    enableRawMode();
    char userInput = ' ';

    while (isRunning == 1)
    {
        struct object obj;
        obj.y = 0;
        obj.x = randomXPosition();
        int pieceIndex = randomPiece();
        tcflush(STDIN_FILENO, TCIFLUSH); // this func is used to clear input queue.
        if (userControllPanel(lockedBoard, displayBoard, userInput, pieceIndex, &obj) != -1)
        {

            isRunning = 1;
        }
        else
        {
            isRunning = 0;
            disableRawMode();
            return 0;
        }
    }
    return 0;
}