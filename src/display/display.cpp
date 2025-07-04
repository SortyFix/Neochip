#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <cstdint>
#include "display.h"
#include <algorithm>
#include <iostream>

NC8_Display::NC8_Display(int width, int height)    
{
    window = SDL_CreateWindow("Neochip CHIP-8 Emulator 1.0", width, height, 0);

    if(window == NULL)
    {
        std::cout << "NEOCHIP: Could not initialize SDL3 window. Quitting. \n" << SDL_GetError();
        SDL_Quit();
        exit(1);
    }
    
    renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    std::fill_n(framebuffer, 64 * 32, 0xFF000000);
}

NC8_Display::~NC8_Display()
{
    killDisplay();
}

void NC8_Display::clear()
{
    std::fill_n(framebuffer, 64 * 32, 0xFF000000);
}

/*
    Toggles a pixel at the desired framebuffer location;
*/
void NC8_Display::togglePixel(int pixel)
{
    if(framebuffer[pixel] == 0xFFFFFFFF)
    {
        framebuffer[pixel] = 0x00000000;
        return;
    }

    framebuffer[pixel] = 0xFFFFFFFF;
}

/**
    Draws a pixel at the desired framebuffer location.
*/
void NC8_Display::drawPixel(int pixel)
{
    framebuffer[pixel] = 0xFFFFFFFF;
}

void NC8_Display::drawSprite(uint8_t vx, uint8_t vy, const uint8_t* n_bytes, size_t size, std::function<void()> vf_handler)
{
    bool collision = false;

    for (int row = 0; row < size; row++)
    {
        uint8_t spriteByte = n_bytes[row];

        for (int col = 0; col < 8; col++)
        {
            bool pixelOn = (spriteByte & (0x80 >> col)) != 0;

            if(pixelOn)
            {
                uint8_t x = (vx + col) % CHIP8_DISPLAY_SIZE_X;
                uint8_t y = (vy + row) % CHIP8_DISPLAY_SIZE_Y;

                size_t index = getFramebufferIndex(x, y);

                if(isPixelOn(index))
                {
                    collision = true;
                }

                togglePixel(index);
            }
        }
    }

    if(collision)
    {
        vf_handler();
    }
}

int NC8_Display::getFramebufferIndex(uint8_t x, uint8_t y)
{
    return x + (y * CHIP8_DISPLAY_SIZE_X);
}

bool NC8_Display::isPixelOn(int index)
{
    return framebuffer[index] == 0xFFFFFFFF;
}

/*
    Returns the index of the matching keycode from the keycodes list when
    a key press is registered through SDL.
*/
uint8_t NC8_Display::getKey()
{
    SDL_Event event;
    std::cout << "NEOCHIP-OK: Entering LISTEN state until key is pressed. \n";
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_EVENT_KEY_DOWN) {
            SDL_KeyboardEvent& keyEvent = event.key;
            auto it = std::find(std::begin(keycodes), std::end(keycodes), keyEvent.key);
            if(it != std::end(keycodes))
            {
                uint8_t dst = std::distance(std::begin(keycodes), it);
                std::cout << "NEOCHIP-OK: Exiting LISTEN state; Key with index " << static_cast<int>(dst) << " pressed.\n";
                return dst;
            }
        }
    }
    return 0xFF;
}

bool NC8_Display::isKeyDown(uint16_t scancode)
{
    const bool* keystate = SDL_GetKeyboardState(nullptr);
    return keystate[scancode];
}

void NC8_Display::updateDisplay()
{
    SDL_UpdateTexture(texture, nullptr, framebuffer, 64 * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void NC8_Display::killDisplay()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}