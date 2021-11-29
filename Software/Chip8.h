#pragma once
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

class Chip8
{
	public:
		Chip8();										// Class constructor
		bool load_rom(string rom_name);					// Loads ROM into memory
		void process();									// Processes opcodes

		int screen_width = 64;							// Screen display width
		int screen_height = 32;							// Screen display height
		uint32_t screen[64 * 32];						// Screen display buffer

		uint8_t keypad[16];								// Hex keypad input

	private:
		// Chip8 Internals
		uint8_t key;									// Currently pressed key
		uint8_t SP;										// Stack Pointer
		uint8_t V[16];									// General purpose registers
		uint8_t delay_timer;							// Delay timer
		uint8_t sound_timer;							// Sound timer
		uint8_t memory[4096];							// Memory (RAM)
		uint16_t I;										// Instruction register
		uint16_t PC;									// Program Counter
		uint16_t stack[16];								// Stack
		uint16_t opcode;								// Current instruction

		// Chip8 Fontset
		uint8_t fontset[80] =
		{
			0xF0, 0x90, 0x90, 0x90, 0xF0,				// 0
			0x20, 0x60, 0x20, 0x20, 0x70,				// 1
			0xF0, 0x10, 0xF0, 0x80, 0xF0,				// 2
			0xF0, 0x10, 0xF0, 0x10, 0xF0,				// 3
			0x90, 0x90, 0xF0, 0x10, 0x10,				// 4
			0xF0, 0x80, 0xF0, 0x10, 0xF0,				// 5
			0xF0, 0x80, 0xF0, 0x90, 0xF0,				// 6
			0xF0, 0x10, 0x20, 0x40, 0x40,				// 7
			0xF0, 0x90, 0xF0, 0x90, 0xF0,				// 8
			0xF0, 0x90, 0xF0, 0x10, 0xF0,				// 9
			0xF0, 0x90, 0xF0, 0x90, 0x90,				// A
			0xE0, 0x90, 0xE0, 0x90, 0xE0,				// B
			0xF0, 0x80, 0x80, 0x80, 0xF0,				// C
			0xE0, 0x90, 0x90, 0x90, 0xE0,				// D
			0xF0, 0x80, 0xF0, 0x80, 0xF0,				// E
			0xF0, 0x80, 0xF0, 0x80, 0x80				// F
		};

		// Helper Variables
		uint8_t x;										// x nibble of opcode
		uint8_t y;										// y nibble of opcode
		typedef void(Chip8::* operation)();				// Function Pointer typedef
		map<uint16_t, operation> OpMap;					// Maps opcodes to functions, using function pointers. See constructor
		operation execute;								// Holds function (pointer) to be executed. Example: execute = OpMap[opcode & 0xF000]

		// Helper Functions
		void _00E0();									// Clear display
		void _00EE();									// Return from subroutine
		void _1nnn();									// Jump to location nnn
		void _2nnn();									// Call subroutine at nnn
		void _3xkk();									// Skip next instruction if Vx = kk
		void _4xkk();									// Skip next instruction if Vx != kk
		void _5xy0();									// Skip next instruction if Vx = Vy
		void _6xkk();									// Vx = kk
		void _7xkk();									// Vx = Vx + kk
		void _8xy0();									// Vx = Vy
		void _8xy1();									// Vx = Vx | Vy
		void _8xy2();									// Vx = Vx & Vy
		void _8xy3();									// Vx = Vx ^ Vy
		void _8xy4();									// Vx = Vx + Vy, with VF flag
		void _8xy5();									// Vx = Vx - Vy, with VF flag
		void _8xy6();									// Vx = Vx >> 1
		void _8xy7();									// Vx = Vy - Vx, with VF flag
		void _8xyE();									// Vx = Vx << 1
		void _9xy0();									// Skip next instruction if Vx != Vy
		void _Annn();									// I = nnn
		void _Bnnn();									// Jump to location nnn + V0
		void _Cxkk();									// Vx = random byte & kk
		void _Dxyn();									// Draw sprites to display
		void _Ex9E();									// Skip next instruction if key = Vx
		void _ExA1();									// Skip next instruction if key != Vx
		void _Fx07();									// Vx = delay timer
		void _Fx0A();									// Wait for key press, then Vx = key
		void _Fx15();									// delay timer = Vx
		void _Fx18();									// sound timer = Vx
		void _Fx1E();									// I = I + Vx
		void _Fx29();									// I = memory address for hex digit
		void _Fx33();									// Store BCD of Vx into I, I+1, I+2
		void _Fx55();									// Store V0-Vx into memory starting at I
		void _Fx65();									// Read  V0-Vx from memory starting at I
};

