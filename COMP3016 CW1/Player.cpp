#include "Player.h"

Player::Player(float x, float y, float w, float h, float speedPerFrame)
	: speed(speedPerFrame)
{ 
	// Initialize player rectangle
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}

void Player::update(int mapWidth, int mapHeight)
{
	// Handle keyboard input
	const bool* key = SDL_GetKeyboardState(nullptr);

	if (key[SDL_SCANCODE_W] || key[SDL_SCANCODE_UP]) {
		rect.y -= speed;
	}
	if (key[SDL_SCANCODE_S] || key[SDL_SCANCODE_DOWN]) {
		rect.y += speed;
	}
	if (key[SDL_SCANCODE_A] || key[SDL_SCANCODE_LEFT]) {
		rect.x -= speed;
	}
	if (key[SDL_SCANCODE_D] || key[SDL_SCANCODE_RIGHT]) {
		rect.x += speed;
	}

	// Constrain player to map bounds
	if (rect.x < 0) rect.x = 0;
	if (rect.y < 0) rect.y = 0;
	if (rect.x > mapWidth - rect.w) rect.x = mapWidth - rect.w;
	if (rect.y > mapHeight - rect.h) rect.y = mapHeight - rect.h;
}

void Player::render(SDL_Renderer* renderer, float cameraX, float cameraY)
{
	// Adjust position based on camera
	SDL_FRect adjust = { rect.x - cameraX, rect.y - cameraY, rect.w, rect.h };

	float centerX = adjust.x + adjust.w / 2.0f;
	float centerY = adjust.y + adjust.h / 2.0f;

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	// Calculate angle to mouse position
	float dx = mouseX - centerX;
	float dy = mouseY - centerY;
	float angle = SDL_atan2f(dy, dx);

	// Define the four corners of the rectangle
	SDL_FPoint corners[4] = {
		{ -adjust.w / 2, -adjust.h / 2 },
		{  adjust.w / 2, -adjust.h / 2 },
		{  adjust.w / 2,  adjust.h / 2 },
		{ -adjust.w / 2,  adjust.h / 2}
	};

	// Rotate corners around the center
	SDL_FPoint rotated[4];
	float cosA = cosf(angle);
	float sinA = sinf(angle);
	for (int i = 0; i < 4; ++i) {
		rotated[i].x = corners[i].x * cosA - corners[i].y * sinA + centerX;
		rotated[i].y = corners[i].x * sinA + corners[i].y * cosA + centerY;
	}

	// Create vertices for rendering
	SDL_Vertex verts[4];
	for (int i = 0; i < 4; ++i) {
		verts[i].position = { rotated[i].x, rotated[i].y };
		verts[i].color.r = 241.0f;
		verts[i].color.g = 90.0f;
		verts[i].color.b = 34.0f;
		verts[i].color.a = 255.0f;
		verts[i].tex_coord = { 0, 0 };
	}

	// Define indices for two triangles
	int indices[6] = { 0, 1, 2, 2, 3, 0 };
	SDL_RenderGeometry(renderer, nullptr, verts, 4, indices, 6);
}

SDL_FPoint Player::centreWorld() const
{
	// Return the center point of the player in world coordinates
	return { rect.x + rect.w / 2.0f, rect.y + rect.h / 2.0f };
}