#pragma once

#include "level.h"
#include "player.h"

typedef enum eInput
{
	INPUT_UP		= 0b00000001,
	INPUT_DOWN		= 0b00000010,
	INPUT_LEFT		= 0b00000100,
	INPUT_RIGHT		= 0b00001000,
	INPUT_START		= 0b00010000,
	INPUT_SELECT	= 0b00100000,
	INPUT_A			= 0b01000000,
	INPUT_B			= 0b10000000,
}Input;

typedef struct sGame
{
	SDL_Window *		window;
	SDL_Renderer *		renderer;

	TTF_Font *			font;
	Uint32				fps;
	Uint32				nbFrame;
	unsigned char		showDebug;

	unsigned char		input;
	unsigned char		showInput;

	SDL_GameController*	controller;
	SDL_Joystick*		joystick;
	int					instanceID;

	Level *				level;
	Player *			player;
}Game;

Game* LoadGame(const char * levelPath);
void UpdateGame(Game* game);
void DrawGame(Game* game);
void FreeGame(Game* game);