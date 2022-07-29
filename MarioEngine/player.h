#pragma once
#include "parser.h"
#include "debug.h"

typedef struct sLevel Level;

typedef struct sPlayer
{
	int		posX;					/**<	Position of the sprite of the player (bottom-left) in x-axis.	*/
	int		posY;					/**<	Position of the sprite of the player (bottom-left) in y-axis.	*/

}Player;

Player* LoadPlayer(unsigned int posX, unsigned int posY);
void DrawPlayer(Player* player, SDL_Renderer* renderer, Level* lvl);
