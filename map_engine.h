/*=============================================================================
 * map_engine.h  --  JJK World Map Engine  (public API)
 *
 * Provides:
 *   - GameMap allocation / population from raw tile data
 *   - RenderMap()  layered draw routine (Raylib variant)
 *   - Camera follow / clamp helpers
 *=============================================================================*/

#ifndef MAP_ENGINE_H
#define MAP_ENGINE_H

#include "map_defs.h"

/* ── Raylib forward declarations (include raylib.h before this header) ────── */
/*   Texture2D and DrawTextureRec are standard Raylib primitives.              */
/*   If you use SDL2 instead, swap the DrawTile_* calls in map_engine.c.       */

/* ── Map lifecycle ──────────────────────────────────────────────────────── */

/**
 * Allocate all three layer cell arrays inside `map`.
 * `map->width` and `map->height` must be set before calling this.
 * Returns true on success, false on allocation failure.
 */
bool MapAllocLayers(GameMap *map);

/**
 * Populate one layer from a flat C array of tile IDs.
 * `data` must contain exactly (map->width * map->height) int16_t entries.
 * `collisionData` may be NULL (all flags will be COL_NONE).
 */
void MapLayerLoadFromArray(GameMap *map, int layerIndex,
                           const int16_t *data,
                           const uint8_t *collisionData);

/** Free all heap memory owned by `map`. */
void MapFree(GameMap *map);

/* ── Camera helpers ─────────────────────────────────────────────────────── */

/**
 * Smoothly move `camera` towards the player's world position.
 * `smoothing` in range (0, 1]: 1.0 = instant snap, 0.1 = heavy smoothing.
 */
void CameraFollow(Camera2D *camera, float playerWorldX, float playerWorldY,
                  float smoothing);

/**
 * Clamp camera so it never shows area outside the map.
 * Call after CameraFollow every frame.
 */
void CameraClamp(Camera2D *camera, const GameMap *map);

/* ── Rendering ──────────────────────────────────────────────────────────── */

/**
 * Draw all tiles in `layerIndex` that fall within the camera viewport.
 * Uses Raylib's DrawTextureRec internally.
 * Implements viewport culling: tiles outside [visCol/Row min..max] are skipped.
 */
void RenderLayer(const GameMap *map, int layerIndex,
                 Camera2D camera, Texture2D tileset);

/**
 * Full map rendering pass.  Call this SPLIT around your player draw:
 *
 *   RenderMapBelow(map, camera, tileset);   // GROUND + FRINGE
 *   DrawPlayer(...);
 *   RenderMapAbove(map, camera, tileset);   // OVERHEAD
 *
 * Or call RenderMap() if player draw order does not matter.
 */
void RenderMapBelow(const GameMap *map, Camera2D camera, Texture2D tileset);
void RenderMapAbove(const GameMap *map, Camera2D camera, Texture2D tileset);
void RenderMap    (const GameMap *map, Camera2D camera, Texture2D tileset);

/* ── Collision query ────────────────────────────────────────────────────── */

/**
 * Return the collision flags for the ground tile at world tile (col, row).
 * Returns COL_SOLID for out-of-bounds coordinates.
 */
uint8_t MapGetCollisionFlags(const GameMap *map, int col, int row);

/**
 * Convenience: pixel-position version of MapGetCollisionFlags.
 * Converts (worldPixelX, worldPixelY) → tile coordinate automatically.
 */
uint8_t MapGetCollisionAt(const GameMap *map, float worldPixelX, float worldPixelY);

#endif /* MAP_ENGINE_H */
