#pragma once
#include <iostream>
#include "SDL2/include/SDL.h"

class Interface 
{
	public:
		Interface(char* screen_title, int screen_width, int screen_height, int screen_scale);
		~Interface();
		void draw_screen(uint32_t screen_frame[], int screen_width);
		void get_input(uint8_t* keypad, bool* quit);

	private:
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		SDL_Texture* texture = nullptr;
};