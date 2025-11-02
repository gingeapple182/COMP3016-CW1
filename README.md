## COMP3016 CW1

# Survivor Protocol

Survivor Protocol is a 2D, top-down survival / bullet-hell simulation built in C++ with SDL3 enhancements for visuals.
You as the player control a rescue unit in a test survival environment. Each simulation round spawns hostile enemies and stranded survivors. Rescuing survivors will restore the players health, but also increases the size of the player entity, making it harder to dodge incoming attacks. The goal is to survive as long as possible in increasingly difficult simulation rounds.

This game demonstrates the following:
- Object-oriented codebase structure
- Runtime config loading from a .txt file
- Object Pooling
- Game state machine (START -> INSTRUCTIONS -> PLAY -> GAMEOVER)
- A simple narrative objective and theme

# Dependencies used
This project was built in **C++** using **SDL3** for rendering, input and timing.

| Dependency  | Purpose                                         | Notes                                           |
|-------------|--------------------------------------------------|-------------------------------------------------|
| **SDL3**    | Window creation, renderer, input, timing         | Core library, required at runtime               |
| **SDL_ttf** | Text rendering for UI and in-game overlays       | Used for start screen, HUD, round summary, etc. |
| **C++ STL** | `vector`, `string`, file I/O, `cmath`, `<chrono>` | No C-style arrays or `char*` used where avoidable |

> **Runtime note:** the final `.zip` must include the SDL3/SDL_ttf DLLs alongside the executable so it can run without Visual Studio.


# Use of AI description
This assignment is under the **“Partnered Work”** category, so AI use is allowed as long as it is declared.

AI was used **as an assistive tool** for:

- helping structure the game into classes (`Game`, `Player`, `EnemyPool`, `SurvivorPool`);
- helping to identify and resolve errors;
- suggesting improvements to code readability and comments;
- teaching code concepts and providing examples - final code written by myself
- drafting this README

Final submitted code has been written by myself, with help from Visual Studio 2022 IDE AI assistant autofill, all autofilled code was reviewed and controlled by myself.


# Game programming patterns used
### 1. Object Pooling
- Used for **enemies**, **survivors**, and **bullets**.
- On spawn, the code reuses an inactive object instead of allocating a new one.
- This keeps runtime stable and avoids having to constantly create and destroy items.

### 2. Simple Game State Machine
States are handled in `Game::update()` and `Game::render()`:

- `START`
- `INSTRUCTIONS`
- `PLAY`
- `GAMEOVER`

### 3. Data-Driven Configuration
- On startup the game calls `Game::loadConfig()` and reads from `config.txt`.
- Sections:
  - `[PLAYER]` -> start health, size, speed
  - `[ENEMY]` -> enemy speed/damage
  -  `[WORLD]` -> survivor spawn, hp gain, growth perceptage - Map size, background grid and enemy spawn increases

### 4. Encapsulation of Entities
- `Player`, `Enemy`, `Survivor` each own their rect, update, and render.
- The `Game` class orchestrates them (spawning, collisions, round progression).
- This demonstrates OOP.


# Game mechanics and how they were coded
### Player
- **Movement:** `Player::update()` reads keyboard (`SDL_GetKeyboardState`) and moves the rect.
- **Bounds:** player is clamped to the map size by moving them back into the map bounds when movement would place them out of bounds.
- **Aiming:** mouse position → `atan2` → `angleRad`.
- **Shooting:** SPACE spawns a bullet from the player’s bullet pool with a short cooldown.

### Enemies
- **Runner:** chases the player directly, deals contact damage.
- **Shooter:** moves towards player until in range, then stops and fires bullets using the enemy bullet pool.
- Enemy stats (speed, damage) now come from the **config** when spawned:
  - runner → `enemyRunnerSpeed`, `enemyRunnerDamage`
  - shooter → `enemyShooterSpeed`, `enemyShooterDamage`

### Survivors
- Spawned each round.
- Collision with player:
  - heals player;
  - increases player size slightly;
  - survivor is deactivated in the pool.
- This is the “risk/reward” part of the gameplay.

### Rounds / Simulation Loop
- At the start of PLAY: enemies + survivors are spawned.
- When all enemies are dead: `roundInProgress = false` → round summary overlay is shown.
- After a short timer: next round is started and more enemies are spawned and survivors refresh.



# UML

# Sample screenshots

# Exception handling

# Further details

- Rendering is deliberately simple (rects and colours) to make logic visible.
- Colours are consistent:
  - player → white
  - survivors → purple
  - runner → green
  - shooter → blue
- Grid is rendered behind gameplay for spatial reference.
- Config file ends with a `-- Notes for reader --` stop marker so comments don’t break parsing.


# Evaluaion
