#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <unistd.h>
#include "display/display.h"
#include "emu/core.h"

int main(int argc, char* argv[])
{
    int width = 1024;
    int height = 512;

    bool running = true;

    SDL_Window* window = NULL;
    char* file_path = argv[1];

    if(SDL_Init(SDL_INIT_VIDEO) == false)
    {
        std::cout << "NEOCHIP: Could not initialize SDL3 core. Quitting. \n" << SDL_GetError();
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if(file_path == NULL)
    {
        std::cout << "NEOCHIP: You need to provide the file path to a CHIP-8 ROM to run a game." << std::endl;
        return 1;
    }

    std::string path = argv[1];
    std::uintmax_t file_size = std::filesystem::file_size(path);
    NC8_Display* display = new NC8_Display();
    std::cout << "NEOCHIP-OK: Initialized display.\n";
    NC8_Core* core = new NC8_Core(path, display, false, false);
    std::cout << "NEOCHIP-OK: Initialized emulator core.\n";

while (running)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
        if (event.type == SDL_EVENT_KEY_DOWN) {
            SDL_KeyboardEvent& keyEvent = event.key;
            if (keyEvent.key == SDLK_ESCAPE) {
                running = false;
            }
            if (keyEvent.key == SDLK_M) {
                core->reset();
            }
        }
    }

    if (core->draw_now) {
        display->updateDisplay();
        core->draw_now = false;
    }

    core->tick();

    usleep(1000);
}


    display->killDisplay();

    return 0;
}