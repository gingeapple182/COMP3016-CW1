#include "Enemy.h"
#include "BulletPool.h"
#include <cmath>

Enemy::Enemy()
// Default inactive enemy
	: x(0), y(0), speed(0), active(false), damage(1), type(EnemyType::Runner)
{
	rect = { 0, 0, 32, 32 }; // Enemy size 32x32
	colour = { 0, 0, 255, 255 }; // blue
}

// Initialize enemy with position, speed, damage, and type
void Enemy::init(float startX, float startY, float enemySpeed, int enemyDamage, EnemyType enemyType)
{	
	x = startX;
	y = startY;
	speed = enemySpeed;
	damage = enemyDamage;
	active = true;
	type = enemyType;

	applyTypeAttributes();

	if (enemyType == EnemyType::Shooter) {
		engageDistance = 350.0f + (rand() % 100); // Random between 350 and 450
		baseSpeed = speed;
	}

	rect.x = x - rect.w / 2;
	rect.y = y - rect.h / 2;
}

// Update enemy position and behavior based on type
void Enemy::update(float deltaTime, const SDL_FPoint& playerPosition, BulletPool& enemyBullets)
{
	if (!active) return;

	if (shooterCooldown > 0.0f) {
		shooterCooldown -= deltaTime;
	}

	float dx = playerPosition.x - x;
	float dy = playerPosition.y - y;
	float distance = std::sqrt(dx * dx + dy * dy);
	if (distance != 0.0f) {
		dx /= distance;
		dy /= distance;
	}

	switch (type)
	{
	case EnemyType::Runner:
		// Simple direct chase
		x += dx * speed * deltaTime;
		y += dy * speed * deltaTime;
		colour = { 0, 255, 0, 255 }; // green
		break;

	case EnemyType::Shooter:
	{
		colour = { 0, 128, 255, 255 }; // Bright blue

		if (distance > engageDistance) {
			// Full-speed approach
			isShooterShooting = false;
			x += dx * baseSpeed * deltaTime;
			y += dy * baseSpeed * deltaTime;
		}
		else if (distance > engageDistance * 0.75f) {
			// Slow approach
			isShooterShooting = true;
			float slowedSpeed = baseSpeed * 0.66f;
			x += dx * slowedSpeed * deltaTime;
			y += dy * slowedSpeed * deltaTime;
			colour = { 0, 64, 200, 255 }; // Medium blue
			// Shooting
			if (shooterCooldown <= 0.0f) {
				Bullet* bullet = enemyBullets.getBullet();
				if (bullet) {
					SDL_Color bulletColour = { 0, 128, 255, 255 };
					bullet->init(x, y, dx, dy, ShooterShootySpeed, bulletColour);
				}
				shooterCooldown = shooterCooldownReset;
			}
		}
		else {
			// Within range — stop and prepare to shoot
			isShooterShooting = true;
			colour = { 0, 0, 128, 255 }; // Dark blue when stationary
			// No movement
			// Shooting
			if (shooterCooldown <= 0.0f) {
				Bullet* bullet = enemyBullets.getBullet();
				if (bullet) {
					SDL_Color bulletColour = { 0, 128, 255, 255 };
					bullet->init(x, y, dx, dy, ShooterShootySpeed, bulletColour);
				}
				shooterCooldown = shooterCooldownReset;
			}
		}
		break;
	}

	case EnemyType::Sniper:
		// Placeholder
		break;
	}

	rect.x = x - rect.w / 2;
	rect.y = y - rect.h / 2;
}

// Render enemy on screen
void Enemy::render(SDL_Renderer* renderer, float cameraX, float cameraY)
{
	// Dont render if inactive
	if (!active) return;

	// Move enemy onto the screen
	SDL_FRect screenRect = {
		rect.x - cameraX,
		rect.y - cameraY,
		rect.w,
		rect.h
	};

	// Draw enemy
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(renderer, &screenRect);
}

// Deactivate enemy
void Enemy::deactivate()
{
	active = false;
}

// Apply default attributes based on enemy type
void Enemy::applyTypeAttributes()
{
	switch (type) {
	case EnemyType::Runner:
		if (speed <= 0) speed = 10.0f;
		if (damage <= 0) damage = 2;
		colour = { 0, 255, 0, 255 }; // green
		break;
	case EnemyType::Shooter:
		if (speed <= 0) speed = 10.0f;
		if (damage <= 0) damage = 1;
		colour = { 255, 0, 0, 255 }; // red
		break;
	case EnemyType::Sniper:
		if (speed <= 0) speed = 10.0f;
		if (damage <= 0) damage = 2;
		colour = { 0, 0, 255, 255 }; // blue
		break;
	}
}