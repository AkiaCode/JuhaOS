#include <WindowSystem.h>
#include <MemoryManagement.h>
#include <Hal.h>
#include <Task.h>
#include <string.h>

static Window::Manager WindowManager;

void MouseTask(void);

void Window::Initialize(void) {
	int i;
	VBEMODEINFOBLOCK *Block = (VBEMODEINFOBLOCK*)VBEMODEINFOBLOCK_STARTADDRESS;
	for(i = 0; i < WINDOW_MAXCOUNT; i++) {
		WindowManager.Windows[i].Flags = WINDOW_FLAGS_NONE;
		WindowManager.Windows[i].Using = FALSE;
	}
	WindowManager.WindowCount = 0;
	WindowManager.VideoMemory = (WORD*)Block->Address;
	WindowManager.Width = Block->Width;
	WindowManager.Height = Block->Height;
	WindowManager.TopWindowPriority = -1;
	WindowManager.Windows = (WINDOW*)Memory::malloc(WINDOW_MAXCOUNT*sizeof(WINDOW));
	WindowManager.BackgroundWindow = Window::CreateWindow("BackgroundWindow" , WINDOW_FLAGS_NO_TITLEBAR , 0 , 0 , Block->Width , Block->Height , WINDOW_DEFAULTWALLPAPERCOLOR);
	Task::CreateTask((QWORD)MouseTask , TASK_DEFAULT , "MouseTask" , "");
	delay(100);
}

static WINDOW *CreateSystemWindow(char *Title , QWORD Flags  , QWORD X , QWORD Y , QWORD Width , QWORD Height , WORD BackgroundColor , int Priority) {
	WindowManager.Windows[Priority].Using = TRUE;
	WindowManager.Windows[Priority].InvisibleColorUsing = FALSE;
	WindowManager.Windows[Priority].InvisibleColor = 0x00;
	strcpy(WindowManager.Windows[Priority].Title , Title);
	Layer::SetLayer(&(WindowManager.Windows[Priority].Layer) , X , Y , X+Width , Y+Height , (WORD*)NULL);
	WindowManager.Windows[Priority].Layer.Buffer = (WORD*)Memory::malloc(Width*Height*sizeof(WORD));
	if(((Flags & WINDOW_FLAGS_NO_TITLEBAR) == WINDOW_FLAGS_NO_TITLEBAR)||(Flags == WINDOW_FLAGS_NO_TITLEBAR)) {
		Graphics::DrawRectangle(&(WindowManager.Windows[Priority].Layer) , 0 , 0 , Width , Height , BackgroundColor);
	}
	else {
		Graphics::DrawWindow(&(WindowManager.Windows[Priority].Layer) , 0 , 0 , Width , Height , Title , BackgroundColor);
	}
	Window::DrawWindowInScreen(&(WindowManager.Windows[Priority]));
	return &(WindowManager.Windows[Priority]);
}

WINDOW *Window::CreateWindow(char *Title , QWORD Flags  , QWORD X , QWORD Y , QWORD Width , QWORD Height , WORD BackgroundColor) {
	int i;
	for(i = 0; i < WINDOW_MAXCOUNT; i++) {
		if(WindowManager.Windows[i].Using == FALSE) {
			WindowManager.Windows[i].Using = TRUE;
			break;
		}
	}
	WindowManager.Windows[i].InvisibleColorUsing = FALSE;
	WindowManager.Windows[i].InvisibleColor = 0x00;
	strcpy(WindowManager.Windows[i].Title , Title);
	Layer::SetLayer(&(WindowManager.Windows[i].Layer) , X , Y , X+Width , Y+Height , (WORD*)NULL);
	WindowManager.Windows[i].Layer.Buffer = (WORD*)Memory::malloc(Width*Height*sizeof(WORD));
	if(((Flags & WINDOW_FLAGS_NO_TITLEBAR) == WINDOW_FLAGS_NO_TITLEBAR)||(Flags == WINDOW_FLAGS_NO_TITLEBAR)) {
		Graphics::DrawRectangle(&(WindowManager.Windows[i].Layer) , 0 , 0 , Width , Height , BackgroundColor);
	}
	else {
		Graphics::DrawWindow(&(WindowManager.Windows[i].Layer) , 0 , 0 , Width , Height , Title , BackgroundColor);
	}
	WindowManager.WindowCount++;
	DrawWindowInScreen(&(WindowManager.Windows[i]));
	return &(WindowManager.Windows[i]);
}

