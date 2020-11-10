#ifndef _WINDOWSYSTEM_H_
#define _WINDOWSYSTEM_H_

#include <Types.h>
#include <Graphics.h>

#define WINDOW_BACKGROUNDID 0x400000
#define WINDOW_MAXCOUNT 1024
#define WINDOW_NONE 0
#define WINDOW_HIDE 1
#define WINDOW_TITLEBAR 2

#define WINDOW_TITLECOLOR RGB(0 , 51 , 153)
#define WINDOW_DEFAULTBACKGROUND RGB(0xFF , 0xFF , 0xFF)
#define WINDOW_DEFAULTWALLPAPERCOLOR RGB(103 , 153 , 255)


namespace Window {
	struct WINDOWINFO {
	    int X;
	    int Y;
	    int Width;
	    int Height;
	    int Priority;
		QWORD ID;
	    QWORD Flags;
	    BOOL Using;

	    char Title[128];
	    WORD *WindowBuffer;
	};

	class Manager { 
		friend class Window;
		public:
			WINDOWINFO Windows[WINDOW_MAXCOUNT];
			WORD DefaultBackgroundColor;
			QWORD TopWindowID;
			QWORD WindowsCount;

			WORD *VideoMemory;
			int Width;
			int Height;
	};
	void Initialize(void);
	QWORD CreateWindow(const char *Title , const char *Description , QWORD EntryPoint , QWORD Flags , int X , int Y , int Width , int Height);
	void MoveWindow(QWORD ID , int X , int Y);
	WORD *GetCurrentWindowBuffer(void);
	void Update(void);
	void UpdateTask(void);
	void MouseTask(void);
}

#endif