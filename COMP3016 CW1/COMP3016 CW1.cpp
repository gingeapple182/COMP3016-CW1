// COMP3016 CW1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Game.h"

int main(int argc, char* argv[])
{
	// SDL initialisation
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!TTF_Init()) {
        std::cerr << "TTF_Init Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

	// Window creation
    int windowWidth = 1000;
    int windowHeight = 750;

    SDL_Window* window = SDL_CreateWindow("COMP3016 CW1", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

	// Renderer creation
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

	// load font

    TTF_Font* font = TTF_OpenFont("Pixellari.ttf", 30);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

	// Game initialisation - game setup moved to game.cpp

    Game game(renderer, font, windowWidth, windowHeight);

    // -- Game loop --

    SDL_Event event;
    bool running = true;
    uint64_t lastTicks = SDL_GetTicksNS();

    while (game.isRunning() && running) {

        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Calculate delta time
        uint64_t now = SDL_GetTicksNS();
        float dt = (now - lastTicks) / 100000000.0f; // 11 0's
        lastTicks = now;

        // Update and render the game
        game.update(dt);
        game.render();

        SDL_Delay(16); // ~60 FPS
    }

	// Cleanup

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    std::cout << "Game closed\n";
    return 0;
}
