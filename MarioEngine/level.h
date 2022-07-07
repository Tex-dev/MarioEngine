#pragma once
#include "parser.h"

typedef struct sAnimatedTile
{
	int *			tiles;			/**<	Array of tiles' ID that compose the animated tile.							*/	

	int				nbTiles;		/**<	Size of the array of tiles' ID.												*/
	int				duration;		/**<	Duration between two tiles (in frame).										*/

}AnimatedTile;

typedef	struct sTileset
{
	SDL_Texture *	sprites;		/**<	"Image" that contains all sprites for level.								*/

	int				resX;			/**<	Size (number of pixels) of each sprite in tileset in x-axis.				*/
	int				resY;			/**<	Size (number of pixels) of each sprite in tileset in y-axis.				*/

	int				sizeX;			/**<	Number of sprite in x-axis.													*/
	int				sizeY;			/**<	Number of sprite in y-axis.													*/

	int				marginX;		/**<	Number of pixels between 2 sprites in x-axis.								*/
	int				marginY;		/**<	Number of pixels between 2 sprites in y-axis.								*/

	AnimatedTile *	animSprites;	/**<	Array of animated sprites.													*/
	int				nbAnimSprites;	/**<	Size of the animated sprites' array.										*/

}Tileset;

typedef struct sLevel
{
	unsigned int	id;				/**<	Unique ID of the level.														*/

	int				playerPosX;		/**<	Position of the player at the beginning of the level in x-axis.				*/
	int				playerPosY;		/**<	Position of the player at the beginning of the level in y-axis.				*/

	Tileset *		tiles;			/**<	The tileset used for the level.												*/

	int				sizeX;			/**<	Size of the level in x-axis (number of sprite).								*/
	int				sizeY;			/**<	Size of the level in y-axis (number of sprite).								*/
	int				depth;			/**<	Number of layers of sprite (without the background).						*/

	short ***		spriteID;		/**<	Arrays of sprites'ID that compose the level (size = sizeX x sizeY x depth).	*/

	int				bckgSpriteID;	/**<	Sprite's ID of the background's level.										*/

}Level;

Level* LoadLevel(const char* path, SDL_Renderer* renderer);
void DrawLevel(Level* lvl, SDL_Renderer* renderer, int posX, int posY, Uint32 frameID);
void FreeLevel(Level* lvl);
