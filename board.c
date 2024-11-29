#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int score = 0;

void moveDown(int board[4][4], bool addScore);
void moveRight(int board[4][4], bool addScore);
void moveUp(int board[4][4], bool addScore);
void moveLeft(int board[4][4], bool addScore);
void spawnTile(int board[4][4]);
bool checkGameOver(int board[4][4]);

bool checkGameOver(int board[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) {
                return false;
                break;
            }

            // Vérifier les fusions possibles avec les voisins
            if ((i < 3 && board[i][j] == board[i + 1][j]) ||  // Fusion possible en bas
                (j < 3 && board[i][j] == board[i][j + 1])) {  // Fusion possible à droite
                return false;
            }
        }
    }

    return true;
}


void spawnTile(int board[4][4]) {
    int empty[16][2];
    int count = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) {
                empty[count][0] = i;
                empty[count][1] = j;
                count++;
            }
        }
    }

    if (count > 0) {
        int random = rand() % count;
        int x = empty[random][0];
        int y = empty[random][1];
        board[x][y] = (rand() % 10 == 0) ? 4 : 2; // 10% chance pour un 4
    }
}


void moveDown(int board[4][4], bool addScore) {
    for (int j = 0; j < 4; j++) { // Parcourt chaque colonne
        int last_merged = 4; // Initialise à une valeur hors limite (4)
        for (int i = 2; i >= 0; i--) { // Parcourt chaque ligne de bas en haut
            if (board[i][j] != 0) { // Si la case n'est pas vide
                int k = i;
                while (k < 3 && board[k + 1][j] == 0) { // Déplace vers le bas
                    board[k + 1][j] = board[k][j];
                    board[k][j] = 0;
                    k++;
                }
                // Fusionne si possible
                if (k < 3 && board[k + 1][j] == board[k][j] && k + 1 < last_merged) {
                    board[k + 1][j] *= 2;
                    if (addScore) score += board[k + 1][j];
                    board[k][j] = 0;
                    last_merged = k + 1; // Met à jour last_merged
                }
            }
        }
    }
}

void moveRight(int board[4][4], bool addScore) {
    for (int i = 0; i < 4; i++) {
        int last_merged = 4; // Initialise à une valeur hors limite (4)
        for (int j = 2; j >= 0; j--) {
            if (board[i][j] != 0) {
                int k = j;
                while (k < 3 && board[i][k + 1] == 0) { // Déplace à droite
                    board[i][k + 1] = board[i][k];
                    board[i][k] = 0;
                    k++;
                }
                // Fusionne si possible
                if (k < 3 && board[i][k + 1] == board[i][k] && k + 1 < last_merged) {
                    board[i][k + 1] *= 2;
                    if (addScore) score += board[i][k + 1];
                    board[i][k] = 0;
                    last_merged = k + 1; // Met à jour last_merged
                }
            }
        }
    }
}

void moveUp(int board[4][4], bool addScore) {
    for (int j = 0; j < 4; j++) {
        int last_merged = -1; // Initialise à une valeur hors limite (-1)
        for (int i = 1; i < 4; i++) {
            if (board[i][j] != 0) {
                int k = i;
                while (k > 0 && board[k - 1][j] == 0) { // Déplace vers le haut
                    board[k - 1][j] = board[k][j];
                    board[k][j] = 0;
                    k--;
                }
                // Fusionne si possible
                if (k > 0 && board[k - 1][j] == board[k][j] && k - 1 > last_merged) {
                    board[k - 1][j] *= 2;
                    if (addScore) score += board[k - 1][j];
                    board[k][j] = 0;
                    last_merged = k - 1; // Met à jour last_merged
                }
            }
        }
    }
}


void moveLeft(int board[4][4], bool addScore) {
    for (int i = 0; i < 4; i++) {
        int last_merged = -1; // Initialise à une valeur hors limite (-1)
        for (int j = 1; j < 4; j++) {
            if (board[i][j] != 0) {
                int k = j;
                while (k > 0 && board[i][k - 1] == 0) { // Déplace à gauche
                    board[i][k - 1] = board[i][k];
                    board[i][k] = 0;
                    k--;
                }
                // Fusionne si possible
                if (k > 0 && board[i][k - 1] == board[i][k] && k - 1 > last_merged) {
                    board[i][k - 1] *= 2;
                    if (addScore) score += board[i][k - 1];
                    board[i][k] = 0;
                    last_merged = k - 1; // Met à jour last_merged
                }
            }
        }
    }
}