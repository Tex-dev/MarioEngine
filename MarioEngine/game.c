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

	res->font = TTF_OpenFont("./data/ARLRDBD.TTF", 16);
	res->showDebug = 0;

	res->nbFrame = 0;

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

void DrawDebugInfos(Game* game)
{
	static SDL_Color color = { 255, 0, 0 };
	SDL_Texture* texFPS, * texFrame;
	char fps[255] = "Salut", frame[255] = "coucou";
	SDL_Surface* surface;
	SDL_Surface* surface2;
	SDL_Rect dstRect = {0};
	int texW = 0;
	int texH = 0;

	sprintf_s(fps, 255, "%u fps", game->fps);
	sprintf_s(frame, 255, "%u frames", game->nbFrame);


	surface = TTF_RenderText_Blended(game->font, fps, color);
	texFPS = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface2 = TTF_RenderText_Blended(game->font, frame, color);
	texFrame = SDL_CreateTextureFromSurface(game->renderer, surface2);
	SDL_FreeSurface(surface2);

	dstRect.x = 8;
	dstRect.y = 0;


	SDL_QueryTexture(texFPS, NULL, NULL, &texW, &texH);
	dstRect.w = texW;
	dstRect.h = texH;
	SDL_RenderCopy(game->renderer, texFPS, NULL, &dstRect);

	dstRect.y = texH;
	SDL_QueryTexture(texFrame, NULL, NULL, &texW, &texH);
	dstRect.w = texW;
	dstRect.h = texH;
	SDL_RenderCopy(game->renderer, texFrame, NULL, &dstRect);

	SDL_DestroyTexture(texFrame);
	SDL_DestroyTexture(texFPS);
}

void DrawGame(Game* game)
{
	SDL_RenderClear(game->renderer);	
	DrawLevel(game->level, game->renderer, game->level->playerPosX, game->level->playerPosY, game->nbFrame);
	if (game->showDebug)
		DrawDebugInfos(game);
	SDL_RenderPresent(game->renderer);
}

void FreeGame(Game* game)
{
	FreeLevel(game->level);

	TTF_CloseFont(game->font);
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);

	free(game);
}
