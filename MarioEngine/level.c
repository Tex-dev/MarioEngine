#include "Level.h"

Level* LoadLevel(const char* path, SDL_Renderer* renderer)
{
    Level* res = NULL;
    element* elemt;
    int i, j, k;
    FILE* file;

    fopen_s(&file, path, "r");
    if (!file)
    {
        printf("ERROR : can't open %s.\n", path);
        return NULL;
    }

    res = (Level*)calloc(1, sizeof(Level));
    if (!res)
        return NULL;

    while (elemt = NextLine(file, "\t"))
    {
        /*
        printf("%s\n", elemt->token);
        for (i = 0; i < elemt->nbValue; i++)
            printf("\t%d) %s\n", i, elemt->value[i]);
        printf("\n");//*/

        switch (elemt->token[0])
        {
            case 'f':
            {
                Tileset* tiles = (Tileset*)calloc(1, sizeof(Tileset));
                if (!tiles)
                    break;

                SDL_Surface* img = IMG_Load(elemt->value[0]);
                SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 224, 163, 216));
                tiles->sprites = SDL_CreateTextureFromSurface(renderer, img);
                SDL_FreeSurface(img);
                if (!tiles->sprites)
                    printf("[-] ERROR - Failed to load texture (%s)\n", SDL_GetError());

                tiles->sizeX = atoi(elemt->value[1]);
                tiles->sizeY = atoi(elemt->value[2]);

                tiles->resX = atoi(elemt->value[3]);
                tiles->resY = atoi(elemt->value[4]);

                tiles->marginX = atoi(elemt->value[5]);
                tiles->marginY = atoi(elemt->value[6]);

                tiles->nbAnimSprites = atoi(elemt->value[7]);
                tiles->animSprites = (AnimatedTile*)calloc(tiles->nbAnimSprites, sizeof(AnimatedTile));
                if (!tiles->animSprites)
                    printf("[-] ERROR - Error during the allocation of the array of animated sprites\n");

                res->tiles = tiles;

                break;
            }

            case 'l':
                res->id = atoi(elemt->value[0]);

                res->sizeX = atoi(elemt->value[1]);
                res->sizeY = atoi(elemt->value[2]);
                res->depth = atoi(elemt->value[3]);

                res->bckgSpriteID = atoi(elemt->value[4]);

                res->spriteID = (short***)calloc(res->sizeX, sizeof(short(**)));
                if (!res->spriteID)
                    return NULL;

                for (i = 0; i < res->sizeX; i++)
                {
                    res->spriteID[i] = (short**)calloc(res->sizeY, sizeof(short*));
                    if (!res->spriteID[i])
                        return res;

                    for (j = 0; j < res->sizeY; j++)
                    {
                        res->spriteID[i][j] = (short*)calloc(res->depth, sizeof(short));
                        if (!res->spriteID[i][j])
                            return res;

                        for (k = 0; k < res->depth; k++)
                            res->spriteID[i][j][k] = -1;
                    }
                }
                break;

            case 'p':
                res->playerPosX = atoi(elemt->value[0]);
                res->playerPosY = atoi(elemt->value[1]);
                break;

            case 'b':
            case 'e':
            case 't':
            {
                int x, y, z;
                int spriteID;

                x = atoi(elemt->value[0]);
                y = atoi(elemt->value[1]);
                z = atoi(elemt->value[2]);

                spriteID = atoi(elemt->value[3]);

                res->spriteID[x][y][z] = spriteID;

                break;
            }

            case 'a':
            {
                int animID = atoi(elemt->value[0]);

                if (!res->tiles || !res->tiles->animSprites)
                    break;

                if (animID >= res->tiles->nbAnimSprites)
                {
                    printf("[-] ERROR - Animated sprites %d is out of array size.\n", animID);
                    break;
                }

                res->tiles->animSprites[animID].duration = atoi(elemt->value[1]);
                res->tiles->animSprites[animID].nbTiles = elemt->nbValue - 2;
                res->tiles->animSprites[animID].tiles = (int*)calloc(res->tiles->animSprites[animID].nbTiles, sizeof(int));
                if (!res->tiles->animSprites[animID].tiles)
                {
                    printf("[-] ERROR - Error during the allocation of the animated sprites with id %d.\n", animID);
                    break;
                }

                for (i = 2; i < elemt->nbValue; i++)
                    res->tiles->animSprites[animID].tiles[i - 2] = atoi(elemt->value[i]);

                break;
            }

            default:
                break;
        }

        FreeElement(elemt);
    }

    fclose(file);

    res->renderRect.x = 0;
    res->renderRect.y = 0;
    res->renderRect.w = RES_X * MULTIPLIER;
    res->renderRect.h = RES_Y * MULTIPLIER;

    return res;
}

