#include <WindowSystem.h>
#include <Graphics.h>
#include <string.h>

void Window::DrawPixel(WINDOW *Window , int X , int Y , WORD Color) {
	int RealX1 = Window->Layer.X1+X;
	int RealY1 = Window->Layer.Y1+Y;
	int RealX2 = Window->Layer.X1+X+1;
	int RealY2 = Window->Layer.Y1+Y+1;
	if(!((Window->Flags & WINDOW_FLAGS_NO_TITLEBAR)||(Window->Flags == WINDOW_FLAGS_NO_TITLEBAR))) {
		RealX1 += GRAPHICS_WINDOW_PADDING+1;
		RealX2 += GRAPHICS_WINDOW_PADDING+1;
		RealY1 += GRAPHICS_WINDOW_PADDING+1+GRAPHICS_WINDOW_TITLEBARSIZE+1;
		RealY2 += GRAPHICS_WINDOW_PADDING+1+GRAPHICS_WINDOW_TITLEBARSIZE+1;

		X += GRAPHICS_WINDOW_PADDING+1;
		Y += GRAPHICS_WINDOW_PADDING+1+GRAPHICS_WINDOW_TITLEBARSIZE+1;
	}
	Graphics::DrawPixel(&(Window->Layer) , X , Y , Color);
	UpdateWindowByCoord(RealX1 , RealY1 , RealX2 , RealY2);
}

void Window::DrawLine(WINDOW *Window , int X1 , int Y1 , int X2 , int Y2 , WORD Color) {
	int RealX1 = Window->Layer.X1+X1;
	int RealY1 = Window->Layer.Y1+Y1;
	int RealX2 = Window->Layer.X1+X2;
	int RealY2 = Window->Layer.Y1+Y2;
	if(!((Window->Flags & WINDOW_FLAGS_NO_TITLEBAR)||(Window->Flags == WINDOW_FLAGS_NO_TITLEBAR))) {
		RealX1 += GRAPHICS_WINDOW_PADDING+1;
		RealX2 += GRAPHICS_WINDOW_PADDING+1;
		RealY1 += GRAPHICS_WINDOW_PADDING+1+GRAPHICS_WINDOW_TITLEBARSIZE+1;
		RealY2 += GRAPHICS_WINDOW_PADDING+1+GRAPHICS_WINDOW_TITLEBARSIZE+1;

		X1 += GRAPHICS_WINDOW_PADDING+1;
		Y1 += GRAPHICS_WINDOW_PADDING+1+GRAPHICS_WINDOW_TITLEBARSIZE+1;
		X2 += GRAPHICS_WINDOW_PADDING+1;
		Y2 += GRAPHICS_WINDOW_PADDING+1+GRAPHICS_WINDOW_TITLEBARSIZE+1;
	}
	Graphics::DrawLine(&(Window->Layer) , X1 , Y1 , X2 , Y2 , Color);
	UpdateWindowByCoord(RealX1 , RealY1 , RealX2 , RealY2);
}

void Window::DrawText(WINDOW *Window , const char *Text , int X , int Y , WORD Color) {
	int RealX1 = Window->Layer.X1+X;
	int RealY1 = Window->Layer.Y1+Y;
	int RealX2 = Window->Layer.X1+X+(strlen(Text)*FONT_CONSOLAS_WIDTH);
	int RealY2 = Window->Layer.Y1+Y+FONT_CONSOLAS_HEIGHT;
	if(!((Window->Flags & WINDOW_FLAGS_NO_TITLEBAR)||(Window->Flags == WINDOW_FLAGS_NO_TITLEBAR))) {
		RealX1 += GRAPHICS_WINDOW_PADDING+1;
		RealX2 += GRAPHICS_WINDOW_PADDING+1;
		RealY1 += GRAPHICS_WINDOW_PADDING+1+GRAPHICS_WINDOW_TITLEBARSIZE+1;
		RealY2 += GRAPHICS_WINDOW_PADDING+1+GRAPHICS_WINDOW_TITLEBARSIZE+1;

		X += GRAPHICS_WINDOW_PADDING+1;
		Y += GRAPHICS_WINDOW_PADDING+1+GRAPHICS_WINDOW_TITLEBARSIZE+1;
	}
	Graphics::DrawText(&(Window->Layer) , Text , X , Y , Color);
	UpdateWindowByCoord(RealX1 , RealY1 , RealX2 , RealY2);
}

void Window::DrawRectangle(WINDOW *Window , int X1 , int Y1 , int X2 , int Y2 , WORD Color) {
	int RealX1 = Window->Layer.X1+X1;
	int RealY1 = Window->Layer.Y1+Y1;
	int RealX2 = Window->Layer.X1+X2;
	int RealY2 = Window->Layer.Y1+Y2;
	if(!((Window->Flags & WINDOW_FLAGS_NO_TITLEBAR)||(Window->Flags == WINDOW_FLAGS_NO_TITLEBAR))) {
		RealX1 += GRAPHICS_WINDOW_PADDING+1;
		RealX2 += GRAPHICS_WINDOW_PADDING+1;
		RealY1 += GRAPHICS_WINDOW_PADDING+1+GRAPHICS_WINDOW_TITLEBARSIZE+1;
		RealY2 += GRAPHICS_WINDOW_PADDING+1+GRAPHICS_WINDOW_TITLEBARSIZE+1;

		X1 += GRAPHICS_WINDOW_PADDING+1;
		Y1 += GRAPHICS_WINDOW_PADDING+1+GRAPHICS_WINDOW_TITLEBARSIZE+1;
		X2 += GRAPHICS_WINDOW_PADDING+1;
		Y2 += GRAPHICS_WINDOW_PADDING+1+GRAPHICS_WINDOW_TITLEBARSIZE+1;
	}
	Graphics::DrawRectangle(&(Window->Layer) , X1 , Y1 , X2 , Y2 , Color);
	UpdateWindowByCoord(RealX1 , RealY1 , RealX2 , RealY2);
}