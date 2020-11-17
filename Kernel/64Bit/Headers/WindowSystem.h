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
#define WINDOW_DEFAULTBACKGROUND RGB(0xFF , 0xFF , 0xFF)
#define WINDOW_DEFAULTWALLPAPERCOLOR RGB(103 , 153 , 255)
#define WINDOW_INVISIBLECOLOR 0x01

#pragma pack(push , 1)

struct WINDOW {
	QWORD Flags;
	QWORD Using;
	BOOL InvisibleColorUsing;
	WORD InvisibleColor;
	LAYER Layer;
	char Title[128];
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
	void MoveWindow(WINDOW *Window , int X , int Y);
	int GetWindowPriority(WINDOW *Window);
	void ChangeWindowToTop(WINDOW *Window);
	WINDOW *GetWindowUsingCoord(int X , int Y);
	void DrawWindowInScreen(WINDOW *Window);
};

#pragma pack(pop)

#endif