#include "game.h"

#if __has_include("vld.h")
	#include "vld.h"
#endif

#include <Windows.h>

#define _GAME
//#define _SPRITE_VALUE
//#define _TEST

Game* Init(const char* levelPath)
{
	if (!SDL_WasInit(SDL_INIT_EVERYTHING))
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			printf("[-] ERROR - Failed to initialise SDL (%s)\n", SDL_GetError());
			return NULL;
		}
	}

	if (!IMG_Init(IMG_INIT_PNG))
	{
		printf("[-] ERROR - Failed to initialise SDL_Image (%s)\n", SDL_GetError());
		return NULL;
	}

	
	int nbJoystick = SDL_NumJoysticks();
	controller = SDL_GameControllerOpen(1);
	if (!controller)
		fprintf(stderr, "Could not open gamecontroller %i: %s\n", 1, SDL_GetError());
	else
	{
		joystick = SDL_GameControllerGetJoystick(controller);
		instanceID = SDL_JoystickInstanceID(joystick);
	}


	return LoadGame(levelPath);
}

int Interaction(Game* game)
{
	SDL_Event e;
	int quit = 0;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			quit = 1;
			break;


		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					quit = 1;
					break;

				//*
				case SDLK_UP:
					game->level->playerPosY++;
					break;

				case SDLK_DOWN:
					game->level->playerPosY = min(game->level->sizeY-1, max(0, game->level->playerPosY - 1));
					break;

				case SDLK_LEFT:
					game->level->playerPosX = min(game->level->sizeX-1, max(0, game->level->playerPosX - 1));
					break;

				case SDLK_RIGHT:
					game->level->playerPosX++;
					break;//*/
			}
//				std::cout << "posX : " << posX << " - posY : " << posY << " === " << posY * 73 + posX << "     \r";
//			printf("posX : %d - posY : %d === %d     \r", posX, posY, posY * 73 + posX);//*/
			break;

		default:
			break;
		}
	}

	return quit;
}

#ifdef _GAME
int main(int argc, char** argv)
{
	Uint32 oldTime, time;
	double deltaTime;
	Game* game;

	if (!(game = Init("./data/lvl1-1.conf")))
		return EXIT_FAILURE;

//	lvl = LoadLevel("./data/lvl1-1.conf");
	oldTime = SDL_GetTicks();
	while (!Interaction(game))
	{
		//*
		time = SDL_GetTicks();
		deltaTime = (time - oldTime)/1000.0;
		printf("FPS : %d                            \r", (int)(1.0/deltaTime));
		oldTime = time;//*/

		/*
		SDL_RenderClear(g_renderer);
		DrawLevel(lvl, lvl->playerPosX, lvl->playerPosY);
		SDL_RenderPresent(g_renderer);//*/
		DrawGame(game);

		Sleep(16);
	}

	/*
//	char* line;
	element* elmt;
	FILE* file;
	int size;


	fopen_s(&file, "./data/lvl1-1.conf", "r");

	if (!file)
		return EXIT_FAILURE;

//	while (ReadLine(file, &line) >= 0)
//	{
//		printf("%s\n", line);
//		free(line);
//	}

	while (elmt = NextLine(file, "\t"))
	{
		printf("%s\n", elmt->token);
		for (size_t i = 0; i < elmt->nbValue; i++)
			printf("\t%d) %s\n", i, elmt->value[i]);
		printf("\n");

		FreeElement(elmt);
	}

	fclose(file);
	//*/
	FreeGame(game);

	SDL_Quit();

	return EXIT_SUCCESS;
}

#else
#ifdef _SPRITE_VALUE

int main(int argc, char* argv[])
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event e;
	int quit = 0;

	SDL_Surface* img;
	SDL_Texture* tiles;
	SDL_Rect srcRect, dstRect;

	int i, j;
	int posX = 0, posY = 0;

	if (!SDL_WasInit(SDL_INIT_VIDEO))
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
//			std::cerr << "[-] ERROR - Failed to initialise SDL (" << SDL_GetError() << ")\n";
			printf("[-] ERROR - Failed to initialise SDL (%s)\n", SDL_GetError());
			return EXIT_FAILURE;
		}
	}

	if (!IMG_Init(IMG_INIT_PNG))
	{
//		std::cerr << "[-] ERROR - Failed to initialise SDL_Image (" << SDL_GetError() << ")\n";
		printf("[-] ERROR - Failed to initialise SDL_Image (%s)\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	window = SDL_CreateWindow("Ma fenetre SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RES_X*MULTIPLIER, RES_Y*MULTIPLIER, SDL_WINDOW_SHOWN);
	if (!window)
	{
//		std::cerr << "[-] ERROR - Failed to create SDL window (" << SDL_GetError() << ")\n";
		printf("[-] ERROR - Failed to create SDL window (%s)\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	img = IMG_Load("data/Stage_tiles.png");
	SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 224, 163, 216));
	tiles = SDL_CreateTextureFromSurface(renderer, img);

	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				quit = 1;
				break;

			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit = 1;
						break;

					case SDLK_UP:
						posY = max(0, posY - 1);
						break;

					case SDLK_DOWN:
						posY++;
						break;

					case SDLK_LEFT:
						posX = max(0, posX - 1);
						break;

					case SDLK_RIGHT:
						posX++;
						break;
				}
