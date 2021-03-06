#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <Types.h>
#include <Task.h>

#define VBEMODEINFOBLOCK_STARTADDRESS 0x4000
#define GRAPHICS_WINDOW_PADDING 2
#define GRAPHICS_MOUSE_WIDTH 12
#define GRAPHICS_MOUSE_HEIGHT 12
#define GRAPHICS_WINDOW_XBUTTONWIDTH 13
#define GRAPHICS_WINDOW_XBUTTONHEIGHT 14
#define GRAPHICS_WINDOW_TITLEBARSIZE 14
#define RGB(Red , Green , Blue) (((BYTE)((Red) >> 3) << 11)|((BYTE)((Green) >> 2) << 5)|((BYTE)((Blue) >> 3)))

#define GRAPHICS_MOUSE_INVISIBLECOLOR RGB(42 , 56 , 12)

#define FONT_CONSOLAS_WIDTH 8
#define FONT_CONSOLAS_HEIGHT 16

typedef struct {
	WORD *Buffer;
	int X1;
	int Y1;
	int X2;
	int Y2;
}LAYER;

namespace Graphics {
	void DrawPixel(LAYER *Layer , int X , int Y , WORD Color);
	void DrawLine(LAYER *Layer , int X1 , int Y1 , int X2 , int Y2 , WORD Color);
	void DrawText(LAYER *Layer , const char *Text , int X , int Y , WORD Color);
	void DrawRectangle(LAYER *Layer , int X1 , int Y1 , int X2 , int Y2 , WORD Color);
	void DrawCursor(LAYER *Layer , int X , int Y);
	void DrawTaskbar(LAYER *Layer , int X , int Y , int Width , const char *Title , int Mode);
	void DrawWindow(LAYER *Layer , int X , int Y , int Width , int Height , const char *Title , WORD BackgroundColor);
};

namespace Layer {
	void SetLayer(LAYER *Layer , int X1 , int Y1 , int X2 , int Y2 , WORD *Buffer);
	BOOL GetOverlappedArea(LAYER *Layer1 , LAYER *Layer2 , LAYER *OverlappedArea);
};

#endif