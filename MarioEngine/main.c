#include "game.h"

#if __has_include("vld.h")
	#include "vld.h"
#endif

#include <Windows.h>

#define _GAME
//#define _SPRITE_VALUE
//#define _TEST

#define _60HZ

#ifdef _50HZ
	#define FRAME_DURATION	(1.0 / 50.0 * 1000.0)
#else
	#ifdef _60HZ
		#define FRAME_DURATION	(1.0 / 60.0 * 1000.0)
	#endif
#endif

void usleep(__int64 usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}

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

	if (TTF_Init())
	{
		printf("[-] ERROR - Failed to initialise SDL_TTF (%s)\n", SDL_GetError());
		return NULL;
	}

	return LoadGame(levelPath);
}

int Interaction(Game* game)
{
	SDL_Event e;
	int quit = 0;
	static unsigned char ctrl=0, shift=0, alt=0;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			quit = 1;
			break;

		case SDL_KEYUP:
			switch (e.key.keysym.sym)
			{
				case SDLK_LCTRL:
				case SDLK_RCTRL:
					ctrl = 0;
					break;

				case SDLK_UP:
					game->input &= (0xFF - INPUT_UP);
					break;

				case SDLK_DOWN:
					game->input &= (0xFF - INPUT_DOWN);
					break;

				case SDLK_LEFT:
					game->input &= (0xFF - INPUT_LEFT);
					break;

				case SDLK_RIGHT:
					game->input &= (0xFF - INPUT_RIGHT);
					break;

				case SDLK_RETURN:
					game->input &= (0xFF - INPUT_START);
					break;

				case SDLK_SPACE:
					game->input &= (0xFF - INPUT_SELECT);
					break;

				case SDLK_w:
					game->input &= (0xFF - INPUT_B);
					break;

				case SDLK_c:
					game->input &= (0xFF - INPUT_A);
					break;
			}
			break;

		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					quit = 1;
					break;

				case SDLK_d:
					if(ctrl)
						game->showDebug = !game->showDebug;
					break;

				case SDLK_i:
					if(ctrl)
						game->showInput = !game->showInput;
					break;

				case SDLK_LCTRL:
				case SDLK_RCTRL:
					ctrl=1;
					break;

				/*
				case SDLK_UP:
					game->level->playerPosY++;
					break;

				case SDLK_DOWN:
					game->level->playerPosY = min(game->level->sizeY - 1, max(0, game->level->playerPosY - 1));
					break;

				case SDLK_LEFT:
					game->level->playerPosX = min(game->level->sizeX - 1, max(0, game->level->playerPosX - 1));
					break;

				case SDLK_RIGHT:
					game->player->posX++;
					break;//*/

				case SDLK_UP:
					game->input |= INPUT_UP;
					break;

				case SDLK_DOWN:
					game->input |= INPUT_DOWN;
					break;

				case SDLK_LEFT:
					game->input |= INPUT_LEFT;
					break;

				case SDLK_RIGHT:
					game->input |= INPUT_RIGHT;
					break;

				case SDLK_RETURN:
					game->input |= INPUT_START;
					break;

				case SDLK_SPACE:
					game->input |= INPUT_SELECT;
					break;

				case SDLK_w:
					game->input |= INPUT_B;
					break;

				case SDLK_c:
					game->input |= INPUT_A;
					break;
			}

//			std::cout << "posX : " << posX << " - posY : " << posY << " === " << posY * 73 + posX << "     \r";
//			printf("posX : %d - posY : %d === %d     \r", posX, posY, posY * 73 + posX);//*/
			break;

		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
