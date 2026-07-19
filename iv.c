#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "utils/readFormats.h"

int main(int argc, char** args){

    //  intializing components
    SDL_Window *pwindow = NULL;
    SDL_Renderer *prenderer = NULL;
    SDL_Event event;

    char file_name[256], extension[10], extension_size = 0;
    printf("write the path for the image: ");
    scanf("%255s", file_name);

    // get file extension
    bool extension_start = false;
    for(int i = 0; file_name[i] != '\0'; i++){
        if(extension_start)
            extension[extension_size++] = file_name[i];

        if(file_name[i] == '.')
            extension_start = true;
    }
    extension[extension_size] = '\0';

    FILE *pfile = fopen(file_name, "rb");
    if (!pfile) {perror("fopen"); return 1;}

    // pointers to supported extensions
    format_ppm *ppm;
    format_pbm *pbm;
    format_tga *tga;

    // checking SDL components
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("Error initializing SDL");
        return 1;
    }

    int height, width;
    if(!strcmp("ppm", extension)){
        ppm = read_ppm(pfile);
        if (!ppm){ perror("failed to load ppm"); return 1;}

        height = ppm->height;
        width = ppm->width;
    }
    else if(!strcmp("pbm", extension)){
        pbm = read_pbm(pfile);
        if (!pbm){ perror("failed to load pbm"); return 1;}

        height = pbm->height;
        width = pbm->width;        
    }
    else if(!strcmp("tga", extension)){
        tga = read_tga(pfile);
        if (!tga){ perror("failed to load tga"); return 1;}

        height = tga->height;
        width = tga->width;
    }
    else{
        printf("extension not supported yet!\n");
        return 1;
    }

    // creating SDL window with the height and width of the image
    pwindow = SDL_CreateWindow("Image Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

    // checking for error in window creation
    if(!pwindow){
        printf("Window cannot be created");
        SDL_Quit();
        return 1;
    }
    // set window to be resizable
    SDL_SetWindowResizable(pwindow, true);
    SDL_SetWindowMinimumSize(pwindow, 200, 200);

    // creating a renderer on the window
    prenderer = SDL_CreateRenderer(pwindow, -1, SDL_RENDERER_ACCELERATED);

    // checking for error in renderer creation
    if(!prenderer){
        printf("Renderer cannot be created");
        SDL_DestroyWindow(pwindow);
        SDL_Quit();
        return 1;
    }
    SDL_RenderSetLogicalSize(prenderer, width, height);
    
    // show the image
    if(!strcmp("ppm", extension)){
        show_ppm(ppm, prenderer);
    }
    else if(!strcmp("pbm", extension)){
        show_pbm(pbm, prenderer);
    }
    else if(!strcmp("tga", extension)){
        show_tga(tga, prenderer);
    }

    bool running = true;
    while(running){
        while(SDL_PollEvent( &event )){
            switch (event.type)
            {
            case SDL_QUIT:
                if(!strcmp("ppm", extension))
                    free(ppm->pixels);
                else if(!strcmp("pbm", extension))
                    free(pbm->bits);
                else if(!strcmp("tga", extension))
                    free_tga(tga);
                
                running = false;
                break;
            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_EXPOSED)
                    if(!strcmp("ppm", extension))
                        show_ppm(ppm, prenderer);
                    else if(!strcmp("pbm", extension))
                        show_pbm(pbm, prenderer);
                    else if(!strcmp("tga", extension))
                        show_tga(tga, prenderer);
            
            default:
                break;
            }
        }
    }


    // free formats
    free(ppm);
    free(pbm);
    free(tga);

    // quitting the program
    fclose(pfile);
    SDL_DestroyWindow(pwindow);
    SDL_Quit();
    return 0;
}