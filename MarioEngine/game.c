#include "game.h"

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

	res->font = TTF_OpenFont("./data/consola.TTF", 4*MULTIPLIER);
	res->fps = 0;
	res->nbFrame = 0;
	res->showDebug = 0;

	res->input = 0;
	res->showInput = 1;

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
	SDL_JoystickEventState(SDL_ENABLE);

	printf("[DEBUG] Informations of joystick\n");
	printf("[DEBUG]    Name: %s\n", SDL_JoystickName(0));
	printf("[DEBUG]    Number of axes: %d\n", SDL_JoystickNumAxes(res->joystick));
	printf("[DEBUG]    Number of hats: %d\n", SDL_JoystickNumHats(res->joystick));
	printf("[DEBUG]    Number of trackballs: %d\n", SDL_JoystickNumBalls(res->joystick));
	printf("[DEBUG]    Number of buttons: %d\n", SDL_JoystickNumButtons(res->joystick));
	
	res->level = LoadLevel(levelPath, res->renderer);
	res->player = LoadPlayer(res->level->playerPosX * res->level->tiles->resX, res->level->playerPosX * res->level->tiles->resY);

	return res;
}

void UpdateGame(Game* game)
{
	AddMessageInDebug("input: B A SE ST  R L D U");
	AddMessageInDebug("       %c %c  %c  %c  %c %c %c %c",	game->input & INPUT_B ? '1' : '0',
															game->input & INPUT_A ? '1' : '0',
															game->input & INPUT_SELECT ? '1' : '0',
															game->input & INPUT_START ? '1' : '0',
															game->input & INPUT_RIGHT ? '1' : '0',
															game->input & INPUT_LEFT ? '1' : '0',
															game->input & INPUT_DOWN ? '1' : '0',
															game->input & INPUT_UP ? '1' : '0');

//		game->player->posX = min(game->level->sizeX - 1, max(0, game->player->posX + 1));
//		game->player->posX++;

	UpdatePlayer(game->player, game->level, game->input);
	UpdateLevel(game->level, game->player);
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

//	DrawLevel(game->level, game->renderer, game->level->playerPosX, game->level->playerPosY, game->nbFrame);
	DrawLevel(game->level, game->renderer, game->player, game->nbFrame);
	DrawPlayer(game->player, game->renderer, game->level);

	if (game->showDebug)
		DisplayDebugMessages(game->renderer, game->font);
	if(game->showInput)
		DisplayInput(game);

	SDL_RenderPresent(game->renderer);
}

void FreeGame(Game* game)
{
	FreeLevel(game->level);
	free(game->player);

	TTF_CloseFont(game->font);
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);

	free(game);
}
