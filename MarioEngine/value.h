#pragma once
#include "debug.h"

#define		MAX_VELOCITY_WALKING		0x0180
#define		MAX_VELOCITY_RUNNING		0x0280
#define		MAX_VELOCITY_SPRINTING		0x0380

#define		ACCELERATION				0x000E

#define		STOP_DECELERATION_NORMAL	0x000E
#define		STOP_DECELERATION_ICE		0x0003
#define		SKID_DECELERATION_NORMAL	0x0020
#define		SKID_DECELERATION_ICE		0x000C

typedef struct sValue
{
	unsigned char	blocks;
	unsigned char	pixels;
	unsigned char	subpixels;
	unsigned char	subsubpixels;
}Value;

Value Short2Value(unsigned short value);
unsigned short Value2Short(Value value);
