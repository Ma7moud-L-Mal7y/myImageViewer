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

typedef struct 
{
    // headers
    Uint8 id_length;                // f1

    Uint8 color_map_type;           // f2

    Uint8 image_type;               // f3

    Uint16 first_entry_idx;         // f4.1
    Uint16 color_map_length;        // f4.2
    Uint8 color_map_entry_size;     // f4.3

    Uint16 x_origin;                // f5.1
    Uint16 y_origin;                // f5.2
    Uint16 width;                   // f5.3
    Uint16 height;                  // f5.4
    Uint8 pixel_depth;              // f5.5
    Uint8 image_descriptor;         // f5.6

    // image map data
    Uint8 *image_id;                // f6

    Uint8 *color_map_data;          // f7

    Uint8 *pixels;                  // f8


} format_tga;



// functions declarations
format_ppm *read_ppm(FILE *file);
void show_ppm(format_ppm *ppm, SDL_Renderer *renderer);

format_pbm *read_pbm(FILE *file);
void show_pbm(format_pbm *pbm, SDL_Renderer *renderer);

format_tga *read_tga(FILE *file);
void show_tga(format_tga *tga, SDL_Renderer *renderer);
void free_tga(format_tga *tga);

#endif