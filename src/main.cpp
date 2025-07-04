#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <filesystem>
#include <iostream>
#include <thread>
#include "display/display.h"
#include "emu/core.h"

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

    std::string path = "/home/sortyfix/Projects/C++/Neochip/src/roms/3-corax+.ch8";
    std::uintmax_t file_size = std::filesystem::file_size(path);
    NC8_Display* display = new NC8_Display();
    std::cout << "NEOCHIP-OK: Initialized display.\n";
    NC8_Core* core = new NC8_Core(path, display);
    std::cout << "NEOCHIP-OK: Initialized emulator core.\n";

    while(running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        display->updateDisplay();
        core->tick();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    display->killDisplay();

    return 0;
}