//				std::cout << "posX : " << posX << " - posY : " << posY << " === " << posY * 73 + posX << "     \r";
				printf("posX : %d - posY : %d === %d     \r", posX, posY, posY * 73 + posX);
				break;

			default:
				break;
			}
		}

//		std::cout << "posX : " << posX << " - posY : " << posY << " === " << posY * 73 + posX << "     \r";
		
		/*
		srcRect.x = posX * 17 + 1;
		srcRect.y = posY * 17 + 1;
		srcRect.w = 16;
		srcRect.h = 16;

		dstRect.w = 16 * MULTIPLIER;
		dstRect.h = 16 * MULTIPLIER;
		dstRect.x = 0;
		dstRect.y = 0;

//		dstRect.x = ((RES_X*MULTIPLIER)/2) - dstRect.w/2;
//		dstRect.y = ((RES_Y*MULTIPLIER)/2) - dstRect.h/2;

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, tiles, &srcRect, &dstRect);
		SDL_RenderPresent(renderer);//*/

		//*
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		for (i = -7; i < 8; i++)
		{
			for (j = -7; j < 8; j++)
			{
				srcRect.x = (posX + i) * 17 + 1;
				srcRect.y = (posY + j) * 17 + 1;
				srcRect.w = 16;
				srcRect.h = 16;

				dstRect.w = 16 * MULTIPLIER;
				dstRect.h = 16 * MULTIPLIER;
				dstRect.x = ((RES_X * MULTIPLIER) / 2) - (1-2*i)*dstRect.w / 2;
				dstRect.y = ((RES_Y * MULTIPLIER) / 2) - (1-2*j)*dstRect.h / 2;

				SDL_RenderCopy(renderer, tiles, &srcRect, &dstRect);

				if (!i && !j)
					SDL_RenderDrawRect(renderer, &dstRect);

			}
		}
		SDL_RenderPresent(renderer);
		//*/
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}

#else
#ifdef _TEST

int main(int argc, char* argv[])
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event e;
	int quit = 0;

	SDL_Surface* img;
	SDL_Texture* tiles;
	SDL_Rect srcRect, dstRect;

	int i, j;
	int posX = 61, posY = 0;
	int animID = 0;
	int nbSprite = 4;

	// Gear		: x=22	y=12	nb=4
	// Roulette : x=41	y=6		nb=3
	// Muncher	: x=55	y=0		nb=3
	// Coin		: x=56	y=4		nb=3
	// Brick	: x=61	y=0		nb=4
	// ?-block	: x=67	y=0		nb=4

	if (!SDL_WasInit(SDL_INIT_VIDEO))
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			//			std::cerr << "[-] ERROR - Failed to initialise SDL (" << SDL_GetError() << ")\n";
			printf("[-] ERROR - Failed to initialise SDL (%s)\n", SDL_GetError());
			return EXIT_FAILURE;
		}
	}

	if (!IMG_Init(IMG_INIT_PNG))
	{
		//		std::cerr << "[-] ERROR - Failed to initialise SDL_Image (" << SDL_GetError() << ")\n";
		printf("[-] ERROR - Failed to initialise SDL_Image (%s)\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	window = SDL_CreateWindow("Mario", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 16 * MULTIPLIER, 16 * MULTIPLIER, SDL_WINDOW_SHOWN|SDL_WINDOW_BORDERLESS);
	if (!window)
	{
		//		std::cerr << "[-] ERROR - Failed to create SDL window (" << SDL_GetError() << ")\n";
		printf("[-] ERROR - Failed to create SDL window (%s)\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	img = IMG_Load("data/Stage_tiles.png");
	SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 224, 163, 216));
	tiles = SDL_CreateTextureFromSurface(renderer, img);

	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
					quit = 1;
					break;

				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							quit = 1;
							break;
					}
				
					break;

				default:
					break;
			}
		}

		srcRect.x = (posX+animID) * 17 + 1;
		srcRect.y = posY * 17 + 1;
		srcRect.w = 16;
		srcRect.h = 16;

		dstRect.w = 16 * MULTIPLIER;
		dstRect.h = 16 * MULTIPLIER;
		dstRect.x = 0;
		dstRect.y = 0;

//		dstRect.x = ((RES_X*MULTIPLIER)/2) - dstRect.w/2;
//		dstRect.y = ((RES_Y*MULTIPLIER)/2) - dstRect.h/2;

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, tiles, &srcRect, &dstRect);
		SDL_RenderPresent(renderer);

		animID++;
		animID %= nbSprite;
		Sleep(133);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}

#endif
#endif
#endif