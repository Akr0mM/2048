#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define W_WIDTH 700
#define W_HEIGHT 700

#define BLACK 0, 0, 0, 255
#define WHITE 255, 255, 255, 255
#define GREEN 100, 255, 0, 255
#define RED 255, 0, 0, 255
#define BG 187, 187, 187, 255
#define BOARD_BG (Color){184, 173, 160, 255}
#define DEATH_BG (Color){164, 153, 140, 255}
#define TILE (Color){204, 194, 184, 255}

typedef struct {
    Uint8 r, g, b, a;
} Color;

SDL_bool game_runs = SDL_TRUE;

SDL_Window *p_Window = NULL;
SDL_Renderer *p_Render = NULL;
SDL_Event event;

const int BOARD_XOFFSET = 100;
const int BOARD_YOFFSET = 100;
const int BOARD_WIDTH = 500;
const int BOARD_HEIGHT = 500;
const int GRID_GAP = 10;
const int TILES_WIDTH = (BOARD_WIDTH - 5 * GRID_GAP) / 4;
const int TILES_HEIGHT = (BOARD_HEIGHT - 5 * GRID_GAP) / 4;

void game_init(void);
void game_inputs(void);
void game_clean(void);
void draw_text(const char *text, int x, int y);
void rect(int x, int y, int width, int height, Color color);

void update(void);
void draw(void);
void moveDown(void);
void moveRight(void);
void moveUp(void);
void moveLeft(void);
void spawnTile(void);
void checkGameOver(void);
Color getTileColor(int value);

bool gameOver = false;
int board[4][4];
int input = 0;
int score = 0;

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    
    srand(time(NULL));

    game_init();

    board[rand() % 4][rand() % 4] = 2;

    while(game_runs)
    {
        game_inputs();

        update();
        draw();

        SDL_RenderPresent(p_Render);
        SDL_Delay(100);
    }

    game_clean();
    return 0;
}

void update(void) {
    if (!gameOver) {
        if (input == 1) { // DOWN
            moveDown();
        } else if (input == 2) { // RIGHT
            moveRight();
        } else if (input == -1) { // UP
            moveUp();
        } else if (input == -2) { // LEFT
            moveLeft();
        }

        if (input != 0) spawnTile();
    }

    // reset input
    input = 0;

    checkGameOver();
}

void checkGameOver() {
    bool hasEmpty = false;  // Vérifie s'il reste des cases vides
    bool canMerge = false;  // Vérifie si des fusions sont possibles

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) {
                hasEmpty = true;  // Une case vide a été trouvée
                break;
            }

            // Vérifier les fusions possibles avec les voisins
            if ((i < 3 && board[i][j] == board[i + 1][j]) ||  // Fusion possible en bas
                (j < 3 && board[i][j] == board[i][j + 1])) {  // Fusion possible à droite
                canMerge = true;
            }
        }
        if (hasEmpty) break;  // Pas besoin de continuer si une case vide est trouvée
    }

    // Si aucune case vide et aucune fusion possible, le jeu est terminé
    if (!hasEmpty && !canMerge) {
        gameOver = true;
    }
}


void spawnTile() {
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


void moveDown() {
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
                    score += board[k + 1][j];
                    board[k][j] = 0;
                    last_merged = k + 1; // Met à jour last_merged
                }
            }
        }
    }
}

void moveRight() {
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
                    score += board[i][k + 1];
                    board[i][k] = 0;
                    last_merged = k + 1; // Met à jour last_merged
                }
            }
        }
    }
}

void moveUp() {
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
                    score += board[k - 1][j];
                    board[k][j] = 0;
                    last_merged = k - 1; // Met à jour last_merged
                }
            }
        }
    }
}


void moveLeft() {
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
                    score += board[i][k - 1];
                    board[i][k] = 0;
                    last_merged = k - 1; // Met à jour last_merged
                }
            }
        }
    }
}

Color getTileColor(int value) {
    switch (value) {
        case 2:    return (Color){238, 228, 218, 255};
        case 4:    return (Color){237, 224, 200, 255};
        case 8:    return (Color){242, 177, 121, 255};
        case 16:   return (Color){245, 149, 99, 255};
        case 32:   return (Color){246, 124, 95, 255};
        case 64:   return (Color){246, 94, 59, 255};
        case 128:  return (Color){237, 207, 114, 255};
        case 256:  return (Color){237, 204, 97, 255};
        case 512:  return (Color){237, 200, 80, 255};
        case 1024: return (Color){237, 197, 63, 255};
        case 2048: return (Color){237, 194, 46, 255};
        case 4096: return (Color){0, 0, 0, 255}; // Noir total pour 4096
        default:   // Intensité croissante de noir pour les tuiles supérieures à 4096
            if (value > 4096) {
                int intensity = 255 - (value / 4096) * 10; // Réduire la luminosité avec un pas de 10
                if (intensity < 0) intensity = 0;         // Cap pour éviter une luminosité négative
                return (Color){intensity, intensity, intensity, 255};
            }
            return (Color){204, 194, 184, 255}; // Couleur par défaut pour 0 ou inconnus
    }
}



