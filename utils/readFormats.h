#ifndef READFORMATS_H
#define READFORMATS_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

// format structs
typedef struct
{
    char magic[3];
    int width, hieght;
    int max_val;
    size_t size;

} format_ppm;

// functions declarations
format_ppm *read_ppm(FILE *file);
void show_ppm(format_ppm *ppm, FILE *file, SDL_Renderer *renderer);

#endif