# Half Glass Karew — JJK RPG

A Jujutsu Kaisen–themed 2D RPG built in C with [raylib](https://www.raylib.com/) and [raygui](https://github.com/raysan5/raygui). Pick a character, explore an overworld map, dive into a cave, and fight random enemies (and a boss) in a Pokémon-style turn-based battle system.

## Features

- Animated start menu with a video background
- Character select carousel (Gojo, Sukuna, Megumi, Nobara)
- Tile-based overworld and cave maps loaded from Tiled `.tmj` files, with collision, teleport, encounter, exit, and heal layers
- Random encounter system with a separate boss-battle encounter flow in the cave
- Turn-based battle engine: move selection menu, HP bar lerp animation, hit/flash feedback, sprite-sheet animation, win/lose flow
- A parallel boss-battle engine (`bossbattle.c/.h`) with its own `Boss`-prefixed types so it can coexist with the regular battle engine in the same build
- Persistent player stats: HP, level, and EXP with level-up scaling
- Custom lightweight JSON parser for reading Tiled map exports (no external JSON library dependency)

## Project structure

```
MAINGAME.c              - entry point; unity-build root, game loop, mode switching
headerfiles/
  character.c            - player movement, animation frame handling
  charselect.c            - character selection screen
  tiled.c / tiled.h       - tile map drawing, viewport scaling, debug UI
  mapcollision.c / .h     - collision checks, encounter triggers, map switching
  menu.c                  - main menu + video background
  battle.c / battle.h     - regular random-encounter battle engine
  bossbattle.c / .h       - boss battle engine (Boss-prefixed types/functions)
  playerstats.c / .h      - level, EXP, HP tracking and HUD
  gamemode.h              - GameMode enum shared across modes
json_parser.h             - minimal .tmj (Tiled JSON) parser
Assets&resources/         - sprites, backgrounds, videos, icons
assets/                   - Tiled map files (.tmj) and tilesets
```

## Build pattern

This project uses a **unity build**: `MAINGAME.c` `#include`s every other `.c` file directly, so the whole game compiles as a single translation unit. Because of this:

- Struct, enum, and macro names must be unique across every included file — two files can't both define a `Character` type, for example.
- Prefer `static` for helper functions that shouldn't be visible outside their own file.
- The boss battle engine intentionally uses a `Boss`/`BOSS` naming convention (`BossCharacter`, `BOSS_BATTLE_INTRO`, `BOSS_MAX_NAME_LEN`, etc.) so it doesn't collide with the identically-shaped regular battle engine.

## Requirements

- A C compiler with C99 support (developed with MinGW on Windows)
- [raylib](https://www.raylib.com/) (and raygui headers if UI widgets are used)
- ffmpeg, if you want to regenerate the menu background video

## Building

```
gcc MAINGAME.c -o game.exe -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm
```

Adjust include/lib paths to wherever your raylib install lives.

## Running

```
./game.exe
```

Controls:
- Arrow keys / WASD — move
- Enter — confirm / advance menus
- Arrow keys + Enter, or number keys 1–4 — select a move in battle
- Space — trigger a random encounter when one appears
- Backspace — fallback exit from the cave (if no exit tile layer is configured on the map)

## Status / known open items

- Battle engine currently supports a fixed pool of random enemies per encounter; multiple simultaneous enemies is a possible future addition
- `UpdateEncounterBoss` doesn't yet support dismissing a cave encounter with Enter the way the overworld version does
- `game.exitLayerIndex` needs to be assigned from `mapcollision.c`'s `SwitchMap`, not left commented out in `MAINGAME.c`'s manual setup

## License

Personal / educational project — no license specified yet.