#pragma once
#include "parser.h"
#include "value.h"

typedef struct sLevel Level;

typedef struct sPlayer
{
	int				posX;					/**<	Position of the sprite of the player (bottom-left) in x-axis.	*/
	int				posY;					/**<	Position of the sprite of the player (bottom-left) in y-axis.	*/

	unsigned short	velocity;				/**<	Current speed of the player.									*/

}Player;

Player* LoadPlayer(unsigned int posX, unsigned int posY);
void UpdatePlayer(Player* player, Level* lvl, unsigned char input);
void DrawPlayer(Player* player, SDL_Renderer* renderer, Level* lvl);
