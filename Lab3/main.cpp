#include <iostream>
#include <SDL.h>
#include "glm/glm.hpp"
//#include <vector>
#include "color.h"
//#include "framebuffer.h"
//#include "triangle.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void render(SDL_Renderer* renderer) {
    // Limpiar la pantalla con un color negro
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("life", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
    );

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Llamar a nuestra función render para dibujar la escena
        render(renderer);

        // Presentar el framebuffer en la pantalla
        SDL_RenderPresent(renderer);

        // Retrasar para limitar el framerate
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}