void draw() {
    // clear bg
    SDL_SetRenderDrawColor(p_Render, BG);
    SDL_RenderClear(p_Render);

    // board
    rect(BOARD_XOFFSET, BOARD_YOFFSET, BOARD_WIDTH, BOARD_HEIGHT, BOARD_BG);

    // tiles
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int x = BOARD_XOFFSET + GRID_GAP + j * (TILES_WIDTH + GRID_GAP);
            int y = BOARD_YOFFSET + GRID_GAP + i * (TILES_WIDTH + GRID_GAP);
            rect(x, y, TILES_WIDTH, TILES_HEIGHT, getTileColor(board[i][j]));

            if (board[i][j] != 0) { // Si la valeur n'est pas 0, on affiche
                char text[10];
                sprintf(text, "%d", board[i][j]); // Convertit l'entier en chaîne
                draw_text(text, x + TILES_WIDTH / 2 - 10, y + TILES_HEIGHT / 2 - 10);
            }
        }
    }

    // score
    char scoreText[50]; // Taille suffisante pour contenir "Score : " et un grand nombre
    sprintf(scoreText, "Score : %d", score); // Formate la chaîne avec le score
    draw_text(scoreText, 10, 10); // Affiche le texte à la position (10, 10)


    if (gameOver) {
        rect(150, 150, 400, 400, DEATH_BG);

        char gameOverText[50]; 
        sprintf(gameOverText, "Game Over !");
        draw_text(gameOverText, 265, 200);

        char finalScore[50]; 
        sprintf(finalScore, "Score : %d", score);
        draw_text(finalScore, 270, 300);
    }
}


void rect(int x, int y, int width, int height, Color color) {
    // Définir la couleur du rendu avec SDL
    SDL_SetRenderDrawColor(p_Render, color.r, color.g, color.b, color.a);
    
    // Créer le rectangle
    SDL_Rect rectangle = {x, y, width, height};

    // Dessiner le rectangle
    SDL_RenderFillRect(p_Render, &rectangle);
}

void draw_text(const char *text, int x, int y)
{
    TTF_Font *font = TTF_OpenFont("Poppins-Medium.ttf", 24);  // Remplace par le chemin de ta police
    if (!font) {
        SDL_Log("Error loading font: %s", TTF_GetError());
        return;
    }

    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        SDL_Log("Error creating text surface: %s", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(SDL_GetRenderer(SDL_GetWindowFromID(1)), surface);
    if (!texture) {
        SDL_Log("Error creating text texture: %s", SDL_GetError());
    }

    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(SDL_GetRenderer(SDL_GetWindowFromID(1)), texture, NULL, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}


void game_init(void)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("Error SDL Init: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    p_Window = SDL_CreateWindow("2048", 
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                W_WIDTH, W_HEIGHT,
                                SDL_WINDOW_SHOWN);
    p_Render = SDL_CreateRenderer(p_Window, -1, SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);

    if(!p_Window || !p_Render) {
        SDL_Log("Error SDL window and renderer: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Initialiser SDL_ttf pour pouvoir charger des polices
    if (TTF_Init() == -1) {
        SDL_Log("Error initializing SDL_ttf: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

void game_inputs(void)
{
    while(SDL_PollEvent(&event)) {
        if(SDL_QUIT == event.type) game_runs = SDL_FALSE;
        if(SDL_KEYDOWN == event.type) {
            switch(event.key.keysym.sym) {
                case SDLK_ESCAPE: 
                    game_runs = SDL_FALSE;
                    break;
                case SDLK_LEFT:
                    input = -2;
                    break;
                case SDLK_UP:
                    input = -1;
                    break;
                case SDLK_DOWN:
                    input = 1;
                    break;
                case SDLK_RIGHT:
                    input = 2;
                    break;
            }
        }
    }
}

void game_clean(void)
{
    SDL_DestroyRenderer(p_Render);
    SDL_DestroyWindow(p_Window);
    SDL_Quit();
}
