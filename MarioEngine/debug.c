#include "debug.h"

void AddMessageInDebug(const char* format, ...)
{
	Message* newMessage, * tmp;
	char buffer[256];
	va_list args;
	va_start(args, format);
	vsprintf_s(buffer, 255, format, args);
	
	newMessage = (Message*)calloc(1, sizeof(Message));
	if (!newMessage)
		return;

	strcpy_s(newMessage->message, 255, buffer);
	newMessage->next = NULL;

	if (!g_debugMessages)
	{
		g_debugMessages = newMessage;
		return;
	}

	tmp = g_debugMessages;
	while (tmp->next)
		tmp = tmp->next;

	tmp->next = newMessage;
	
	va_end(args);
}

void DisplayDebugMessages(SDL_Renderer * renderer, TTF_Font * font)
{
	static SDL_Color color = { 255, 0, 0 };
	Message* current = g_debugMessages;
	SDL_Rect dstRect = { 0 };
	int texW = 0;
	int texH = 0;

	dstRect.x = 8;
	dstRect.y = 0;
	
	while (current)
	{
		SDL_Surface* surface;
		SDL_Texture* texture;

		surface = TTF_RenderText_Blended(font, current->message, color);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		dstRect.y += texH;
		SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
		dstRect.w = texW;
		dstRect.h = texH;
		SDL_RenderCopy(renderer, texture, NULL, &dstRect);

		SDL_DestroyTexture(texture);

		current = current->next;
	}
}

void ClearDebugMessages()
{
	Message* tmp = g_debugMessages;
	Message* next;

	while (tmp)
	{
		next = tmp->next;
		free(tmp);
		tmp = next;
	}

	g_debugMessages = NULL;
}
