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

    char file_name[256], extension[10];
    Uint8 extension_size = 0;
    printf("write the path for the image: ");
    scanf("%s", file_name);
    for(int i = 0; file_name[i] != '.'; i++){

    }

    FILE *pfile = fopen(file_name, "rb");
    if (!pfile) {perror("fopen"); return 1;}

    format_ppm *ppm = read_ppm(pfile);
    if (!ppm){ perror("failed to load ppm"); return 1;}




    
    // checking SDL components
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("Error initializing SDL");
        return 1;
    }

    // creating SDL window
    pwindow = SDL_CreateWindow("Image Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ppm->width, ppm->hieght, SDL_WINDOW_SHOWN);

    // checking for error in window creation
    if(!pwindow){
        printf("Window cannot be created");
        SDL_Quit();
        return 1;
    }

    // creating a surface on the window
    prenderer = SDL_CreateRenderer(pwindow, -1, SDL_RENDERER_ACCELERATED);

    // checking for error in surface creation
    if(!prenderer){
        printf("Renderer cannot be created");
        SDL_DestroyWindow(pwindow);
        SDL_Quit();
        return 1;
    }

    /*
    SDL_SetWindowResizable(pwindow, true);
    SDL_SetWindowMinimumSize(pwindow, 200, 200);
    SDL_RenderSetLogicalSize(prenderer, ppm->width, ppm->hieght);
    */

    show_ppm(ppm, pfile, prenderer);

    
    // running until x button is pressed
    int running = true;
    while(running){
        while(SDL_PollEvent( &event )){
            switch (event.type){
                case SDL_QUIT:
                    {running = false;
                    break;}
            }
        }
    }



    // quitting the program
    fclose(pfile);
    free(ppm);
    SDL_DestroyWindow(pwindow);
    SDL_Quit();
    return 0;
}