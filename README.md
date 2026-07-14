# JJK World – Pokémon-style Map Engine

A GBA/NDS-era Pokémon-inspired 2D tile map engine written in pure C using [Raylib](https://www.raylib.com/).

---

## File Overview

| File | Purpose |
|---|---|
| `map_defs.h` | All structs, constants, tile IDs, inline helpers |
| `map_engine.h` | Public API declarations |
| `map_engine.c` | Full implementation (culling, camera, renderer) |
| `sample_map.h` | Hardcoded 20×15 Tokyo demo map (3 layers) |
| `main.c` | Game loop, player movement, collision, HUD |
| `tileset_final_test_output.png` | 512×512 tileset (16×16 grid, magenta mask) |

---

## Compilation

### Windows (MinGW + Raylib)
```bat
gcc main.c map_engine.c -o jjk_game.exe ^
    -I. -Iraylib/include ^
    -Lraylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lm
```

### Linux / macOS
```bash
clang main.c map_engine.c -o jjk_game \
    -lraylib -lGL -lm -lpthread -ldl
```

> Make sure `tileset_final_test_output.png` is in the **same directory** as the executable.

---

## Controls

| Key | Action |
|---|---|
| Arrow Keys / WASD | Move player |
| `+` / `=` | Zoom in |
| `-` | Zoom out |
| `ESC` | Quit |

---

## Architecture

```
RenderMapBelow()   ← Ground + Fringe layers
DrawPlayer()       ← Player sprite (between layers)
RenderMapAbove()   ← Overhead layer (rooftops, treetops)
```

### Three-Layer System

| Layer | Index | Contents |
|---|---|---|
| GROUND | 0 | Grass, road, sidewalk, water |
| FRINGE | 1 | Building bases, tree trunks, vehicles, vending machines |
| OVERHEAD | 2 | Rooftops, treetops, Tokyo Tower tips |

### Tile ID Formula

```c
int col = tileID % SHEET_COLS;   // SHEET_COLS = 32
int row = tileID / SHEET_COLS;
int srcX = col * TILE_SIZE;      // TILE_SIZE = 16
int srcY = row * TILE_SIZE;
```

### Viewport Culling

Only tiles within the camera frustum (+ 1-tile margin) are processed each frame — identical to GBA-era hardware optimizations.

---

## Extending the Map

1. Edit the `g_groundData`, `g_fringeData`, `g_overheadData` arrays in `sample_map.h`.
2. Add new named tile constants to `map_defs.h` by identifying the tile's column and row in the PNG:  
   `#define MY_TILE  (row * 32 + col)`
3. For multi-tile structures, place each 16×16 piece at the correct layer and offset position.

---

## Magenta Transparency

The tileset uses `#FF00FF` as a colour-key mask. The engine converts this to true alpha at load time via Raylib's `ImageColorReplace()`. No extra steps needed in your asset pipeline.
