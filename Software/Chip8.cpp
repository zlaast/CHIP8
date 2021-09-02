#include "chip8.h"

Chip8::Chip8()
{
	// Set initial values
	I = 0;
	SP = 0;
	PC = 0x200;
	delay_timer = 0;
	sound_timer = 0;
	memset(V, 0, sizeof(V));
	memset(stack, 0, sizeof(stack));
	memset(memory, 0, sizeof(memory));
	memset(screen, 0, sizeof(screen));
	
	// Seed random number
	srand((unsigned)time(0));

	// Load fontset into Chip8 memory
	for (int i = 0; i < 80; i++)
		memory[i] = fontset[i];

	// Map opcodes to functions
	OpMap[0x00E0] = &Chip8::_00E0;
	OpMap[0x00EE] = &Chip8::_00EE;
	OpMap[0x1000] = &Chip8::_1nnn;
	OpMap[0x2000] = &Chip8::_2nnn;
	OpMap[0x3000] = &Chip8::_3xkk;
	OpMap[0x4000] = &Chip8::_4xkk;
	OpMap[0x5000] = &Chip8::_5xy0;
	OpMap[0x6000] = &Chip8::_6xkk;
	OpMap[0x7000] = &Chip8::_7xkk;
	OpMap[0x8000] = &Chip8::_8xy0;
	OpMap[0x8001] = &Chip8::_8xy1;
	OpMap[0x8002] = &Chip8::_8xy2;
	OpMap[0x8003] = &Chip8::_8xy3;
	OpMap[0x8004] = &Chip8::_8xy4;
	OpMap[0x8005] = &Chip8::_8xy5;
	OpMap[0x8006] = &Chip8::_8xy6;
	OpMap[0x8007] = &Chip8::_8xy7;
	OpMap[0x800E] = &Chip8::_8xyE;
	OpMap[0x9000] = &Chip8::_9xy0;
	OpMap[0xA000] = &Chip8::_Annn;
	OpMap[0xB000] = &Chip8::_Bnnn;
	OpMap[0xC000] = &Chip8::_Cxkk;
	OpMap[0xD000] = &Chip8::_Dxyn;
	OpMap[0xE09E] = &Chip8::_Ex9E;
	OpMap[0xE0A1] = &Chip8::_ExA1;
	OpMap[0xF007] = &Chip8::_Fx07;
	OpMap[0xF00A] = &Chip8::_Fx0A;
	OpMap[0xF015] = &Chip8::_Fx15;
	OpMap[0xF018] = &Chip8::_Fx18;
	OpMap[0xF01E] = &Chip8::_Fx1E;
	OpMap[0xF029] = &Chip8::_Fx29;
	OpMap[0xF033] = &Chip8::_Fx33;
	OpMap[0xF055] = &Chip8::_Fx55;
	OpMap[0xF065] = &Chip8::_Fx65;
}

bool Chip8::load_rom(string rom_name)
{
	ifstream rom(rom_name, ios::binary);
	if (rom.is_open())
	{
		// Get file length
		rom.seekg(0, rom.end);
		int rom_length = rom.tellg();
		rom.seekg(0, rom.beg);

		// Read ROM into buffer
		char* buffer = new char[rom_length];
		rom.read(buffer, rom_length);

		// Copy buffer into memory
		for (int i = 0; i < rom_length; i++)
			memory[0x200 + i] = buffer[i];

		delete[] buffer;
		rom.close();
	}
	else
	{
		cout << "Error: Could not find or load ROM" << endl;
		return false;
	}

	return true;
}

void Chip8::process()
{
	// Get opcode
	opcode = memory[PC] << 8 | memory[PC + 1];					// Load opcode
	x = (opcode & 0x0F00) >> 8;									// Get x nibble
	y = (opcode & 0x00F0) >> 4;									// Get y nibble

	// Get pressed key
	key = V[x];

	// Increase program counter
	PC += 2;

	// Filter opcodes
	switch (opcode & 0xF000)
	{
		case 0x0000:
			execute = OpMap[opcode & 0x00FF];
			break;
		case 0x8000:
			execute = OpMap[opcode & 0xF00F];
			break;
		case 0xE000:
			execute = OpMap[opcode & 0xF0FF];
			break;
		case 0xF000:
			execute = OpMap[opcode & 0xF0FF];
			break;
		default:
			execute = OpMap[opcode & 0xF000];
			break;
	}
	(this->*execute)();

	// Decrement Timers
	if (delay_timer > 0)
		delay_timer--;

	if (sound_timer > 0)
		sound_timer--;
}

