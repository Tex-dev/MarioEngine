#pragma once

/*
#include <iostream>
#include <sstream>
#include <vector>
#include <string>//*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdarg.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>


#define RES_X		256
#define	RES_Y		192
//#define	RES_Y		240

#define POS_X		1
#define POS_Y		1

#define MULTIPLIER	4

/*
static SDL_Renderer *	g_renderer;
static SDL_Window *		g_window;//*/

/*
#define max(x,y)	((x)>(y)?(x):(y))
#define min(x,y)	((x)<(y)?(x):(y))//*/

typedef enum eInput
{
	INPUT_UP = 0b00000001,
	INPUT_DOWN = 0b00000010,
	INPUT_LEFT = 0b00000100,
	INPUT_RIGHT = 0b00001000,
	INPUT_START = 0b00010000,
	INPUT_SELECT = 0b00100000,
	INPUT_A = 0b01000000,
	INPUT_B = 0b10000000,
}Input;
