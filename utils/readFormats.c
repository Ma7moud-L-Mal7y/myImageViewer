#include "readFormats.h"

format_ppm *read_ppm(FILE *file){
    format_ppm *output = malloc(sizeof(format_ppm));

    fscanf(file, "%s", output->magic);
    fscanf(file, "%d %d", &output->width, &output->hieght);
    fscanf(file, "%d", &output->max_val);

    output->size = output->hieght * output->width * 3;

    return output;
}

void show_ppm(format_ppm *ppm, FILE *file, SDL_Renderer *renderer){
    if(ppm->magic[1] != '6'){
        printf("only P6 format is supported for now");
        return;
    }

    Uint8 r, g, b;
    int  max_color_val = ppm->max_val;
    for(int j = 0; j < ppm->hieght; j++){
        for(int i = 0; i < ppm->width; i++){
            r = fgetc(file) * 255 / max_color_val;
            g = fgetc(file) * 255 / max_color_val;
            b = fgetc(file) * 255 / max_color_val;
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }
    SDL_RenderPresent(renderer);
}