void Chip8::_00E0()
{
	memset(screen, 0, sizeof(screen));
}

void Chip8::_00EE()
{
	PC = stack[--SP];
}

void Chip8::_1nnn()
{
	PC = opcode & 0x0FFF;
}

void Chip8::_2nnn()
{
	stack[SP] = PC;
	SP++;
	PC = opcode & 0x0FFF;
}

void Chip8::_3xkk()
{
	if (V[x] == (opcode & 0x00FF))
		PC += 2;
}

void Chip8::_4xkk()
{
	if (V[x] != (opcode & 0x00FF))
		PC += 2;
}

void Chip8::_5xy0()
{
	if (V[x] == V[y])
		PC += 2;
}

void Chip8::_6xkk()
{
	V[x] = opcode & 0x00FF;
}

void Chip8::_7xkk()
{
	V[x] += opcode & 0x00FF;
}

void Chip8::_8xy0()
{
	V[x] = V[y];
}

void Chip8::_8xy1()
{
	V[x] |= V[y];
}

void Chip8::_8xy2()
{
	V[x] &= V[y];
}

void Chip8::_8xy3()
{
	V[x] ^= V[y];
}

void Chip8::_8xy4()
{
	uint16_t sum = V[x] + V[y];
	if (sum > 255)
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[x] += V[y];
}

void Chip8::_8xy5()
{
	if (V[x] > V[y])
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[x] -= V[y];
}

void Chip8::_8xy6()
{
	V[0xF] = V[x] & 0x1;
	V[x] >>= 1;
}

void Chip8::_8xy7()
{
	if (V[y] > V[x])
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[x] = V[y] - V[x];
}

void Chip8::_8xyE()
{
	V[0xF] = (V[x] & 0x80) >> 7;
	V[x] <<= 1;
}

void Chip8::_9xy0()
{
	if (V[x] != V[y])
		PC += 2;
}

void Chip8::_Annn()
{
	I = opcode & 0x0FFF;
}

void Chip8::_Bnnn()
{
	PC = (opcode & 0x0FFF) + V[0];
}

void Chip8::_Cxkk()
{
	uint8_t random_number = rand() % 255;
	V[x] = random_number & (opcode & 0x00FF);
}

void Chip8::_Dxyn()
{
	uint8_t xo = V[x] % screen_width;							// x origin of sprite
	uint8_t yo = V[y] % screen_height;							// y origin of sprite
	uint8_t n = opcode & 0x000F;								// height of sprite
	uint8_t pixel;
	V[0xF] = 0;

	for (int y_row = 0; y_row < n; y_row++)
	{
		pixel = memory[I + y_row];
		for (int x_col = 0; x_col < 8; x_col++)
		{
			if (pixel & (0x80 >> x_col))
			{
				if (screen[(yo + y_row) * screen_width + (xo + x_col)] == 0xDF8139FF)
					V[0xF] = 1;

				screen[(yo + y_row) * screen_width + (xo + x_col)] ^= 0xDF8139FF;
			}
		}
	}
}

void Chip8::_Ex9E()
{
	if (keypad[key])
		PC += 2;
}

void Chip8::_ExA1()
{
	if (!keypad[key])
		PC += 2;
}

void Chip8::_Fx07()
{
	V[x] = delay_timer;
}

void Chip8::_Fx0A()
{
	bool key_pressed = false;
	for (int i = 0; i < 0xF; i++)
	{
		if (keypad[i])
		{
			V[x] = i;
			key_pressed = true;
			break;
		}
	}
	
	if (!key_pressed)
		PC -= 2;
}

void Chip8::_Fx15()
{
	delay_timer = V[x];
}

void Chip8::_Fx18()
{
	sound_timer = V[x];
}

void Chip8::_Fx1E()
{
	I += V[x];
}

void Chip8::_Fx29()
{
	I = V[x] * 0x5;
}

void Chip8::_Fx33()
{
	memory[I] = V[x] / 100;
	memory[I + 1] = V[x] / 10 % 10;
	memory[I + 2] = V[x] % 100 % 10;
}

void Chip8::_Fx55()
{
	for (int i = 0; i <= x; i++)
		memory[I + i] = V[i];
}

void Chip8::_Fx65()
{
	for (int i = 0; i <= x; i++)
		V[i] = memory[I + i];
}