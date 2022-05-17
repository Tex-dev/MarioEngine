#pragma once

#include "utils.h"

typedef struct sElement
{
	char *	token;

	char**	value;
	int		nbValue;
}element;

element* NextLine(FILE* file, const char* seps);
int ReadLine(FILE* file, char** line);						// Remove
void FreeElement(element* elmt);
