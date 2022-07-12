#include "game.h"

void DisplayDebugInfos(Game* game);
void DisplayInput(Game* game);

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

//	res->font = TTF_OpenFont("./data/ARLRDBD.TTF", 16);
	res->font = TTF_OpenFont("./data/consola.TTF", 16);
	res->fps = 0;
	res->nbFrame = 0;
	res->showDebug = 0;

	res->input = 0;
	res->showInput = 0;

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

void UpdateGame(Game* game)
{


}

void DisplayDebugInfos(Game* game)
{
	static SDL_Color color = { 255, 0, 0 };
	SDL_Texture *texFPS, *texFrame, *texILabel, *texInput;
	char fps[255] = "Salut", frame[255] = "coucou", input[255], iLabel[255];
	SDL_Surface* surface;
	SDL_Rect dstRect = {0};
	int texW = 0;
	int texH = 0;

	sprintf_s(fps, 255, "%u fps", game->fps);
	sprintf_s(frame, 255, "%u frames", game->nbFrame);
	sprintf_s(iLabel, 255, "input: B A SE ST  R L D U");
	sprintf_s(input, 255, "       %c %c  %c  %c  %c %c %c %c",	game->input & INPUT_B ? '1' : '0',
																						game->input & INPUT_A ? '1' : '0',
																						game->input & INPUT_SELECT ? '1' : '0',
																						game->input & INPUT_START ? '1' : '0',
																						game->input & INPUT_RIGHT ? '1' : '0',
																						game->input & INPUT_LEFT ? '1' : '0',
																						game->input & INPUT_DOWN ? '1' : '0',
																						game->input & INPUT_UP ? '1' : '0');

	surface = TTF_RenderText_Blended(game->font, fps, color);
	texFPS = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = TTF_RenderText_Blended(game->font, frame, color);
	texFrame = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = TTF_RenderText_Blended(game->font, iLabel, color);
	texILabel = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = TTF_RenderText_Blended(game->font, input, color);
	texInput = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);


	dstRect.x = 8;
	dstRect.y = 0;


	SDL_QueryTexture(texFPS, NULL, NULL, &texW, &texH);
	dstRect.w = texW;
	dstRect.h = texH;
	SDL_RenderCopy(game->renderer, texFPS, NULL, &dstRect);

	dstRect.y += texH;
	SDL_QueryTexture(texFrame, NULL, NULL, &texW, &texH);
	dstRect.w = texW;
	dstRect.h = texH;
	SDL_RenderCopy(game->renderer, texFrame, NULL, &dstRect);

	dstRect.y += texH;
	SDL_QueryTexture(texILabel, NULL, NULL, &texW, &texH);
	dstRect.w = texW;
	dstRect.h = texH;
	SDL_RenderCopy(game->renderer, texILabel, NULL, &dstRect);

	dstRect.y += texH;
	SDL_QueryTexture(texInput, NULL, NULL, &texW, &texH);
	dstRect.w = texW;
	dstRect.h = texH;
	SDL_RenderCopy(game->renderer, texInput, NULL, &dstRect);

	SDL_DestroyTexture(texILabel);
	SDL_DestroyTexture(texInput);
	SDL_DestroyTexture(texFrame);
	SDL_DestroyTexture(texFPS);
}

void DisplayInput(Game* game)
{
	SDL_Rect rect, rect2;
	SDL_BlendMode bm;
	Uint8 r, g, b, a;

	rect.x = 878;
	rect.y = 10;
	rect.w = 136;
	rect.h = 36;

	SDL_GetRenderDrawBlendMode(game->renderer, &bm);
	SDL_GetRenderDrawColor(game->renderer, &r, &g, &b, &a);

	SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(game->renderer, 126, 171, 189, 128);

	rect.x = 868;
	rect.y = 0;
	rect.w = 156;
	rect.h = 56;
	SDL_RenderFillRect(game->renderer, &rect);


	rect.w = rect.h = 12;
	rect.x = 878 + 12;
	rect.y = 10 + 12;
	SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 128);
	SDL_RenderFillRect(game->renderer, &rect);

	// LEFT
	rect.x = 878;
	rect.y = 10 + 12;
	if (game->input & INPUT_LEFT)
		SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 128);
	else
		SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 128);
	SDL_RenderFillRect(game->renderer, &rect);

	// RIGHT
	rect.x = 878 + 24;
	rect.y = 10 + 12;
	if (game->input & INPUT_RIGHT)
		SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 128);
	else
		SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 128);
	SDL_RenderFillRect(game->renderer, &rect);

	// UP
	rect.x = 878 + 12;
	rect.y = 10;
	if (game->input & INPUT_UP)
		SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 128);
	else
		SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 128);
	SDL_RenderFillRect(game->renderer, &rect);

	// DOWN
	rect.x = 878 + 12;
	rect.y = 10 + 24;
	if (game->input & INPUT_DOWN)
		SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 128);
	else
		SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 128);
	SDL_RenderFillRect(game->renderer, &rect);

	rect.w = 16;
	rect.h = 8;

	// SELECT
	rect.x = 878 + 44;
	rect.y = 10 + 20;
	if (game->input & INPUT_SELECT)
		SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 128);
	else
		SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 128);
	SDL_RenderFillRect(game->renderer, &rect);

	// START
	rect.x = 878 + 68;
	rect.y = 10 + 20;
	if (game->input & INPUT_START)
		SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 128);
	else
		SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 128);
	SDL_RenderFillRect(game->renderer, &rect);

	// B
	rect2.h = 16;
	rect2.w = 8;
	rect2.x = 878 + 100;
	rect2.y = 10 + 16;

	rect.x = 878 + 96;
	rect.y = 10 + 20;
	if (game->input & INPUT_B)
		SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 128);
	else
		SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 128);
	SDL_RenderFillRect(game->renderer, &rect);
	SDL_RenderFillRect(game->renderer, &rect2);

	// A
	rect2.x = 878 + 124;

	rect.x = 878 + 120;
	rect.y = 10 + 20;
	if (game->input & INPUT_A)
		SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 128);
	else
		SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 128);
	SDL_RenderFillRect(game->renderer, &rect);
	SDL_RenderFillRect(game->renderer, &rect2);



	SDL_SetRenderDrawBlendMode(game->renderer, bm);
	SDL_SetRenderDrawColor(game->renderer, r, g, b, a);
}

void DrawGame(Game* game)
{
	SDL_RenderClear(game->renderer);	
	DrawLevel(game->level, game->renderer, game->level->playerPosX, game->level->playerPosY, game->nbFrame);
	if (game->showDebug)
		DisplayDebugInfos(game);
	if(game->showInput)
		DisplayInput(game);

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
