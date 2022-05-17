#include "game.h"

Game* LoadGame(const char* levelPath)
{
	int nbJoystick, i;
	Game* res = NULL;
	res = (Game*)calloc(1, sizeof(Game));
	if (!res)
		return NULL;

	res->window = SDL_CreateWindow("Ma fenetre SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RES_X * MULTIPLIER, RES_Y * MULTIPLIER, SDL_WINDOW_SHOWN);
	if (!res->window)
	{
		printf("[-] ERROR - Failed to create SDL window (%s)\n", SDL_GetError());
		free(res);
		return NULL;
	}

	res->renderer = SDL_CreateRenderer(res->window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(res->renderer, 0, 0, 0, 255);
	SDL_SetRenderDrawColor(res->renderer, 0, 0, 255, 255);

	res->controller = NULL;
	res->joystick = NULL;
	res->instanceID = 0;

	nbJoystick = SDL_NumJoysticks();
	for (i = 0; i < nbJoystick; i++)
	{
		if (SDL_IsGameController(i))
		{
			res->controller = SDL_GameControllerOpen(i);
			if (res->controller)
			{
				res->joystick = SDL_GameControllerGetJoystick(res->controller);
				res->instanceID = SDL_JoystickInstanceID(res->joystick);
				break;
			}
		}
	}
	
	res->level = LoadLevel(levelPath, res->renderer);

	return res;
}

void DrawGame(Game* game)
{
	SDL_RenderClear(game->renderer);
	DrawLevel(game->level, game->renderer, game->level->playerPosX, game->level->playerPosY);
	SDL_RenderPresent(game->renderer);
}

void FreeGame(Game* game)
{
	FreeLevel(game->level);

	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);

	free(game);
}
