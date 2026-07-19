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
        Uint8 r = ppm->pixels[i],
              g = ppm->pixels[i+1],
              b = ppm->pixels[i+2];
        size_t pixel = i / 3;
        SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
        SDL_RenderDrawPoint(renderer, pixel % (ppm->width), pixel / (ppm->width));
    }
    
    SDL_RenderPresent(renderer);
}



format_pbm *read_pbm(FILE *file){
    format_pbm *output = malloc(sizeof(format_pbm));

    fscanf(file, "%s", output->magic);
    fscanf(file, "%d %d", &output->width, &output->height);

    if(output->magic[1] == '1'){
        output->bits = malloc((output->height * output->width) * sizeof(Uint8));
        for(int j = 0; j < output->height; j++){
            for(int i = 0; i < output->width; i++){
                fscanf(file , " %c", &output->bits[j * output->width + i]);
            }
        }
    }
    else{
        output->bits = malloc((output->height * output->width) * sizeof(Uint8));
        fgetc(file);
        for(int j = 0; j < output->height; j++){
            for(int i = 0; i < (output->width + 7)/8; i++){
                char byte = fgetc(file);
                for(int k = 0; k < 8; k++){
                    if(j * output->width + i*8 + k < output->height * output->width)
                        output->bits[j * output->width + i*8 + k] = ((byte >> (7 - k))  & 1) + '0';
                    else
                        break;
                }
            }
        }
    }
    
    return output;
}

void show_pbm(format_pbm *pbm, SDL_Renderer *renderer){
    for(int j = 0; j < pbm->height; j++){
        for(int i = 0; i < pbm->width; i++){
            Uint8 color = (pbm->bits[j * pbm->width + i] == '1') ? 0x00 : 0xFF; 
            SDL_SetRenderDrawColor(renderer, color, color, color, 0xFF);
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }

    SDL_RenderPresent(renderer);
}

format_tga *read_tga(FILE *file){
    format_tga *output = malloc(sizeof(format_tga));


    // read header (field 1 -> 5)
    output->id_length = fgetc(file);

    output->color_map_type = fgetc(file);

    output->image_type = fgetc(file);

    fread(&output->first_entry_idx, sizeof(Uint16), 1, file);
    fread(&output->color_map_length, sizeof(Uint16), 1, file);
    output->color_map_entry_size = fgetc(file);


    fread(&output->x_origin, sizeof(Uint16), 1, file);
    fread(&output->y_origin, sizeof(Uint16), 1, file);
    fread(&output->width, sizeof(Uint16), 1, file);
    fread(&output->height, sizeof(Uint16), 1, file);
    output->pixel_depth = fgetc(file);
    output->image_descriptor = fgetc(file);

    // read image map data (field 6 -> 8)
    if(output->id_length > 0){
        output->image_id = malloc(output->id_length);
        fread(output->image_id, sizeof(Uint8), output->id_length, file);
    }
    else
        output->image_id = NULL;

    if(output->color_map_type > 0){
        size_t size = (output->color_map_entry_size + 7)/8 * output->color_map_length;
        output->color_map_data = malloc(size);
        for(size_t i = 0; i < size; i++){
            output->color_map_data[i] = fgetc(file);
        }
    }
    else
        output->color_map_data = NULL;

    {
        Uint8 bytes_per_pixel = (output->pixel_depth / 8);
        size_t size = output->width * output->height * bytes_per_pixel;
        output->pixels = malloc(size);

        if(output->image_type <= 3){
            fread(output->pixels, sizeof(Uint8), size, file);
        }
        else{
            for(int i = 0; i < size; ){
                Uint8 packet = fgetc(file);
                Uint8 run_length = (packet & (1<<7));
                Uint8 count = (packet & ~(1<<7)) + 1;

                if(run_length){
                    Uint8 repeated_pixel[4];
                    fread(repeated_pixel, 1, bytes_per_pixel, file);
                    while(count--){
                        for(int j = 0; j < bytes_per_pixel; j++){
                            output->pixels[i++] = repeated_pixel[j];
                        }
                    }
                }
                else{
                    while(count--){
                        fread(&output->pixels[i], 1, bytes_per_pixel, file);
                        i += bytes_per_pixel;
                    }
                }
            }
        }
    }


    return output;
}

void show_tga(format_tga *tga, SDL_Renderer *renderer){
    if(tga->image_type == 0){
        printf("No image data provided");
        return;
    }
    else if(tga->image_type == 1 || tga->image_type == 9){
        size_t size = tga->height * tga->width;
        Uint8 bytes_per_entry = ((tga->color_map_entry_size + 7)/8);

        for(size_t i = 0; i < size; i++){
            size_t cmap_idx = (tga->pixels[i] - tga->first_entry_idx) * bytes_per_entry;
            Uint8 b = tga->color_map_data[cmap_idx],
                  g = tga->color_map_data[cmap_idx+1],
                  r = tga->color_map_data[cmap_idx+2],
                  a = (bytes_per_entry == 3) ? 0xFF : tga->color_map_data[cmap_idx + 3];
            SDL_SetRenderDrawColor(renderer, r, g, b, a);

            size_t x = i % tga->width,
                   y = !(tga->image_descriptor & (1 << 5)) ? tga->height - 1 - (i / tga->width) : i / tga->width;
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    else if(tga->image_type == 2 || tga->image_type == 10){
        Uint8 byte_depth = (tga->pixel_depth/8);
        size_t size = tga->height * tga->width * byte_depth;

        for(size_t i = 0; i < size; i+= byte_depth){
            Uint8 b = tga->pixels[i],
                  g = tga->pixels[i+1],
                  r = tga->pixels[i+2],
                  a = (byte_depth == 3) ? 0xFF : tga->pixels[i+3];
            SDL_SetRenderDrawColor(renderer, r, g, b, a);

            size_t x = (i / byte_depth) % tga->width,
                   y = !(tga->image_descriptor & (1 << 5)) ? tga->height - 1 - (i / byte_depth) / tga->width : (i / byte_depth) / tga->width;
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    else if(tga->image_type == 3 || tga->image_type == 11){
        size_t size = tga->height * tga->width;
        for(size_t i = 0; i < size; i++){
            Uint8 br = tga->pixels[i];
            SDL_SetRenderDrawColor(renderer, br, br, br, 0xFF);

            size_t x = i % tga->width,
                   y = !(tga->image_descriptor & (1 << 5)) ? tga->height - 1 - (i / tga->width) : i / tga->width;
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    else{
        printf("format is not supported yet");
        return;
    }

    SDL_RenderPresent(renderer);
}

void free_tga(format_tga *tga){
    if(tga->id_length > 0)
        free(tga->image_id);

    if(tga->color_map_type > 0)
        free(tga->color_map_data);

    free(tga->pixels);
}