static void UpdateWindowByCoord(int X1 , int Y1 , int X2 , int Y2) {
	int i;
	int X;
	int Y;
	int BufferX;
	int BufferY;
	LAYER AreaToUpdate;
	LAYER OverlappedArea;
	WINDOW *Window;
	WORD *VideoMemory = WindowManager.VideoMemory;
	if(X1 < 0) {
		X1 = 0;
	}
	if(Y1 < 0) {
		Y1 = 0;
	}
	if(X2 > WindowManager.Width) {
		X2 = WindowManager.Width;
	}
	if(Y2 > WindowManager.Height) {
		Y2 = WindowManager.Height;
	}
	for(i = 0; i < WINDOW_MAXCOUNT; i++) {
		Window = &(WindowManager.Windows[i]);
		if(Window->Using == FALSE) {
			i++;
			continue;
		}
		for(BufferY = 0; BufferY < Window->Layer.Y2-Window->Layer.Y1; BufferY++) {
			Y = Window->Layer.Y1+BufferY;
			for(BufferX = 0; BufferX < Window->Layer.X2-Window->Layer.X1; BufferX++) {
				X = WindowManager.Windows[i].Layer.X1+BufferX;
				if((X1 <= X) && (X < X2) && (Y1 <= Y) && (Y < Y2)) {
					if(Window->Layer.Buffer[BufferY*(Window->Layer.X2-Window->Layer.X1)+BufferX] != Window->InvisibleColor) {
						VideoMemory[Y*(WindowManager.Width)+X] = Window->Layer.Buffer[BufferY*(Window->Layer.X2-Window->Layer.X1)+BufferX];
					}
					else if(Window->InvisibleColorUsing == FALSE) {
						VideoMemory[Y*(WindowManager.Width)+X] = Window->Layer.Buffer[BufferY*(Window->Layer.X2-Window->Layer.X1)+BufferX];
					}
				}
			}
		}
	}
}

void Window::UpdateWindow(WINDOW *Window) {
	if(Window->Using == TRUE) {
		UpdateWindowByCoord(Window->Layer.X1 , Window->Layer.Y1 , Window->Layer.X2 , Window->Layer.Y2);
	}
}

void Window::MoveWindow(WINDOW *Window , int X , int Y) {
	int LastX = Window->Layer.X1;
	int LastY = Window->Layer.Y1;
	int Width = Window->Layer.X2-Window->Layer.X1;
	int Height = Window->Layer.Y2-Window->Layer.Y1;
	
	Window->Layer.X1 = X;
	Window->Layer.X2 = X+Width;
	Window->Layer.Y1 = Y;
	Window->Layer.Y2 = Y+Height;
	if(Window->Using == TRUE) {
		UpdateWindowByCoord(LastX , LastY , LastX+Width , LastY+Width);
		UpdateWindowByCoord(X , Y , X+Width , Y+Width);
	}
}

void Window::ChangeWindowToTop(WINDOW *Window) {
	WINDOW *TopWindow = NULL;
	if(Window == WindowManager.BackgroundWindow) {
		return;
	}
	if(Window->Using == FALSE) {
		return;
	}
	memcpy(TopWindow , &(WindowManager.Windows[WindowManager.WindowCount]) , sizeof(WindowManager.Windows[WindowManager.WindowCount]));
	memcpy(&(WindowManager.Windows[WindowManager.WindowCount]) , Window , sizeof(Window));
	memcpy(Window , TopWindow , sizeof(Window));
	DrawWindowInScreen(Window);
	return;
}

