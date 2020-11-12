#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <Types.h>
#include <Task.h>
#include <Layer.h>

#define VBEMODEINFOBLOCK_STARTADDRESS 0x4000
#define GRAPHICS_MOUSE_WIDTH 32
#define GRAPHICS_MOUSE_HEIGHT 32
#define RGB(Red , Green , Blue) (((BYTE)((Red) >> 3) << 11)|((BYTE)((Green) >> 2) << 5)|((BYTE)((Blue) >> 3)))

namespace Graphics {
	void DrawPixel(LAYER *Layer , int X , int Y , WORD Color , BOOL Update);
	void DrawLine(LAYER *Layer , int X1 , int Y1 , int X2 , int Y2 , WORD Color , BOOL Update);
	void DrawText(LAYER *Layer , const char *Text , int X , int Y , WORD Color , BOOL Update);
	void DrawRectangle(LAYER *Layer , int X1 , int Y1 , int X2 , int Y2 , WORD Color , BOOL Update);
	void DrawEmptyRectangle(LAYER *Layer , int X1 , int Y1 , int X2 , int Y2 , WORD Color , int Padding , BOOL Update);
	void DrawCursor(LAYER *Layer , int X , int Y , BOOL Update);
	void DrawWindow(LAYER *Layer , int X , int Y , int Width , int Height , const char *Title , WORD BackgroundColor , BOOL Update);
};

#endif