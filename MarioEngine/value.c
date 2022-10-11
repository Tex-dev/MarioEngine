#include "value.h"

Value Short2Value(unsigned short value)
{
	Value res;

	res.subsubpixels = value % 16;

	value = value >> 4;
	res.subpixels = value % 16;

	value = value >> 4;
	res.pixels = value % 16;

	value = value >> 4;
	res.blocks = value % 16;

	return res;
}

unsigned short Value2Short(Value value)
{
	unsigned short res = 0;

	res += value.subsubpixels;
	res += value.subpixels << 4;
	res += value.pixels << 8;
	res += value.blocks << 12;

	return res;
}
