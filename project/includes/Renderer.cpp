#include "Renderer.hpp"

void PlotPixel(int x, int y, uint32_t color,
	void* BitMapMemory, int BytePrePixel, int BitMapWidth)
{
	//Where the bitmap is in memory
	uint8_t* Page = (uint8_t*)BitMapMemory;
	//Lenght per stride/row
	uint32_t pitch = BytePrePixel * BitMapWidth;

	//The Y position 
	uint8_t* Row = Page + y * pitch;

	//The X position. 
	uint32_t* Pixel = (uint32_t*)(Row + x * 4);

	*Pixel = color;
}