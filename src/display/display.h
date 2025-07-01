#ifndef NC8_DISPLAY_H
#define NC8_DISPLAY_H

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
        void clear();
        void updateDisplay();
        void killDisplay();
        void togglePixel(int pixel);
        void drawPixel(int pixel);
        void drawSprite(uint8_t vx, uint8_t vy, const uint8_t* n_bytes, size_t size, std::function<void()> vf_handler);
        int getFramebufferIndex(uint8_t x, uint8_t y);
        bool isPixelOn(int index);

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
};

#endif