#ifndef _WINDOWSYSTEM_H_
#define _WINDOWSYSTEM_H_

#include <Types.h>
#include <Layer.h>
#include <Graphics.h>

#define WINDOW_FLAGS_DEFAULT 0x01
#define WINDOW_FLAGS_NO_TITLEBAR 0x02
#define WINDOW_FLAGS_NO_BACKGROUND 0x04
#define WINDOW_MAXCOUNT LAYER_MAXCOUNT

#define WINDOW_TITLECOLOR RGB(0 , 51 , 153)
#define WINDOW_DEFAULTBACKGROUND RGB(0xFF , 0xFF , 0xFF)
#define WINDOW_DEFAULTWALLPAPERCOLOR RGB(103 , 153 , 255)
#define WINDOW_INVISIBLECOLOR 0x01

#define EVENT_NONE
#define EVENT_XBUTTON 1
#define EVENT_MOUSE 2
#define EVENT_MOUSE_CLICKED 4
#define EVENT_KEYBOARD 8

typedef struct {
	BOOL Using;
	BYTE Flags;
	WORD *Buffer;
	LAYER *Layer;
}WINDOW;

typedef struct {
	BYTE Event;

	int MouseX;
	int MouseY;

	BYTE Keyboard;
}EVENT;

namespace Window {
	class Manager {
		public:
			int WindowsCount;
			WINDOW Windows[WINDOW_MAXCOUNT];
			void WindowManagerTask(void);

			WINDOW *BackgroundWindow;
			WINDOW *MouseWindow;
	};
	void Initialize(void);
	void MouseWindow(void);
	void BackgroundWindow(void);
	void EventWindow(void);
	WINDOW *CreateWindow(const char *Title , WORD Flags , int X , int Y , int Width , int Height , WORD BackgroundColor);
	void DrawPixel(WINDOW *Window , int X , int Y , WORD Color);
	void DrawRectangle(WINDOW *Window , int X1 , int Y1 , int X2 , int Y2 , WORD Color);
	void DrawText(WINDOW *Window , const char *Text , int X , int Y , WORD Color);
	void DrawLine(WINDOW *Window , int X1 , int Y1 , int X2 , int Y2 , WORD Color);
	namespace Event {
		EVENT *GetEvent(WINDOW *Window);

	};
};

#endif