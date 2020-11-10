#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <Types.h>
#include <Task.h>

#define VBEMODEINFOBLOCK_STARTADDRESS 0x4000
#define GRAPHICS_MOUSE_WIDTH 32
#define GRAPHICS_MOUSE_HEIGHT 32
#define RGB(Red , Green , Blue) (((BYTE)((Red) >> 3) << 11)|((BYTE)((Green) >> 2) << 5)|((BYTE)((Blue) >> 3)))
#define GRAPHICS_MAXLAYERCOUNT 512

#pragma pack(push , 1)



#pragma pack(pop)

namespace Graphics {
	void DrawPixel(WORD *WindowBuffer , int X , int Y , WORD Color);
	void DrawLine(WORD *WindowBuffer , int X1 , int Y1 , int X2 , int Y2 , WORD Color);
	void DrawRectangle(WORD *WindowBuffer , int X1 , int Y1 , int X2 , int Y2 , WORD Color);
	void DrawEmptyRectangle(WORD *WindowBuffer , int X1 , int Y1 , int X2 , int Y2 , WORD Color , int Padding);
	void DrawCursor(WORD *WindowBuffer , int X , int Y);
	void DrawWindow(WORD *WindowBuffer , int X , int Y , int Width , int Height , const char *Title);
};

#endif