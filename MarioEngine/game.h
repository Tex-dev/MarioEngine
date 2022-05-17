#pragma once

#include "level.h"

typedef struct sGame
{
	SDL_Window *		window;
	SDL_Renderer *		renderer;

	SDL_GameController*	controller;
	SDL_Joystick*		joystick;
	int					instanceID;

	Level *				level;
}Game;

Game* LoadGame(const char * levelPath);
void DrawGame(Game* game);
void FreeGame(Game* game);