WINDOW *Window::GetWindowUsingCoord(int X , int Y) {
	int i;
	WINDOW *Window;
	for(i = WINDOW_MAXCOUNT-1; i > 1; i--) {
		Window = &(WindowManager.Windows[i]);
		if(Window->Using == FALSE) {
			i--;
			continue;
		}
		if((X > Window->Layer.X1) && (X < Window->Layer.X2) && (Y > Window->Layer.Y1) && (Y < Window->Layer.Y2)) {
			return Window;
		}
	}
}

void Window::DrawWindowInScreen(WINDOW *Window) {
	int X;
	int Y;
	WORD *VideoMemory = WindowManager.VideoMemory;
	for(Y = 0; Y < Window->Layer.Y2-Window->Layer.Y1; Y++) {
		for(X = 0; X < Window->Layer.X2-Window->Layer.X1; X++) {
			if(Window->Layer.Buffer[Y*(Window->Layer.X2-Window->Layer.X1)+X] != Window->InvisibleColor) {
				VideoMemory[(Y+Window->Layer.Y1)*(WindowManager.Width)+(X+Window->Layer.X1)] = Window->Layer.Buffer[Y*(Window->Layer.X2-Window->Layer.X1)+X];
			}
			else if(Window->InvisibleColorUsing == FALSE) {
				VideoMemory[(Y+Window->Layer.Y1)*(WindowManager.Width)+(X+Window->Layer.X1)] = Window->Layer.Buffer[Y*(Window->Layer.X2-Window->Layer.X1)+X];
			}
		}
	}
}

void MouseTask(void) {
	int X;
	int Y;
	int DX;
	int DY;
	int Button;
	VBEMODEINFOBLOCK *Block = (VBEMODEINFOBLOCK*)VBEMODEINFOBLOCK_STARTADDRESS;

	X = (Block->Width-GRAPHICS_MOUSE_WIDTH)/2;
	Y = (Block->Height-GRAPHICS_MOUSE_HEIGHT)/2;
	WindowManager.MouseWindow = CreateSystemWindow("MouseWindow" , WINDOW_FLAGS_NO_TITLEBAR , X , Y , GRAPHICS_MOUSE_WIDTH , GRAPHICS_MOUSE_HEIGHT , GRAPHICS_MOUSE_INVISIBLECOLOR , WINDOW_MAXCOUNT);
	Graphics::DrawCursor(&(WindowManager.MouseWindow->Layer) , 0 , 0);
	WindowManager.MouseWindow->InvisibleColorUsing = TRUE;
	WindowManager.MouseWindow->InvisibleColor = GRAPHICS_MOUSE_INVISIBLECOLOR;
	Window::UpdateWindow(WindowManager.MouseWindow);

	while(1) {
		if(Hal::Mouse::GetMouseData(&(DX) , &(DY) , &(Button)) == FALSE) {
        	continue;
        }
        if(Button == 1) {
        	Window::ChangeWindowToTop(Window::GetWindowUsingCoord(X , Y));
        }
        X += DX;
        Y += DY;
        if(X <= -GRAPHICS_MOUSE_WIDTH/2) {
            X = -GRAPHICS_MOUSE_WIDTH/2;
        }
        if(Y <= -GRAPHICS_MOUSE_HEIGHT/2) {
            Y = -GRAPHICS_MOUSE_WIDTH/2;
        }
        if(X >= Block->Width-(GRAPHICS_MOUSE_WIDTH/2)) {
            X = Block->Width-(GRAPHICS_MOUSE_WIDTH/2);
        }
        if(X >= Block->Width-(GRAPHICS_MOUSE_WIDTH/2)) {
            X = Block->Width-(GRAPHICS_MOUSE_WIDTH/2);
        }
        if(Y >= Block->Height-(GRAPHICS_MOUSE_HEIGHT/2)) {
            Y = Block->Height-(GRAPHICS_MOUSE_HEIGHT/2);
        }
        Window::MoveWindow(WindowManager.MouseWindow , X , Y);
    }
}