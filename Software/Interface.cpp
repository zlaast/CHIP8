#include "Interface.h"

Interface::Interface(char* screen_title, int screen_width, int screen_height, int screen_scale)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(screen_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width * screen_scale, screen_height * screen_scale, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, screen_width, screen_height);
}

Interface::~Interface()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Interface::draw_screen(uint32_t screen_frame[], int screen_width)
{
	SDL_UpdateTexture(texture, NULL, screen_frame, screen_width * sizeof(uint32_t));
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void Interface::get_input(uint8_t* keypad, bool* quit)
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT:
			*quit = true;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
				case SDLK_1:
					keypad[0x1] = 1;
					break;
				case SDLK_2:
					keypad[0x2] = 1;
					break;
				case SDLK_3:
					keypad[0x3] = 1;
					break;
				case SDLK_4:
					keypad[0xC] = 1;
					break;
				case SDLK_q:
					keypad[0x4] = 1;
					break;
				case SDLK_w:
					keypad[0x5] = 1;
					break;
				case SDLK_e:
					keypad[0x6] = 1;
					break;
				case SDLK_r:
					keypad[0xD] = 1;
					break;
				case SDLK_a:
					keypad[0x7] = 1;
					break;
				case SDLK_s:
					keypad[0x8] = 1;
					break;
				case SDLK_d:
					keypad[0x9] = 1;
					break;
				case SDLK_f:
					keypad[0xE] = 1;
					break;
				case SDLK_z:
					keypad[0xA] = 1;
					break;
				case SDLK_x:
					keypad[0x0] = 1;
					break;
				case SDLK_c:
					keypad[0xB] = 1;
					break;
				case SDLK_v:
					keypad[0xF] = 1;
					break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
				case SDLK_1:
					keypad[0x1] = 0;
					break;
				case SDLK_2:
					keypad[0x2] = 0;
					break;
				case SDLK_3:
					keypad[0x3] = 0;
					break;
				case SDLK_4:
					keypad[0xC] = 0;
					break;
				case SDLK_q:
					keypad[0x4] = 0;
					break;
				case SDLK_w:
					keypad[0x5] = 0;
					break;
				case SDLK_e:
					keypad[0x6] = 0;
					break;
				case SDLK_r:
					keypad[0xD] = 0;
					break;
				case SDLK_a:
					keypad[0x7] = 0;
					break;
				case SDLK_s:
					keypad[0x8] = 0;
					break;
				case SDLK_d:
					keypad[0x9] = 0;
					break;
				case SDLK_f:
					keypad[0xE] = 0;
					break;
				case SDLK_z:
					keypad[0xA] = 0;
					break;
				case SDLK_x:
					keypad[0x0] = 0;
					break;
				case SDLK_c:
					keypad[0xB] = 0;
					break;
				case SDLK_v:
					keypad[0xF] = 0;
					break;
			}
			break;
	}
}