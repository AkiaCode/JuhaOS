#ifndef _WINDOWSYSTEM_H_
#define _WINDOWSYSTEM_H_

#include <Types.h>
#include <Graphics.h>

#define WINDOW_MAXCOUNT 512

#define WINDOW_BACKGROUNDID 0x200000

#define WINDOW_FLAGS_NONE 0xFF
#define WINDOW_FLAGS_DEFAULT 0x01
#define WINDOW_FLAGS_NO_TITLEBAR 0x02
#define WINDOW_FLAGS_NO_BACKGROUND 0x04

#define WINDOW_TITLECOLOR RGB(0 , 51 , 153)
#define WINDOW_TITLECOLOR_UNSELECTED RGB(160 , 186 , 237)
#define WINDOW_DEFAULTBACKGROUND RGB(0xFF , 0xFF , 0xFF)
#define WINDOW_DEFAULTWALLPAPERCOLOR RGB(103 , 153 , 255)
#define WINDOW_INVISIBLECOLOR 0x01

#define WINDOW_EVENT_KEYBOARD 0x01
#define WINDOW_EVENT_MOUSE 0x02
#define WINDOW_EVENT_QUIT 0x04
#define WINDOW_EVENT_ACTIVATED 0x08
#define WINDOW_EVENT_QUEUE_MAXSIZE 1024

#pragma pack(push , 1)

class WindowEventQueue;

struct WINDOWEVENT {
	DWORD Type;
	union {
		int X;
		int Y;
		int Button;
	}Mouse;
	BYTE KeyboardData;
};

class WindowEventQueue {
	public:
		void Initialize(int MaxSize);
		BOOL CheckEmpty(void);
		BOOL CheckFull(void);
		BOOL SendEvent(WINDOWEVENT Event);
		BOOL GetEvent(WINDOWEVENT *Event);
	private:
		int MaxSize;
		int Front;
		int Rear;
		WINDOWEVENT *Buffer;
};

struct WINDOW {
	QWORD Flags;
	QWORD Using;

	WORD BackgroundColor;

	BOOL InvisibleColorUsing;
	WORD InvisibleColor;
	LAYER Layer;
	char Title[128];

	WindowEventQueue EventQueue;
};

namespace Window {
	class Manager {
		public:
			QWORD TopWindowPriority;
			QWORD WindowCount;
			WINDOW *Windows;

			WORD *VideoMemory;
			int Width;
			int Height;

			WINDOW *MouseWindow;
			WINDOW *BackgroundWindow;
	};
	void Initialize(void);
	WINDOW *CreateWindow(char *Title , QWORD Flags  , QWORD X , QWORD Y , QWORD Width , QWORD Height , WORD BackgroundColor);
	void UpdateWindow(WINDOW *Window);
	void UpdateWindowByCoord(int X1 , int Y1 , int X2 , int Y2);
	void MoveWindow(WINDOW *Window , int X , int Y);
	int GetWindowPriority(WINDOW *Window);
	void ChangeWindowToTop(WINDOW *Window);
	WINDOW *GetWindowUsingCoord(int X , int Y);
	BOOL DeleteWindow(WINDOW *Window);
	void WindowSystem(void);

	void DrawPixel(WINDOW *Window , int X , int Y , WORD Color);
	void DrawLine(WINDOW *Window , int X1 , int Y1 , int X2 , int Y2 , WORD Color);
	void DrawText(WINDOW *Window , const char *Text , int X , int Y , WORD Color);
	void DrawRectangle(WINDOW *Window , int X1 , int Y1 , int X2 , int Y2 , WORD Color);
};

#pragma pack(pop)

#endif