#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
    int *data;
    int width;
    int height;
    char name[256];
} TileLayer;

typedef struct {
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    TileLayer *layers;
    int layerCount;
} TileMap;

// Simple JSON number extraction
static int extract_int(const char *str, const char *key) {
    char search[512];
    snprintf(search, sizeof(search), "\"%s\":%d", key, 0);
    
    char pattern[256];
    snprintf(pattern, sizeof(pattern), "\"%s\":", key);
    
    const char *pos = strstr(str, pattern);
    if (pos) {
        pos += strlen(pattern);
        while (*pos && isspace(*pos)) pos++;
        return atoi(pos);
    }
    return 0;
}

// Extract string value from JSON
static void extract_string(const char *str, const char *key, char *out, int outLen) {
    char pattern[256];
    snprintf(pattern, sizeof(pattern), "\"%s\":\"", key);
    
    const char *pos = strstr(str, pattern);
    if (pos) {
        pos += strlen(pattern);
        int i = 0;
        while (*pos && *pos != '"' && i < outLen - 1) {
            out[i++] = *pos++;
        }
        out[i] = '\0';
    } else {
        out[0] = '\0';
    }
}

// Extract integer array from JSON "data":[...]
static int *extract_array(const char *str, int *count) {
    const char *start = strstr(str, "\"data\":[");
    if (!start) return NULL;
    
    start += strlen("\"data\":[");
    
    // Count elements first
    int elemCount = 0;
    const char *tmp = start;
    while (*tmp && *tmp != ']') {
        if (isdigit(*tmp)) {
            elemCount++;
            while (*tmp && isdigit(*tmp)) tmp++;
        } else {
            tmp++;
        }
    }
    
    *count = elemCount;
    int *array = (int *)malloc(elemCount * sizeof(int));
    
    // Parse elements
    int idx = 0;
    tmp = start;
    while (*tmp && *tmp != ']' && idx < elemCount) {
        if (isdigit(*tmp)) {
            array[idx++] = atoi(tmp);
            while (*tmp && isdigit(*tmp)) tmp++;
        } else {
            tmp++;
        }
    }
    
    return array;
}

// Find the opening '{' of the next tile layer object, searching from 'afterPos' onward
static const char *find_layer_start(const char *buffer, const char *afterPos) {
    const char *typePos = strstr(afterPos, "\"type\":\"tilelayer\"");
    if (!typePos) return NULL;
    // Walk backward from the type marker to the nearest unmatched '{' that opens this object
    const char *p = typePos;
    while (p > buffer && *p != '{') p--;
    return (*p == '{') ? p : NULL;
}

// Find the matching closing brace for an object that starts at 'start' (which must point to '{')
static const char *find_matching_brace(const char *start) {
    int depth = 0;
    bool inString = false;
    for (const char *p = start; *p; p++) {
        if (*p == '"' && (p == start || *(p - 1) != '\\')) {
            inString = !inString;
        } else if (!inString) {
            if (*p == '{') depth++;
            else if (*p == '}') {
                depth--;
                if (depth == 0) return p;
            }
        }
    }
    return NULL;
}

// Load TMJ file
static TileMap* LoadTileMap(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open %s\n", filename);
        return NULL;
    }
    
    // Read entire file
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *buffer = (char *)malloc(fileSize + 1);
    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';
    fclose(file);
    
    TileMap *map = (TileMap *)malloc(sizeof(TileMap));
    
    // Extract map properties
    map->width = extract_int(buffer, "width");
    map->height = extract_int(buffer, "height");
    map->tileWidth = extract_int(buffer, "tilewidth");
    map->tileHeight = extract_int(buffer, "tileheight");
    
    // Count layers
    map->layerCount = 0;
    const char *layerStart = buffer;
    while ((layerStart = strstr(layerStart, "\"type\":\"tilelayer\"")) != NULL) {
        map->layerCount++;
        layerStart++;
    }
    
    map->layers = (TileLayer *)malloc(map->layerCount * sizeof(TileLayer));
    
    // Parse each layer by locating its "type":"tilelayer" marker and using
    // real brace-depth matching to find where the layer object actually ends.
    const char *searchPos = buffer;
    for (int i = 0; i < map->layerCount; i++) {
        const char *layerStart = find_layer_start(buffer, searchPos);
        if (!layerStart) break;
        
        const char *layerEndBrace = find_matching_brace(layerStart);
        if (!layerEndBrace) break;
        
        int layerLen = (int)(layerEndBrace - layerStart) + 1;
        char *layerStr = (char *)malloc(layerLen + 1);
        strncpy(layerStr, layerStart, layerLen);
        layerStr[layerLen] = '\0';
        
        // Extract layer properties
        extract_string(layerStr, "name", map->layers[i].name, sizeof(map->layers[i].name));
        map->layers[i].width = extract_int(layerStr, "width");
        map->layers[i].height = extract_int(layerStr, "height");
        
        int count = 0;
        map->layers[i].data = extract_array(layerStr, &count);
        if (!map->layers[i].data) {
            // Fall back to zero-size layer instead of leaving a dangling NULL
            // that later code would index into.
            map->layers[i].width = 0;
            map->layers[i].height = 0;
        }
        
        free(layerStr);
        searchPos = layerEndBrace + 1;
    }
    
    free(buffer);
    return map;
}

static void UnloadTileMap(TileMap *map) {
    if (!map) return;
    for (int i = 0; i < map->layerCount; i++) {
        if (map->layers[i].data) free(map->layers[i].data);
    }
    free(map->layers);
    free(map);
}

#endif