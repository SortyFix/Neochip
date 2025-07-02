#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <filesystem>
#include <fstream>
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

    std::string path = "lsdkfj.ch8";
    std::uintmax_t file_size = std::filesystem::file_size(path);
    NC8_Display* display = new NC8_Display();
    NC8_Core* core = new NC8_Core(path, display);
    std::ifstream romfile(path, std::ios::binary);
    uint8_t rom_buffer[file_size];

    if(romfile)
    {
        romfile.read(reinterpret_cast<char*>(rom_buffer), file_size);
    } 

    while(running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        display->updateDisplay();

        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    display->killDisplay();

    return 0;
}