void UpdateLevel(Level* lvl, Player* player)
{
    int distX = player->posX - lvl->renderRect.x;
    int distY = player->posY - lvl->renderRect.y;

    int spriteSizeX = lvl->tiles->resX;
    int spriteSizeY = lvl->tiles->resY;

    //TODO: Change this, maybe by config file in level struct
    int borderL = 7 * spriteSizeX;
    int borderR = 8 * spriteSizeX;
    int borderT = 7 * spriteSizeY;
    int borderD = (int)(4.5 * spriteSizeY);

    if (distX < borderL)
        lvl->renderRect.x = player->posX - borderL;
    else if (distX > borderR)
        lvl->renderRect.x = player->posX - borderR;

//    lvl->renderRect.x = max(0, min((lvl->sizeX * spriteSizeX) - borderR, lvl->renderRect.x));
    lvl->renderRect.x = max(0, min((lvl->sizeX * spriteSizeX) - lvl->renderRect.w / MULTIPLIER, lvl->renderRect.x));


    if (distY < borderD)
        lvl->renderRect.y = player->posY - borderD;
    else if (distY > borderT)
        lvl->renderRect.y = player->posY - borderT;

    //    lvl->renderRect.x = max(0, min((lvl->sizeX * spriteSizeX) - borderR, lvl->renderRect.x));
    lvl->renderRect.y = max(0, min((lvl->sizeY * spriteSizeY) - lvl->renderRect.h / MULTIPLIER, lvl->renderRect.y));


    AddMessageInDebug("player pos : (%d ; %d)", player->posX, player->posY);
    AddMessageInDebug("render pos : (%d ; %d)", lvl->renderRect.x, lvl->renderRect.y);
}

/*
void DrawLevelOld(Level* lvl, SDL_Renderer* renderer, int posX, int posY, Uint32 frameID)
{
    int nbSpriteX, nbSpriteY;
    int nbSpritesInSheet;
    SDL_Rect bkgdRect;
    int begX, begY;
    int i, j, k;

    nbSpritesInSheet = lvl->tiles->sizeX * lvl->tiles->sizeY;

    nbSpriteX = RES_X / 16;
    nbSpriteY = RES_Y / 16;

    begX = min(lvl->sizeX - nbSpriteX, max(0, posX - (nbSpriteX / 2 - 1)));
    begY = min(lvl->sizeY - nbSpriteY, max(0, posY - (nbSpriteY / 2 - 1)));

    bkgdRect.w = lvl->tiles->resX;
    bkgdRect.h = lvl->tiles->resY;
    bkgdRect.x = (lvl->bckgSpriteID % lvl->tiles->sizeX) * (lvl->tiles->resX + lvl->tiles->marginX) + lvl->tiles->marginX;
    bkgdRect.y = (lvl->bckgSpriteID / lvl->tiles->sizeX) * (lvl->tiles->resY + lvl->tiles->marginY) + lvl->tiles->marginY;

    for (i = 0; i < nbSpriteX; i++)
    {
        for (j = 0; j < nbSpriteY; j++)
        {
            SDL_Rect srcRect, destRect;
            int spritePosX, spritePosY;

            destRect.w = lvl->tiles->resX * MULTIPLIER;
            destRect.h = lvl->tiles->resY * MULTIPLIER;
            destRect.x = i * destRect.w;
            destRect.y = (nbSpriteY - (j + 1)) * destRect.h;

            srcRect.w = lvl->tiles->resX;
            srcRect.h = lvl->tiles->resY;

            // On dessine le background
            SDL_RenderCopy(renderer, lvl->tiles->sprites, &bkgdRect, &destRect);

            for (k = 0; k < lvl->depth; k++)
            {
                if (lvl->spriteID[begX + i][begY + j][k] > -1)
                {
                    int posX;
                    int posY;

                    if (lvl->spriteID[begX + i][begY + j][k] >= nbSpritesInSheet)                // It's an animated sprites
                    {
                        AnimatedTile at = lvl->tiles->animSprites[lvl->spriteID[begX + i][begY + j][k] - nbSpritesInSheet];

                        posX = at.tiles[(frameID/at.duration)%at.nbTiles] % lvl->tiles->sizeX;
                        posY = at.tiles[(frameID/at.duration)%at.nbTiles] / lvl->tiles->sizeX;
                    }
                    else                                                                        // It's a static sprites
                    {
                        posX = lvl->spriteID[begX + i][begY + j][k] % lvl->tiles->sizeX;
                        posY = lvl->spriteID[begX + i][begY + j][k] / lvl->tiles->sizeX;
                    }

                    //TODO : On dessine le sprite
                    srcRect.x = posX * (lvl->tiles->resX + lvl->tiles->marginX) + lvl->tiles->marginX;
                    srcRect.y = posY * (lvl->tiles->resY + lvl->tiles->marginY) + lvl->tiles->marginY;

                    SDL_RenderCopy(renderer, lvl->tiles->sprites, &srcRect, &destRect);
                }
            }

            if (begX + i == posX && begY + j == posY)
            {
                SDL_RenderDrawRect(renderer, &destRect);

                for (int x = -3; x < 3; x++)
                {
                    for (int y = -3; y < 3; y++)
                    {
                        SDL_RenderDrawPoint(renderer, destRect.x + destRect.w / 2 + x, destRect.y + destRect.h / 2 + y);
                    }
                }
            }
        }
    }
}//*/

