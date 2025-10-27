// COMP3016 CW1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL3/SDL.h>


int main(int argc, char* argv[])
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Window* window = SDL_CreateWindow("COMP3016 CW1", 600, 400, SDL_WINDOW_RESIZABLE);
	if (!window) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) {
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_FRect playerF = { 50.0f, 50.0f, 50.0f, 50.0f };
	const float playerSpeed = 5.0f;
	uint64_t lastTicks = SDL_GetTicksNS();
	int tileSize = 50;

	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//SDL_RenderClear(renderer);
	//SDL_RenderPresent(renderer);
	//SDL_Delay(3000);

	bool moving = true;
	SDL_Event event;

	const bool* key = SDL_GetKeyboardState(nullptr);
	while (moving) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				moving = false;
			}
		}
		
		SDL_PumpEvents();
		key = SDL_GetKeyboardState(nullptr);
		uint64_t now = SDL_GetTicksNS();
		float dt = (now - lastTicks) / 1000.0f;
		lastTicks = now;
		
		if (key[SDL_SCANCODE_W] || key[SDL_SCANCODE_UP]) {
			playerF.y -= playerSpeed;
		}
		if (key[SDL_SCANCODE_S] || key[SDL_SCANCODE_DOWN]) {
			playerF.y += playerSpeed;
		}
		if (key[SDL_SCANCODE_A] || key[SDL_SCANCODE_LEFT]) {
			playerF.x -= playerSpeed;
		}
		if (key[SDL_SCANCODE_D] || key[SDL_SCANCODE_RIGHT]) {
			playerF.x += playerSpeed;
		}

		SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &playerF);
		SDL_RenderPresent(renderer);

		SDL_Delay(16); // ~60 FPS
	}

	

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

    std::cout << "Hello World!\n";
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
