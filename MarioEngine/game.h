#pragma once

#include "level.h"
#include "player.h"

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