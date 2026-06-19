#ifndef READFORMATS_H
#define READFORMATS_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

// format structs
typedef struct
{
    char magic[3];
    int width, height;
    int max_val;
    size_t size;
    Uint8* pixels;

} format_ppm;

typedef struct
{
    char magic[3];
    int width, height;
    Uint8 *bits;

} format_pbm;

// functions declarations
format_ppm *read_ppm(FILE *file);
void show_ppm(format_ppm *ppm, SDL_Renderer *renderer);
void free_ppm(format_ppm *ppm);

format_pbm *read_pbm(FILE *file);
void show_pbm(format_pbm *pbm, SDL_Renderer *renderer);
void free_pbm(format_pbm *pbm);

#endif