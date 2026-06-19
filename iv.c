#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "utils/readFormats.h"

/*  
TO RUN THE PROGRAM:

make
make run

*/

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

    // pointers for supported extensions
    format_ppm *ppm;
    format_pbm *pbm;

    // checking SDL components
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("Error initializing SDL");
        return 1;
    }

    if(!strcmp("ppm", extension)){
        ppm = read_ppm(pfile);
        if (!ppm){ perror("failed to load ppm"); return 1;}

        // creating SDL window for ppm
        pwindow = SDL_CreateWindow("Image Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ppm->width, ppm->height, SDL_WINDOW_SHOWN);

    }
    else if(!strcmp("pbm", extension)){
        pbm = read_pbm(pfile);
        if (!pbm){ perror("failed to load pbm"); return 1;}

        // creating SDL window for pbm
        pwindow = SDL_CreateWindow("Image Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pbm->width, pbm->height, SDL_WINDOW_SHOWN);

    }
    else{
        printf("extension not supported yet!\n");
        return 1;
    }


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
        

    
    if(!strcmp("ppm", extension)){
        SDL_RenderSetLogicalSize(prenderer, ppm->width, ppm->height);
        show_ppm(ppm, prenderer);
    }
    else if(!strcmp("pbm", extension)){
        SDL_RenderSetLogicalSize(prenderer, pbm->width, pbm->height);
        show_pbm(pbm, prenderer);
    }
    bool running = true;
    while(running){
        while(SDL_PollEvent( &event )){
            switch (event.type)
            {
            case SDL_QUIT:
                if(!strcmp("ppm", extension))
                    free_ppm(ppm);
                else if(!strcmp("pbm", extension))
                    free_pbm(pbm);
                running = false;
                break;
            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_EXPOSED)
                    if(!strcmp("ppm", extension))
                        show_ppm(ppm, prenderer);
                    else if(!strcmp("pbm", extension))
                        show_pbm(pbm, prenderer);
            
            default:
                break;
            }
        }
    }



    // quitting the program
    fclose(pfile);
    SDL_DestroyWindow(pwindow);
    SDL_Quit();
    return 0;
}