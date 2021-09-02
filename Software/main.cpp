#pragma once
#include <iostream>
#include "chip8.h"
#include "interface.h"
using namespace std;

int main(int argc, char* argv[]) 
{
	Chip8 chip8;
	char screen_title[] = "CHIP8 Emulator";

	// Load ROM
	cout << screen_title << endl;
	cout << "Enter ROM: ";
	string rom_name = "";
	cin >> rom_name;

	if (!chip8.load_rom(rom_name))
		return 1;

	// Create display and key input
	int screen_scale = 20;
	Interface io(screen_title, chip8.screen_width, chip8.screen_height, screen_scale);

	// Timing. Lock frame rate
	const int FPS = 500;
	const int frame_delay = 1000 / FPS;
	Uint32 frame_start;
	int frame_time;

	// Process opcodes
	bool quit = false;
	while (!quit)
	{
		frame_start = SDL_GetTicks();							// Start of frame

		// Get key input, process opcodes, update screen
		io.get_input(chip8.keypad, &quit);					// Keypad input
		chip8.process();									// Process opcodes
		io.draw_screen(chip8.screen, chip8.screen_width);	// Draw frame

		frame_time = SDL_GetTicks() - frame_start;				// Check frame time
		if (frame_delay > frame_time)							// Correct frame rate
			SDL_Delay(frame_delay - frame_time);
	}
	return 0;
}