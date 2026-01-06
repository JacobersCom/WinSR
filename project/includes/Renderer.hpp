#pragma once
#include <stdint.h>


void PlotPixel(int x, int y, uint32_t color,
	void* BitMapMemory, int BytePrePixel, int BitMapWidth);