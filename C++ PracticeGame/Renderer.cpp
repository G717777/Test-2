//void RenderBackground() 
//{
//	unsigned int* currPixel = (unsigned int*)renderState.bufferMemory;
//	for (int y = 0; y < renderState.bufferHeight; y++)
//	{
//		for (int x = 0; x < renderState.bufferWidth; x++)
//		{
//			*currPixel++ = 0xff785670;
//
//		}
//	}
//}


internal void
ClearScreen(unsigned int color) 
{
	unsigned int* currPixel = (unsigned int*)renderState.bufferMemory;
	for (int y = 0; y < renderState.bufferHeight; y++)
	{
		for (int x = 0; x < renderState.bufferWidth; x++)
		{
			*currPixel++ = color;

		}
	}
}

internal void
DrawRecPixels(int x0, int y0, int x1, int y1, u32 color) 
{
	x0 = Clamp(0, x0, renderState.bufferWidth);
	x1 = Clamp(0, x1, renderState.bufferWidth);
	y0 = Clamp(0, y0, renderState.bufferHeight);
	y1 = Clamp(0, y1, renderState.bufferHeight);
	for (int y = y0; y < y1; y++)
	{
		unsigned int* currPixel = (unsigned int*)renderState.bufferMemory + x0 + y * renderState.bufferWidth;
		for (int x = x0; x < x1; x++)
		{
			*currPixel++ = color;

		}
	}
}

globalVar float renderScale = 0.01f;

internal void
DrawRec(float x, float y, float hX, float hY, u32 color)
{
	//convert normal units to pixels
	// multiply pos and half-size (hX or hY) by screen width or height - gives unit relative to width/ height

	x *= renderState.bufferHeight * renderScale;
	y *= renderState.bufferHeight * renderScale;
	hX *= renderState.bufferHeight * renderScale;
	hY *= renderState.bufferHeight * renderScale;

	//center them
	x += renderState.bufferWidth / 2.0f;
	y += renderState.bufferHeight / 2.0f;

	int x0 = x - hX;
	int x1 = x + hX;
	int y0 = y - hY;
	int y1 = y + hY;

	DrawRecPixels(x0, y0, x1, y1, color);
}
void DrawScore(int num, float x, float y, float size, u32 color)
{
	float halfSize = size * .5f;

	switch (num)
	{
	case 0:
		DrawRec(x - size, y, halfSize, 2.5f * size, color);
		DrawRec(x + size, y, halfSize, 2.5f * size, color);
		DrawRec(x , y + size * 2.f, halfSize, halfSize, color);
		DrawRec(x, y - size * 2.f, halfSize, halfSize, color);
		break;
	case 1:
		DrawRec(x + size, y, halfSize, 2.5f * size, color);
		break;

	case 2:
		DrawRec(x, y + size * 2.f, size * 1.5f, halfSize, color);
		DrawRec(x, y, size * 1.5f, halfSize, color);
		DrawRec(x, y - size * 2.f, size * 1.5f, halfSize, color);
		DrawRec(x + size, y + size, halfSize, halfSize, color);
		DrawRec(x - size, y - size, halfSize, halfSize, color);

		break;

	case 3:
		DrawRec(x - halfSize, y + size * 2.f, size , halfSize, color);
		DrawRec(x - halfSize, y, size, halfSize, color);
		DrawRec(x - halfSize, y - size * 2.f, size, halfSize, color);
		DrawRec(x + size, y , halfSize, size * 2.f, color);
		break;



	default:
		break;
	}

}