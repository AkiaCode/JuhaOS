#ifndef _WINDOWSYSTEM_H_
#define _WINDOWSYSTEM_H_

#include <Types.h>
#include <Layer.h>
#include <Graphics.h>

#define WINDOW_FLAGS_DEFAULT 0x01
#define WINDOW_FLAGS_NO_TITLEBAR 0x02
#define WINDOW_FLAGS_NO_BACKGROUND 0x04
#define WINDOW_MAXCOUNT 512

#define WINDOW_TITLECOLOR RGB(0 , 51 , 153)
#define WINDOW_DEFAULTBACKGROUND RGB(0xFF , 0xFF , 0xFF)
#define WINDOW_DEFAULTWALLPAPERCOLOR RGB(103 , 153 , 255)
#define WINDOW_INVISIBLECOLOR 0x01

typedef struct {
	BOOL Using;
	WORD *Buffer;
	LAYER *Layer;
}WINDOW;

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
	WINDOW *CreateWindow(const char *Title , WORD Flags , int X , int Y , int Width , int Height , WORD Backgroundcolor);

};

#endif