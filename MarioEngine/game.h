#pragma once

#include "level.h"

typedef struct sGame
{
	SDL_Window *		window;
	SDL_Renderer *		renderer;

	TTF_Font *			font;
	Uint32				fps;
	Uint32				nbFrame;
	unsigned char		showDebug;


	SDL_GameController*	controller;
	SDL_Joystick*		joystick;
	int					instanceID;

	Level *				level;
}Game;

Game* LoadGame(const char * levelPath);
void DrawGame(Game* game);
void FreeGame(Game* game);