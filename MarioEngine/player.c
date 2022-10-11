#include "player.h"
#include "level.h"

Player* LoadPlayer(unsigned int posX, unsigned int posY)
{
	Player* res;

	res = (Player*)calloc(1, sizeof(Player));
	if (!res)
		return NULL;

	res->posX = posX;
	res->posY = posY;

	return res;
}

void UpdatePlayer(Player* player, Level* lvl, unsigned char input)
{
	unsigned char move = 1;

	if (input & INPUT_B)
		move = 4;

	if (input & INPUT_LEFT)
		player->posX = min(lvl->tiles->resX * (lvl->sizeX - 1), max(0, player->posX - move));

	if (input & INPUT_RIGHT)
		player->posX = min(lvl->tiles->resX * (lvl->sizeX - 1), max(0, player->posX + move));

	if (input & INPUT_UP)
		player->posY = min(lvl->tiles->resY * (lvl->sizeY - 1), max(0, player->posY + move));

	if (input & INPUT_DOWN)
		player->posY = min(lvl->tiles->resY * (lvl->sizeY - 1), max(0, player->posY - move));

	if (input & INPUT_SELECT && input & INPUT_START)
	{
		player->posX = lvl->playerPosX * lvl->tiles->resX;
		player->posY = lvl->playerPosY * lvl->tiles->resY;
	}
}

void DrawPlayer(Player* player, SDL_Renderer* renderer, Level* lvl)
{
	SDL_Rect destRect;

	destRect.w = lvl->tiles->resX * MULTIPLIER;
	destRect.h = lvl->tiles->resY * MULTIPLIER;
	destRect.x = (player->posX - lvl->renderRect.x) * MULTIPLIER;
	destRect.y = lvl->renderRect.h - (player->posY - lvl->renderRect.y) * MULTIPLIER - destRect.h;

	SDL_RenderDrawRect(renderer, &destRect);

	for (int x = -3; x < 3; x++)
	{
		for (int y = -3; y < 3; y++)
		{
			SDL_RenderDrawPoint(renderer, destRect.x + destRect.w / 2 + x, destRect.y + destRect.h / 2 + y);
		}
	}
}
