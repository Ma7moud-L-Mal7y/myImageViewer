#include "readFormats.h"

format_ppm *read_ppm(FILE *file){
    format_ppm *output = malloc(sizeof(format_ppm));

    fscanf(file, "%s", output->magic);
    fscanf(file, "%d %d", &output->width, &output->height);
    fscanf(file, "%d", &output->max_val);

    output->size = output->height * output->width * 3;
    output->pixels = malloc((output->size) * sizeof(Uint8));

    for(size_t i = 0; i < output->size; i++){
        output->pixels[i] = fgetc(file) * 255 / output->max_val;
    }


    return output;
}

void show_ppm(format_ppm *ppm, SDL_Renderer *renderer){
    if(ppm->magic[1] != '6'){
        printf("only P6 format is supported for now");
        return;
    }

    for(size_t i = 0; i < ppm->size; i += 3){
        Uint8 r = ppm->pixels[i], g = ppm->pixels[i+1], b = ppm->pixels[i+2];
        size_t pixel = i / 3;
        SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
        SDL_RenderDrawPoint(renderer, pixel % (ppm->width), pixel / (ppm->width));
    }
    
    SDL_RenderPresent(renderer);
}

void free_ppm(format_ppm *ppm){
    free(ppm->pixels);
    free(ppm);
}


format_pbm *read_pbm(FILE *file){
    format_pbm *output = malloc(sizeof(format_pbm));

    fscanf(file, "%s", output->magic);
    fscanf(file, "%d %d", &output->width, &output->height);
    output->bits = malloc((output->height * output->width) * sizeof(Uint8));

    if(output->magic[1] == '1'){
        for(int j = 0; j < output->height; j++){
            for(int i = 0; i < output->width; i++){
                fscanf(file , " %c", &output->bits[j * output->width + i]);
            }
        }
    }
    else;
    
    return output;
}

void show_pbm(format_pbm *pbm, SDL_Renderer *renderer){
    if(pbm->magic[1] == '1'){
        for(int j = 0; j < pbm->height; j++){
            for(int i = 0; i < pbm->width; i++){
                Uint8 color = (pbm->bits[j * pbm->width + i] == '1') ? 0x00 : 0xFF; 
                SDL_SetRenderDrawColor(renderer, color, color, color, 255);
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }
    }
    else;

    SDL_RenderPresent(renderer);
}

void free_pbm(format_pbm *pbm){
    free(pbm->bits);
    free(pbm);
}