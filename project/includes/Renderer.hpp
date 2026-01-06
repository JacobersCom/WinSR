#pragma once
#include <stdint.h>


void PlotPixel(int x, int y, uint32_t color,
	void* BitMapMemory, int BytePrePixel, int BitMapWidth);

void ParamertieLine(int x1, int y1, int x2, int y2, uint32_t color,
	void* BitMapMemory, int BytePrePixel, int BitMapWidth);