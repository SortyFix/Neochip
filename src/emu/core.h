#include "../display/display.h"
#include <SDL3/SDL_stdinc.h>
#include <cstdint>
#include <string>

#define FONTSET_SIZE 80
#define CYCLES_PER_TIMER 8

class NC8_Core {
    public:
        bool shift_quirk = false;
        bool logic_quirk = false;
        
        NC8_Display* display;
        NC8_Core(std::string path, NC8_Display* dsp, bool shift, bool logic);

        bool draw_now = false;
        
        void tick();
        void reset();
    private:
        int current_tick;
        size_t rom_buffer_size;
        uint16_t pc;            // program counter
        uint8_t sp;             // stack pointer
        uint8_t memory[4096];   // 4096 bytes of memory
        uint16_t stack[16];     // 16 layers of subroutines
        uint8_t g_reg[16];      // general register
        uint16_t i_reg;         // I register
        uint8_t delay;          // delay timer
        uint8_t sound;          // sound timer
        uint16_t opcode;

        void loadROM(std::string path);
        void loadFonts();
        void setVF(bool val);

        void updateDelay();

        void OP_00E0();
        void OP_00EE();
        void OP_1nnn();
        void OP_2nnn();
        void OP_3xkk();
        void OP_4xkk();
        void OP_5xy0();
        void OP_6xkk();
        void OP_7xkk();
        void OP_8xy0();
        void OP_8xy1();
        void OP_8xy2();
        void OP_8xy3();
        void OP_8xy4();
        void OP_8xy5();
        void OP_8xy6();
        void OP_8xy7();
        void OP_8xyE();
        void OP_9xy0();
        void OP_Annn();
        void OP_Bnnn();
        void OP_Cxkk();
        void OP_Dxyn();

        // TODO BEGIN
        void OP_Ex9E();
        void OP_ExA1();
        void OP_Fx07();
        void OP_Fx0A();
        void OP_Fx15();
        void OP_Fx18();
        // TODO END

        void OP_Fx1E();
        void OP_Fx29();
        void OP_Fx33();
        void OP_Fx55();
        void OP_Fx65();

    uint8_t rom_buffer[];
};