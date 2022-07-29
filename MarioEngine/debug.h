#pragma once
#include "utils.h"

typedef struct sMessage Message;

struct sMessage
{
	char		message[255];
	Message* next;
};

static Message* g_debugMessages = NULL;

void AddMessageInDebug(const char* format, ...);
void DisplayDebugMessages(SDL_Renderer* renderer, TTF_Font* font);
void ClearDebugMessages();
