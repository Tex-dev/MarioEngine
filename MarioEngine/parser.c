#include "parser.h"

//int ReadLine(FILE* file, char** line);

element* NextLine(FILE* file, const char* seps)
{
	element* result = NULL;
	char* copyLine = NULL;
	char* context = NULL;
	char* word = NULL;
	char* line = NULL;
	char isLine = 0;
	int length;
	int index;
	int i;

	if (!file)
		return NULL;

	/*
	if ((lineLength = ReadLine(file, &line)) <= 0)
		return NULL;//*/

	while (!isLine)
	{
		length = ReadLine(file, &line);

		if (length < 0)
			return NULL;

		if (length > 0 && line && line[0] != '#')
			isLine = 1;
		else
			free(line);
	}

//	printf("%s\n", line);
	
	for (i = 0; i < length; i++)
	{
		if (line[i] == '#')
		{
			line[i] = '\0';
			break;
		}
	}

	result = (element*)calloc(1, sizeof(element));
	if (!result)
		return NULL;
	result->nbValue = 0;


	length = strlen(line);
	copyLine = (char*)calloc(length + 1, sizeof(char));
	if (!copyLine)
		return NULL;
	strcpy_s(copyLine, length+1, line);
	
	word = strtok_s(copyLine, seps, &context);
	word = strtok_s(NULL, seps, &context);
	while (word)
	{
		result->nbValue++;
		word = strtok_s(NULL, seps, &context);
	}
	free(copyLine);


	result->token = line;

	result->value = (char**)calloc(result->nbValue, sizeof(char*));
	if (!result->value)
		return result;

	word = strtok_s(line, seps, &context);
	word = strtok_s(NULL, seps, &context);
	index = 0;
	while (word)
	{
		result->value[index] = word;
		
		word = strtok_s(NULL, seps, &context);
		index++;
	}

	return result;



	/*
	word = strtok_s(line, seps, &context);
	word = strtok_s(NULL, seps, &context);
	index = 0;
	while (word)
	{
		length = strlen(word);
		result->value[index] = (char*)calloc(length + 1, sizeof(char));
		if (!result->value[index])
			return result;
		strcpy_s(result->value[index], length, word);

		word = strtok_s(NULL, seps, &context);
		index++;
	}

	free(line);

	return result;//*/
}

int ReadLine(FILE* file, char** line)
{
	long int pos = ftell(file);
	int count = 0;
	int c, i;

	if (!line)
		return -2;

	while ((c = fgetc(file)) != EOF && c != '\n')
		count++;

	if (c == EOF && !count)
		return -1;

	fseek(file, pos, SEEK_SET);

	*line = (char*)calloc(count + 1, sizeof(char));
	if (!*line)
		return -2;

	for (i = 0; i < count; i++)
	{
		c = fgetc(file);
		(*line)[i] = c;
	}

	// Read the '\n' character
	c = fgetc(file);

	return count;
}

void FreeElement(element* elmt)
{
	if (!elmt)
		return;

	if (elmt->value)
		free(elmt->value);

	if (elmt->token)
		free(elmt->token);

	free(elmt);
}
