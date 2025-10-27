# COMP3016-CW1

## Rescue Protocol

Rescue Protocol is a top-down sci-fi bullet hell roguelike. The goal is to survive as long as possible by rescuing survivors scattered around the map. Rescuing a survivor will increase your health, but also increase the size of your party, making you an easier target to hit. As you progress through the rounds of enemy waves, their numbers and attack styles will increase, making it more challenging to protect yourself against them. There will be some buffs you can obtain between certain rounds, and some temporary powerups across the map that change each round. All together creating a repayable, high score-based game to challenge yourself in. 


## To-do list


- [x] Make player
	- Description: Implement a controllable player entity (movement, simple sprite/rect, collision box).

- [ ] Map design (high level)
	- Description: Decide map layout, tile size, spawn points for survivors/enemies, and play area bounds.

- [ ] Make map (implementation)
	- Description: Implement tile grid / map loading (hardcoded or from a simple CSV/txt). Render tiles and a basic collision layer.

- [ ] Clamp camera / camera follow
	- Description: Implement a camera that follows the player and clamps to the map bounds so the camera doesn't show empty space.

- [ ] Player shooting
	- Description: Add ability for the player to shoot bullets/projectiles. Include a simple bullet struct and spawn logic.

- [ ] Make survivors (game objects)
	- Description: Implement survivor entities that can be rescued. Define spawn, simple AI (idle) and interaction with the player.

- [ ] Make health system
	- Description: Implement player health, damage, healing from rescued survivors, and simple UI display (health bar or number).

- [ ] Basic enemy AI
	- Description: Implement a simple enemy that moves toward the player or along a patrol route.

- [ ] Enemy attack
	- Description: Add attack behavior for enemies (melee hit, projectile, or both). Include damage application to player.

- [ ] Survivor/enemy interactions / balancing
	- Description: Tune how survivors affect difficulty (extra party members), enemy spawn rates, and damage values.

- [ ] Scoring system
	- Description: Track score for rescued survivors, defeated enemies, time survived, etc. Show score on-screen and persist high score locally.

- [ ] Alternate enemy AI (patterns)
	- Description: Implement additional enemy behaviors (orbiter, dodger, kamikaze, bullet patterns) to increase variety.

- [ ] Alternate enemy attack types
	- Description: Add varied attack mechanics (spread shots, homing bullets, area attacks) for different enemy types.

- [ ] Map design (polish)
	- Description: Create multiple maps/levels, place obstacles and cover, design interesting arenas for wave fights.

- [ ] Player design (art + polish)
	- Description: Replace placeholder rect with player sprite, add animations and feedback (hurt flash, invulnerability frames).

- [ ] Survivor design (art + behavior)
	- Description: Add sprites and small animations/behaviors for survivors (panic, follow, attach to party).

- [ ] Enemies design (art + SFX)
	- Description: Create enemy sprites, animations, and sound effects per enemy type.

- [ ] Music and audio
	- Description: Add background music and SFX for shooting, damage, rescue, and UI actions. Include volume controls.

- [ ] Additional polish / engineering tasks
	- Object pooling for bullets (performance)
	- Pause/menu screens, options (audio, controls)
	- high score and basic settings
	- Testing and bugfixes


- Milestone 1 (playable prototype): player movement, map, camera, shooting, one enemy type, one survivor type, health/score display
- Milestone 2 (first loop): basic waves, scoring, rescue mechanics, simple UI and high-score
- Milestone 3 (polish): art, music, additional enemy patterns, optimisations
