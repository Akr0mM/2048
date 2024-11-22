#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define W_WIDTH 2000
#define W_HEIGHT 1000

#define BLACK 0, 0, 0, 255
#define WHITE 255, 255, 255, 255
#define GREEN 100, 255, 0, 255
#define RED 255, 0, 0, 255

SDL_bool game_runs = SDL_TRUE;

SDL_Window *p_Window = NULL;
SDL_Renderer *p_Render = NULL;
SDL_Event event;

void game_init(void);
void game_inputs(void);
void game_clean(void);
void draw_text(const char *text, int x, int y);

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    
    srand(time(NULL));

    // nb elements
    const int nb = 300; 
    int arr[nb];

    const int delay = 20;

    // init array 1 to nb
    for (int i = 1; i <= nb; i++) {
        arr[i - 1] = i;
    }

    // shuffle array
    for (int i = nb - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    // space between bar 
    const int space = 0; 

    // width and height of bar
    int width = (W_WIDTH - 40) / nb - space;
    if (width < 1) width = 1;
    const int height = (W_HEIGHT - 40) / nb;

    // verif
    bool verif = false;
    int verifCount = 1;

    // exchange count every swap / actions
    int exchange = 0;

    game_init();

    while(game_runs)
    {
        game_inputs();

        SDL_SetRenderDrawColor(p_Render, BLACK);
        SDL_RenderClear(p_Render);

        if (!verif) {
            // draw bar white
            for (int i = 0; i < nb; i++) {
                SDL_Rect square = { 50 + i * (width + space), W_HEIGHT - 20 - arr[i] * height, width, arr[i] * height }; 
                SDL_SetRenderDrawColor(p_Render, WHITE);
                SDL_RenderFillRect(p_Render, &square);
            }
            
            int count = 0;

            //! Sort the bars (Bubble sort here)
            for (int i = 0; i < nb - 1; i++) {
                if (arr[i] > arr[i + 1]) {
                    count++;
                    int temp = arr[i];
                    arr[i] = arr[i + 1];
                    arr[i + 1] = temp;
                }
            }

            exchange += count;

            // verif if all correct
            if (count == 0) {
                verif = true;
            }
        } else {
            for (int i = 0; i < verifCount; i++) {
                // draw bar green if correct else red
                SDL_Rect square = { 50 + i * (width + space), W_HEIGHT - 20 - arr[i] * height, width, arr[i] * height }; 
                if (arr[i] <= i + 1) {
                    SDL_SetRenderDrawColor(p_Render, GREEN); 
                } else {
                    SDL_SetRenderDrawColor(p_Render, RED); 
                }
                SDL_RenderFillRect(p_Render, &square);
            }

            // draw bar not corrected yet
            for (int i = verifCount; i < nb; i++) {
                SDL_Rect square = { 50 + i * (width + space), W_HEIGHT - 20 - arr[i] * height, width, arr[i] * height }; 
                SDL_SetRenderDrawColor(p_Render, WHITE);
                SDL_RenderFillRect(p_Render, &square);
            }

            if (verifCount < nb) {
                verifCount++;
            }
        }

        // nb of bars
        char nbBar[20];
        sprintf(nbBar, "Nb: %d", nb);
        draw_text(nbBar, 10, 10);

        // nb of exchange
        char nbExchange[20];
        sprintf(nbExchange, "Exchange: %d", exchange);
        draw_text(nbExchange, 250, 10);

        // delay
        char textDelay[20];
        sprintf(textDelay, "Delay: %d", delay);
        draw_text(textDelay, 500, 10);

        SDL_RenderPresent(p_Render);
        SDL_Delay(delay);
    }

    game_clean();
    return 0;
}

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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

    p_Window = SDL_CreateWindow("Sorting Algorithm", 
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
                case SDLK_ESCAPE: game_runs = SDL_FALSE;
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