void DrawLevel(Level* lvl, SDL_Renderer* renderer, Player* player, Uint32 frameID)
{
    int spritePosX, spritePosY;
    int nbSpriteX, nbSpriteY;
    int nbSpritesInSheet;
    SDL_Rect bkgdRect;
    int i, j, k;

    nbSpritesInSheet = lvl->tiles->sizeX * lvl->tiles->sizeY;

    nbSpriteX = RES_X / lvl->tiles->resX;
    nbSpriteY = RES_Y / lvl->tiles->resY;
    nbSpriteX = lvl->renderRect.x % lvl->tiles->resX ? nbSpriteX + 1 : nbSpriteX;
    nbSpriteY = lvl->renderRect.y % lvl->tiles->resY ? nbSpriteY + 1 : nbSpriteY;

    bkgdRect.w = lvl->tiles->resX;
    bkgdRect.h = lvl->tiles->resY;
    bkgdRect.x = (lvl->bckgSpriteID % lvl->tiles->sizeX) * (lvl->tiles->resX + lvl->tiles->marginX) + lvl->tiles->marginX;
    bkgdRect.y = (lvl->bckgSpriteID / lvl->tiles->sizeX) * (lvl->tiles->resY + lvl->tiles->marginY) + lvl->tiles->marginY;

    spritePosX = lvl->renderRect.x / lvl->tiles->resX;
    /*
    if (spritePosX )//&& lvl->renderRect.x % lvl->tiles->resX)
        spritePosX--;//*/

    spritePosY = lvl->renderRect.y / lvl->tiles->resY;
    /*
    if (spritePosY && lvl->renderRect.y % lvl->tiles->resY)
        spritePosY++;//*/


    for (i = 0; i < nbSpriteX; i++)
    {
        for (j = 0; j < nbSpriteY; j++)
        {
            SDL_Rect srcRect, destRect;

            destRect.w = lvl->tiles->resX * MULTIPLIER;
            destRect.h = lvl->tiles->resY * MULTIPLIER;
            destRect.x = i * destRect.w;
            destRect.y = (nbSpriteY - (j + 1)) * destRect.h;

            if (lvl->renderRect.x % lvl->tiles->resX)
                destRect.x -= (lvl->renderRect.x % lvl->tiles->resX) * MULTIPLIER;

            //*
            if (lvl->renderRect.y % lvl->tiles->resY)
                destRect.y += ((lvl->renderRect.y % lvl->tiles->resY) - lvl->tiles->resY) * MULTIPLIER;//*/

            srcRect.w = lvl->tiles->resX;
            srcRect.h = lvl->tiles->resY;

            // On dessine le background
            SDL_RenderCopy(renderer, lvl->tiles->sprites, &bkgdRect, &destRect);

            for (k = 0; k < lvl->depth; k++)
            {
                if (lvl->spriteID[spritePosX + i][spritePosY + j][k] > -1)
                {
                    int posX;
                    int posY;

                    if (lvl->spriteID[spritePosX + i][spritePosY + j][k] >= nbSpritesInSheet)                // It's an animated sprites
                    {
                        AnimatedTile at = lvl->tiles->animSprites[lvl->spriteID[spritePosX + i][spritePosY + j][k] - nbSpritesInSheet];

                        posX = at.tiles[(frameID / at.duration) % at.nbTiles] % lvl->tiles->sizeX;
                        posY = at.tiles[(frameID / at.duration) % at.nbTiles] / lvl->tiles->sizeX;
                    }
                    else                                                                                     // It's a static sprites
                    {
                        posX = lvl->spriteID[spritePosX + i][spritePosY + j][k] % lvl->tiles->sizeX;
                        posY = lvl->spriteID[spritePosX + i][spritePosY + j][k] / lvl->tiles->sizeX;
                    }

                    //TODO : On dessine le sprite
                    srcRect.x = posX * (lvl->tiles->resX + lvl->tiles->marginX) + lvl->tiles->marginX;
                    srcRect.y = posY * (lvl->tiles->resY + lvl->tiles->marginY) + lvl->tiles->marginY;

                    SDL_RenderCopy(renderer, lvl->tiles->sprites, &srcRect, &destRect);
                }
            }
        }
    }

    /*
//    int nbSpriteX, nbSpriteY;
//    int nbSpritesInSheet;
//    SDL_Rect bkgdRect;
    int begX, begY;
    int i, j, k;

    nbSpritesInSheet = lvl->tiles->sizeX * lvl->tiles->sizeY;

    nbSpriteX = RES_X / 16;
    nbSpriteY = RES_Y / 16;

    begX = min(lvl->sizeX - nbSpriteX, max(0, player->posX - (nbSpriteX / 2 - 1)));
    begY = min(lvl->sizeY - nbSpriteY, max(0, player->posY - (nbSpriteY / 2 - 1)));

    bkgdRect.w = lvl->tiles->resX;
    bkgdRect.h = lvl->tiles->resY;
    bkgdRect.x = (lvl->bckgSpriteID % lvl->tiles->sizeX) * (lvl->tiles->resX + lvl->tiles->marginX) + lvl->tiles->marginX;
    bkgdRect.y = (lvl->bckgSpriteID / lvl->tiles->sizeX) * (lvl->tiles->resY + lvl->tiles->marginY) + lvl->tiles->marginY;

    for (i = 0; i < nbSpriteX; i++)
    {
        for (j = 0; j < nbSpriteY; j++)
        {
            SDL_Rect srcRect, destRect;

            destRect.w = lvl->tiles->resX * MULTIPLIER;
            destRect.h = lvl->tiles->resY * MULTIPLIER;
            destRect.x = i * destRect.w;
            destRect.y = (nbSpriteY - (j + 1)) * destRect.h;

            srcRect.w = lvl->tiles->resX;
            srcRect.h = lvl->tiles->resY;

            // On dessine le background
            SDL_RenderCopy(renderer, lvl->tiles->sprites, &bkgdRect, &destRect);

            for (k = 0; k < lvl->depth; k++)
            {
                if (lvl->spriteID[begX + i][begY + j][k] > -1)
                {
                    int posX;
                    int posY;

                    if (lvl->spriteID[begX + i][begY + j][k] >= nbSpritesInSheet)                // It's an animated sprites
                    {
                        AnimatedTile at = lvl->tiles->animSprites[lvl->spriteID[begX + i][begY + j][k] - nbSpritesInSheet];

                        posX = at.tiles[(frameID / at.duration) % at.nbTiles] % lvl->tiles->sizeX;
                        posY = at.tiles[(frameID / at.duration) % at.nbTiles] / lvl->tiles->sizeX;
                    }
                    else                                                                        // It's a static sprites
                    {
                        posX = lvl->spriteID[begX + i][begY + j][k] % lvl->tiles->sizeX;
                        posY = lvl->spriteID[begX + i][begY + j][k] / lvl->tiles->sizeX;
                    }

                    //TODO : On dessine le sprite
                    srcRect.x = posX * (lvl->tiles->resX + lvl->tiles->marginX) + lvl->tiles->marginX;
                    srcRect.y = posY * (lvl->tiles->resY + lvl->tiles->marginY) + lvl->tiles->marginY;

                    SDL_RenderCopy(renderer, lvl->tiles->sprites, &srcRect, &destRect);
                }
            }

            if (begX + i == player->posX && begY + j == player->posY)
            {
                SDL_RenderDrawRect(renderer, &destRect);

                for (int x = -3; x < 3; x++)
                {
                    for (int y = -3; y < 3; y++)
                    {
                        SDL_RenderDrawPoint(renderer, destRect.x + destRect.w / 2 + x, destRect.y + destRect.h / 2 + y);
                    }
                }
            }
        }
    }//*/

}

void FreeLevel(Level* lvl)
{
    int i, j;

    if (!lvl)
        return;

    if (lvl->spriteID)
    {
        for (i = 0; i < lvl->sizeX; i++)
        {
            if (!lvl->spriteID[i])
                continue;

            for (j = 0; j < lvl->sizeY; j++)
                free(lvl->spriteID[i][j]);
            free(lvl->spriteID[i]);
        }
        free(lvl->spriteID);
    }

    if (lvl->tiles)
    {
        if (lvl->tiles->animSprites)
        {
            for (i = 0; i < lvl->tiles->nbAnimSprites; i++)
                free(lvl->tiles->animSprites[i].tiles);
        }

        free(lvl->tiles->animSprites);
        SDL_DestroyTexture(lvl->tiles->sprites);
        free(lvl->tiles);
    }

    free(lvl);
}
