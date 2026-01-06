#include "Renderer.hpp"
#include "Math.hpp"

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

void ParamertieLine(int x1, int y1, int x2, int y2, uint32_t color, 
	void* BitMapMemory, int BytePrePixel, int BitMapWidth)
{
	//What difference is greater?
	uint32_t deltaX = (x2 - x1);
	uint32_t deltaY = (y2 - y1);
	uint32_t rateOfChange = deltaY > deltaX ? deltaY : deltaX;

	for (uint32_t i = 0; i < rateOfChange; i++)
	{
		//ratio
		float r = (float)i / (float)rateOfChange;
		//lerp
		uint32_t x = Lerp(x1, r, deltaX);
		uint32_t y = Lerp(x1, r, deltaY);
		PlotPixel(x, y, color, BitMapMemory, BytePrePixel, BitMapWidth);
	}
}