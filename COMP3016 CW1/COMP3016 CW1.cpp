// COMP3016 CW1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL3/SDL.h>


int main(int argc, char* argv[])
{
	// Initialize SDL
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Create window and renderer
	int windowWidth = 1000;
	int	windowHeight = 750;

	SDL_Window* window = SDL_CreateWindow("COMP3016 CW1", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
	if (!window) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// Create renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) {
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Game loop variables
	SDL_FRect playerF = { 50.0f, 50.0f, 50.0f, 50.0f };
	const float playerSpeed = 5.0f;
	playerF.x = (windowWidth - playerF.w) / 2.0f;
	playerF.y = (windowHeight - playerF.h) / 2.0f;

	uint64_t lastTicks = SDL_GetTicksNS();
	int tileSize = 50;

	const int mapWidth = 2000;
	const int mapHeight = 2000;

	float cameraX = 0.0f;
	float cameraY = 0.0f;

	float mouseX, mouseY;

	// Game loop
	bool moving = true;
	SDL_Event event;

	const bool* key = SDL_GetKeyboardState(nullptr);

	while (moving) {
		while (SDL_PollEvent(&event)) {
			// Close window event
			if (event.type == SDL_EVENT_QUIT) {
				moving = false;
			}
		}

		// Handle keyboard input
		SDL_PumpEvents();
		key = SDL_GetKeyboardState(nullptr);

		// Update player position
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

		// Clamp player position to map bounds
		if (playerF.x < 0) playerF.x = 0;
		if (playerF.y < 0) playerF.y = 0;
		if (playerF.x > mapWidth - playerF.w) playerF.x = mapWidth - playerF.w;
		if (playerF.y > mapHeight - playerF.h) playerF.y = mapHeight - playerF.h;

		//center camera on player
		cameraX = playerF.x + playerF.w / 2 - (windowWidth / 2);
		cameraY = playerF.y + playerF.h / 2 - (windowHeight / 2);
		if (cameraX < 0) cameraX = 0;
		if (cameraY < 0) cameraY = 0;
		if (cameraX > mapWidth - windowWidth) cameraX = mapWidth - windowWidth;
		if (cameraY > mapHeight - windowHeight) cameraY = mapHeight - windowHeight;

		// Render background grid
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green grid lines
		for (int y = 0; y <= mapHeight; y += tileSize) {
			for (int x = 0; x <= mapWidth; x += tileSize) {
				SDL_FRect tileRect = { x - cameraX, y - cameraY, (float)tileSize, (float)tileSize };
				SDL_RenderRect(renderer, &tileRect);
			}
		}

		// Render player as rotated rectangle facing the mouse
		SDL_FRect adjustedPlayerF = { playerF.x - cameraX, playerF.y - cameraY, playerF.w, playerF.h };

		// Center of player
		float cx = adjustedPlayerF.x + adjustedPlayerF.w / 2;
		float cy = adjustedPlayerF.y + adjustedPlayerF.h / 2;

		// Get mouse position
		SDL_GetMouseState(&mouseX, &mouseY);
		float dx = mouseX - cx;
		float dy = mouseY - cy;

		// Compute angle to mouse
		float angle = SDL_atan2f(dy, dx);

		// Define rectangle corners relative to center
		SDL_FPoint corners[4] = {
			{ -adjustedPlayerF.w / 2, -adjustedPlayerF.h / 2 },
			{  adjustedPlayerF.w / 2, -adjustedPlayerF.h / 2 },
			{  adjustedPlayerF.w / 2,  adjustedPlayerF.h / 2 },
			{ -adjustedPlayerF.w / 2,  adjustedPlayerF.h / 2 }
		};

		// Rotate corners around center
		SDL_FPoint rotated[4];
		float cosA = cosf(angle);
		float sinA = sinf(angle);
		for (int i = 0; i < 4; ++i) {
			rotated[i].x = cx + corners[i].x * cosA - corners[i].y * sinA;
			rotated[i].y = cy + corners[i].x * sinA + corners[i].y * cosA;
		}

		// Fill vertices for SDL_RenderGeometry
		SDL_Color playerColorF = { 241, 90, 34, 255 };
		SDL_Vertex verts[4];
		for (int i = 0; i < 4; ++i) {
			verts[i].position = rotated[i];
			verts[i].color.r = 241.0f;
			verts[i].color.g = 90.0f;
			verts[i].color.b = 34.0f;
			verts[i].color.a = 255.0f;
			verts[i].tex_coord = { 0.0f, 0.0f };
		}

		// Render the rotated rectangle
		int indices[6] = { 0, 1, 2, 2, 3, 0 };
		SDL_RenderGeometry(renderer, nullptr, verts, 4, indices, 6);



		SDL_RenderPresent(renderer);

		SDL_Delay(16); // ~60 FPS
	}


	// Cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	std::cout << "Game closed\n";
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu