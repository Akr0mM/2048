#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int makeMove(int status[4][4]);
int evaluation(int board[4][4]);
int minmax(int board[4][4], int level);

int makeMove(int status[4][4]) 
{
    int deep = 1;
    int board[4][4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            board[i][j] = status[i][j];
        }
    }

    int moves[4] = { 0, 0, 0, 0 };

    for (int dir = 0; dir < 4; dir++)
    {
        int lastBoard[4][4];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                lastBoard[i][j] = board[i][j];
            }
        }

        if (dir == 0) moveDown(board, false);
        else if (dir == 1) moveLeft(board, false);
        else if (dir == 2) moveUp(board, false);
        else if (dir == 3) moveRight(board, false); 

        int eval = minmax(board, deep);
        // int eval = evaluation(board);
        
        if (eval > moves[dir])
        {
            moves[dir] = eval;
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                board[i][j] = lastBoard[i][j];
            }
        }
    }

    int maxIndex = 0;
    for (int i = 1; i < 4; i++) {
        if (moves[i] > moves[maxIndex]) {
            maxIndex = i;
        }
    }

    if (maxIndex == 0) return 1;
    if (maxIndex == 1) return -2;
    if (maxIndex == 2) return 1;
    if (maxIndex == 3) return 2;
}

int minmax(int board[4][4], int level)
{
    if (level == 0)
    {
        return evaluation(board);
    }

    int moves[4] = { 0, 0, 0, 0 };

    for (int dir = 0; dir < 4; dir++)
    {
        int lastBoard[4][4];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                lastBoard[i][j] = board[i][j];
            }
        }

        if (dir == 0) moveDown(board, false);
        else if (dir == 1) moveLeft(board, false);
        else if (dir == 2) moveUp(board, false);
        else if (dir == 3) moveRight(board, false);

        int eval = minmax(board, level - 1);
        
        if (eval > moves[dir])
        {
            moves[dir] = eval;
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                board[i][j] = lastBoard[i][j];
            }
        }
    }

    int max = moves[0];
    for (int i = 1; i < 4; i++) {
        if (moves[i] > max) {
            max = moves[i];
        }
    }

    return max;
}

int evaluation(int board[4][4])
{
    int table[4][4] = { { 1, 4, 16, 64 },
                        { 16384, 4096, 1024, 256 },
                        { 16384, 65536, 262144, 1048576, },
                        { 1073741824, 268435456, 67108864, 16777216 }
                      };

    int point = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            point += table[i][j] * board[i][j];

        }
    }

    return point;
};
