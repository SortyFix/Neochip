#ifndef NC8_DISPLAY_H
#define NC8_DISPLAY_H

#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <functional>

#define CHIP8_DISPLAY_SIZE_X 64
#define CHIP8_DISPLAY_SIZE_Y 32


class NC8_Display
{
    public:
        NC8_Display(int width = 1024, int height = 512);
        ~NC8_Display();
        uint32_t framebuffer[64 * 32];
        uint16_t scancodes[16] = {
            SDL_SCANCODE_0,
            SDL_SCANCODE_1,
            SDL_SCANCODE_2,
            SDL_SCANCODE_3,
            SDL_SCANCODE_4,
            SDL_SCANCODE_5,
            SDL_SCANCODE_6,
            SDL_SCANCODE_7,
            SDL_SCANCODE_8,
            SDL_SCANCODE_9,
            SDL_SCANCODE_A,
            SDL_SCANCODE_B,
            SDL_SCANCODE_C,
            SDL_SCANCODE_D,
            SDL_SCANCODE_E,
            SDL_SCANCODE_F
        };
        SDL_Keycode keycodes[16] = {SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8, 
                                SDLK_9, SDLK_A, SDLK_B, SDLK_C, SDLK_D, SDLK_E, SDLK_F};
        void clear();
        void updateDisplay();
        void killDisplay();
        void togglePixel(int pixel);
        void drawPixel(int pixel);
        uint8_t getKey();
        void drawSprite(uint8_t vx, uint8_t vy, const uint8_t* n_bytes, size_t size, std::function<void()> vf_handler);
        int getFramebufferIndex(uint8_t x, uint8_t y);
        bool isKeyDown(uint16_t keycode);
        bool isPixelOn(int index);

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
};

#endif