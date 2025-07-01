#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <iostream>
#include "display/display.h"

int main()
{
    int width = 1024;
    int height = 512;

    bool running = true;

    SDL_Window* window = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) == false)
    {
        std::cout << "NEOCHIP: Could not initialize SDL3 core. Quitting. \n" << SDL_GetError();
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    NC8_Display* display = new NC8_Display();

    while(running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        display->updateDisplay();
    }

    display->killDisplay();

    return 0;
}