//			printf("[DEBUG] Button : %d\n", e.jbutton.button);
			switch (e.jbutton.button)
			{
				case 0:
					game->input = (e.jbutton.type == SDL_JOYBUTTONDOWN) ? game->input | INPUT_A : game->input & (0xFF - INPUT_A);
					break;

				case 2:
					game->input = (e.jbutton.type == SDL_JOYBUTTONDOWN) ? game->input | INPUT_B : game->input & (0xFF - INPUT_B);
					break;

				case 6:
					game->input = (e.jbutton.type == SDL_JOYBUTTONDOWN) ? game->input | INPUT_SELECT : game->input & (0xFF - INPUT_SELECT);
					break;

				case 7:
					game->input = (e.jbutton.type == SDL_JOYBUTTONDOWN) ? game->input | INPUT_START : game->input & (0xFF - INPUT_START);
					break;
			}
			break;

		case SDL_JOYHATMOTION:
		{
			unsigned char value = game->input & (INPUT_A | INPUT_B | INPUT_START | INPUT_SELECT);
			if (e.jhat.value & SDL_HAT_LEFT)
				value |= INPUT_LEFT;
			if (e.jhat.value & SDL_HAT_RIGHT)
				value |= INPUT_RIGHT;
			if (e.jhat.value & SDL_HAT_UP)
				value |= INPUT_UP;
			if (e.jhat.value & SDL_HAT_DOWN)
				value |= INPUT_DOWN;

			game->input = value;
			break;
		}

		/*
		case SDL_JOYAXISMOTION:
			// Movement of axis
			printf("Movement of axis\n");
			printf("%d is the new value of the axis %d for the joystick %d\n", e.jaxis.value, e.jaxis.axis, e.jaxis.which);
			break;//*/

		default:
			break;
		}

	}

//	printf("ctrl: %d\r", ctrl);

	return quit;
}

#ifdef _GAME
int main(int argc, char** argv)
{
	Uint32 beginFrame, endFrame;
	Uint32 oldTime, time;
	Uint32 deltaTimeMilli;
	double deltaTime;
	Game* game;

	// TODO: Player Test
	SDL_Surface* playerSur;
	SDL_Texture* playerTex;
	SDL_Rect srcRect = { 1, 16, 16, 16 };

	/*
	Value v = Short2Value(MAX_VELOCITY_RUNNING);
	short s = Value2Short(v);

	v = Short2Value(MAX_VELOCITY_WALKING);
	s = Value2Short(v);

	v = Short2Value(ACCELERATION);
	s = Value2Short(v);

	v = Short2Value(SKID_DECELERATION_NORMAL);
	s = Value2Short(v);//*/

	if (!(game = Init("./data/lvl1-1.conf")))
		return EXIT_FAILURE;

//	double duration_60 = 1.0 / 60.0 * 1000.0;
//	double duration_50 = 1.0 / 50.0 * 1000.0;
	double duration;

//	printf("60FPS, frame duration: %f\n", 1.0 / 60.0 * 1000.0);
//	printf("50FPS, frame duration: %f\n", 1.0 / 50.0 * 1000.0);

	// TODO: Player Test
	playerSur = IMG_Load("./data/mario-luigi-4.png");
	SDL_SetColorKey(playerSur, SDL_TRUE, SDL_MapRGB(playerSur->format, 224, 163, 216));
	playerTex = SDL_CreateTextureFromSurface(game->renderer, playerSur);
	SDL_FreeSurface(playerSur);

//	lvl = LoadLevel("./data/lvl1-1.conf");
	oldTime = SDL_GetTicks();
	while (!Interaction(game))
	{
		beginFrame = SDL_GetTicks();
		//*
		time = beginFrame;
		deltaTimeMilli = time - oldTime;
		deltaTime = (time - oldTime)/1000.0;
		game->fps = (int)(1.0 / deltaTime);
		
		AddMessageInDebug("%u fps", game->fps);
		AddMessageInDebug("%u frames", game->nbFrame);

		oldTime = time;//*/

		UpdateGame(game);

		/*
		SDL_RenderClear(g_renderer);
		DrawLevel(lvl, lvl->playerPosX, lvl->playerPosY);
		SDL_RenderPresent(g_renderer);//*/
		DrawGame(game);

//		Sleep(1);
		endFrame = SDL_GetTicks();

		duration = FRAME_DURATION - (endFrame - beginFrame);
//		duration *= 1000;

//		printf("FPS: %4d - Duration: %f                           \r", (int)(1.0 / deltaTime), duration);
		if (duration > 0)
			Sleep((DWORD)duration);
//			usleep((long long)duration);

		ClearDebugMessages();
		game->nbFrame++;
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

	TTF_Quit();
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

	// Gear		: x=22	y=12	nb=4		id: 
	// Roulette : x=41	y=6		nb=3		id: 479
	// Muncher	: x=55	y=0		nb=3		id: 
	// Coin		: x=56	y=4		nb=3		id: 348
	// Brick	: x=61	y=0		nb=4		id: 61
	// ?-block	: x=67	y=0		nb=4		id: 67

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