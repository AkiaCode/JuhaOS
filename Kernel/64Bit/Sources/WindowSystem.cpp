#include <WindowSystem.h>
#include <MemoryManagement.h>
#include <Hal.h>
#include <Task.h>
#include <string.h>
#include <Common.h>

static Window::Manager WindowManager;

static void MouseWindow(void);
static void SystemWindow(void);

void Window::UpdateWindowByCoord(int X1 , int Y1 , int X2 , int Y2) {
	int i;
	int X;
	int Y;
	int BufferX;
	int BufferY;

	int BufferX1;
	int BufferY1;
	int BufferX2;
	int BufferY2;
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
	for(i = 0; i <= WINDOW_MAXCOUNT; i++) {
		Window = &(WindowManager.Windows[i]);
		if(Window->Using == FALSE) {
			continue;
		}
		BufferX1 = X1-Window->Layer.X1;
		BufferY1 = Y1-Window->Layer.Y1;
		BufferX2 = X2-Window->Layer.X1;
		BufferY2 = Y2-Window->Layer.Y1;
		if(BufferX1 < 0) {
			BufferX1 = 0;
		}
		if(BufferY1 < 0) {
			BufferY1 = 0;
		}
		if(BufferX2 > Window->Layer.X2-Window->Layer.X1) {
			BufferX2 = Window->Layer.X2-Window->Layer.X1;
		}
		if(BufferY2 > Window->Layer.Y2-Window->Layer.Y1) {
			BufferY2 = Window->Layer.Y2-Window->Layer.Y1;
		}
		for(BufferY = BufferY1; BufferY < BufferY2; BufferY++) {
			Y = Window->Layer.Y1+BufferY;
			for(BufferX = BufferX1; BufferX < BufferX2; BufferX++) {
				X = Window->Layer.X1+BufferX;
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

static WINDOW *CreateSystemWindow(char *Title , QWORD Flags  , QWORD X , QWORD Y , QWORD Width , QWORD Height , WORD BackgroundColor , int Priority) {
	__asm__ ("cli");
	WindowManager.Windows[Priority].Using = TRUE;
	WindowManager.Windows[Priority].BackgroundColor = BackgroundColor;
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
	Window::UpdateWindow(&(WindowManager.Windows[Priority]));
	WindowManager.Windows[Priority].EventQueue.Initialize(WINDOW_EVENT_QUEUE_MAXSIZE);
	__asm__("sti");
	return &(WindowManager.Windows[Priority]);
}

static void MakeWindowHighest(WINDOW *Window) {
	Graphics::DrawTaskbar(&(Window->Layer) , Window->Layer.X1 , Window->Layer.Y1 , Window->Layer.X2-Window->Layer.X1 , Window->Title , 1);
}

static void MakeWindowNoHighest(WINDOW *Window) {
	Graphics::DrawTaskbar(&(Window->Layer) , Window->Layer.X1 , Window->Layer.Y1 , Window->Layer.X2-Window->Layer.X1 , Window->Title , 0);
}

void Window::Initialize(void) {
	int i;
	VBEMODEINFOBLOCK *Block = (VBEMODEINFOBLOCK*)VBEMODEINFOBLOCK_STARTADDRESS;
	__asm__ ("cli");
	for(i = 0; i < WINDOW_MAXCOUNT; i++) {
		WindowManager.Windows[i].Flags = WINDOW_FLAGS_NONE;
		WindowManager.Windows[i].Using = FALSE;
	}
	WindowManager.WindowCount = 0;
	WindowManager.VideoMemory = (WORD*)Block->Address;
	WindowManager.Width = Block->Width;
	WindowManager.Height = Block->Height;
	WindowManager.TopWindowPriority = -1;
	WindowManager.Windows = (WINDOW*)Memory::malloc((WINDOW_MAXCOUNT+1)*sizeof(WINDOW));
	WindowManager.BackgroundWindow = CreateSystemWindow("BackgroundWindow" , WINDOW_FLAGS_NO_TITLEBAR , 0 , 0 , Block->Width , Block->Height , WINDOW_DEFAULTWALLPAPERCOLOR , 0);
	Task::CreateTask((QWORD)MouseWindow , TASK_DEFAULT , TASK_PRIORITY_HIGH , "MouseSystem" , "");
	__asm__("sti");
	delay(100);
}

WINDOW *Window::CreateWindow(char *Title , QWORD Flags  , QWORD X , QWORD Y , QWORD Width , QWORD Height , WORD BackgroundColor) {
	int i;
	int WindowIndex;
	__asm__ ("cli");
	for(i = 0; i < WINDOW_MAXCOUNT; i++) {
		if(WindowManager.Windows[i].Using == FALSE) {
			WindowManager.Windows[i].Using = TRUE;
			break;
		}
	}
	WindowManager.Windows[i].InvisibleColorUsing = FALSE;
	WindowManager.Windows[i].InvisibleColor = 0x00;
	WindowManager.Windows[i].BackgroundColor = BackgroundColor;
	strcpy(WindowManager.Windows[i].Title , Title);
	Layer::SetLayer(&(WindowManager.Windows[i].Layer) , X , Y , X+Width , Y+Height , (WORD*)NULL);
	WindowManager.Windows[i].Layer.Buffer = (WORD*)Memory::malloc(Width*Height*sizeof(WORD));
	if(((Flags & WINDOW_FLAGS_NO_TITLEBAR) == WINDOW_FLAGS_NO_TITLEBAR)||(Flags == WINDOW_FLAGS_NO_TITLEBAR)) {
		Graphics::DrawRectangle(&(WindowManager.Windows[i].Layer) , 0 , 0 , Width , Height , BackgroundColor);
	}
	else {
		Graphics::DrawWindow(&(WindowManager.Windows[i].Layer) , 0 , 0 , Width , Height , Title , BackgroundColor);
	}
	WindowIndex = i;
	WindowManager.WindowCount++;
	for(i = 1; i < WindowManager.WindowCount; i++) {
		if(WindowManager.Windows[i].Using == FALSE) {
			continue;
		} 
		if((WindowManager.Windows[i].Flags == WINDOW_FLAGS_NO_TITLEBAR)||((WindowManager.Windows[i].Flags & WINDOW_FLAGS_NO_TITLEBAR) == WINDOW_FLAGS_NO_TITLEBAR)) {
			continue;
		}
		Graphics::DrawTaskbar(&(WindowManager.Windows[i].Layer) , 0 , 0 , WindowManager.Windows[i].Layer.X2-WindowManager.Windows[i].Layer.X1 , WindowManager.Windows[i].Title , 0);
		UpdateWindowByCoord(WindowManager.Windows[i].Layer.X1 , WindowManager.Windows[i].Layer.Y1 , WindowManager.Windows[i].Layer.X2 , WindowManager.Windows[i].Layer.Y1+GRAPHICS_WINDOW_TITLEBARSIZE+5);
	}
	Graphics::DrawTaskbar(&(WindowManager.Windows[WindowIndex].Layer) , 0 , 0 , WindowManager.Windows[WindowIndex].Layer.X2-WindowManager.Windows[WindowIndex].Layer.X1 , WindowManager.Windows[WindowIndex].Title , 1);
	UpdateWindow(&(WindowManager.Windows[WindowIndex]));
	WindowManager.Windows[i].EventQueue.Initialize(WINDOW_EVENT_QUEUE_MAXSIZE);
	__asm__ ("sti");
	return &(WindowManager.Windows[i]);
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

	if(Window == WindowManager.BackgroundWindow) {
		return;
	}
	if(Window->Using == FALSE) {
		return;
	}
	
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
	int i;
	int WindowIndex;
	WINDOW WindowToMove;
	if(Window == WindowManager.BackgroundWindow) {
		return;
	}
	if(Window->Using == FALSE) {
		return;
	}
	for(i = 1; i < WINDOW_MAXCOUNT-1; i++) {
		if(Window == &(WindowManager.Windows[i])) {
			WindowIndex = i;
			break;
		}
	}
	if(i >= WindowManager.WindowCount) {
		Graphics::DrawTaskbar(&(WindowManager.Windows[i].Layer) , 0 , 0 , WindowManager.Windows[i].Layer.X2-WindowManager.Windows[i].Layer.X1 , WindowManager.Windows[i].Title , 1);
		UpdateWindowByCoord(WindowManager.Windows[i].Layer.X1 , WindowManager.Windows[i].Layer.Y1 , WindowManager.Windows[i].Layer.X2 , WindowManager.Windows[i].Layer.Y1+GRAPHICS_WINDOW_TITLEBARSIZE+5);
		return;
	}
	memcpy(&(WindowToMove) , &(WindowManager.Windows[WindowIndex]) , sizeof(WindowManager.Windows[WindowIndex]));

	for(i = WindowIndex; i < WindowManager.WindowCount; i++) {
		WindowManager.Windows[i] = WindowManager.Windows[i+1];
	}
	memcpy(&(WindowManager.Windows[WindowManager.WindowCount]) , &(WindowToMove) , sizeof(WindowToMove));

	for(i = 1; i < WindowManager.WindowCount; i++) {
		if(WindowManager.Windows[i].Using == FALSE) {
			continue;
		}
		if((WindowManager.Windows[i].Flags == WINDOW_FLAGS_NO_TITLEBAR)||((WindowManager.Windows[i].Flags & WINDOW_FLAGS_NO_TITLEBAR) == WINDOW_FLAGS_NO_TITLEBAR)) {
			continue;
		}
		Graphics::DrawTaskbar(&(WindowManager.Windows[i].Layer) , 0 , 0 , WindowManager.Windows[i].Layer.X2-WindowManager.Windows[i].Layer.X1 , WindowManager.Windows[i].Title , 0);
		UpdateWindowByCoord(WindowManager.Windows[i].Layer.X1 , WindowManager.Windows[i].Layer.Y1 , WindowManager.Windows[i].Layer.X2 , WindowManager.Windows[i].Layer.Y1+GRAPHICS_WINDOW_TITLEBARSIZE+5);
	}
	Graphics::DrawTaskbar(&(WindowManager.Windows[WindowManager.WindowCount].Layer) , 0 , 0 , WindowManager.Windows[WindowManager.WindowCount].Layer.X2-WindowManager.Windows[WindowManager.WindowCount].Layer.X1 , WindowManager.Windows[WindowManager.WindowCount].Title , 1);
	UpdateWindow(&(WindowManager.Windows[WindowIndex]));
	UpdateWindow(&(WindowManager.Windows[WindowManager.WindowCount]));
	return;
}

int Window::GetWindowPriority(WINDOW *Window) {
	int i;
	if(Window == WindowManager.BackgroundWindow) {
		return 0;
	}
	if(Window->Using == FALSE) {
		return -1;
	}
	for(i = 0; i < WINDOW_MAXCOUNT; i++) {
		if(Window == &(WindowManager.Windows[i])) {
			return i;
		}
	}
	return -1;
}

WINDOW *Window::GetWindowUsingCoord(int X , int Y) {
	int i;
	WINDOW *Window;
	for(i = WINDOW_MAXCOUNT-1; i > 1; i--) {
		Window = &(WindowManager.Windows[i]);
		if(Window->Using == FALSE) {
			continue;
		}
		if((X > Window->Layer.X1) && (X < Window->Layer.X2) && (Y > Window->Layer.Y1) && (Y < Window->Layer.Y2)) {
			return Window;
		}
	}
}

BOOL Window::DeleteWindow(WINDOW *Window) {
	int i;
	int X1;
	int Y1;
	int X2;
	int Y2;
	__asm__ ("cli");
	if(Window == WindowManager.BackgroundWindow) {
		__asm__("sti");
		return FALSE;
	}
	if(Window == WindowManager.MouseWindow) {
		__asm__("sti");
		return FALSE;
	}
	if(Window->Using == FALSE) {
		__asm__("sti");
		return FALSE;
	}
	for(i = 1; i < WindowManager.WindowCount; i++) {
		if(WindowManager.Windows[i].Using == FALSE) {
			continue;
		}
		if(Window == &(WindowManager.Windows[i])) {
			break;
		}
	}
	WindowManager.Windows[i].Using = FALSE;
	X1 = WindowManager.Windows[i].Layer.X1;
	Y1 = WindowManager.Windows[i].Layer.Y1;
	X2 = WindowManager.Windows[i].Layer.X2;
	Y2 = WindowManager.Windows[i].Layer.Y2;
	UpdateWindowByCoord(X1 , Y1 , X2 , Y2);
	__asm__("sti");
	return TRUE;
}

static Common::Queue KeyboardQueue;

static void KeyboardController(void) {
	BYTE Keyboard;
	while(1) {
		KeyboardQueue.Enqueue(getch());
	}
}

void Window::WindowSystem(void) {
	WINDOWEVENT Event;
	KeyboardQueue.Initialize(2048);
	Task::CreateTask((QWORD)KeyboardController , TASK_SYSTEM , TASK_PRIORITY_HIGH , "MouseQueueSystem" , "");
	while(1) {
		if(KeyboardQueue.CheckEmpty() != TRUE) {
			Event.Type = WINDOW_EVENT_KEYBOARD;
			Event.KeyboardData = KeyboardQueue.Dequeue();
			WindowManager.Windows[WindowManager.WindowCount].EventQueue.SendEvent(Event);
		}
	}
}

void MouseWindow(void) {
	int X;
	int Y;
	int DX;
	int DY;
	int Button;
	VBEMODEINFOBLOCK *Block = (VBEMODEINFOBLOCK*)VBEMODEINFOBLOCK_STARTADDRESS;
	WINDOW *Window;
	X = (Block->Width-GRAPHICS_MOUSE_WIDTH)/2;
	Y = (Block->Height-GRAPHICS_MOUSE_HEIGHT)/2;
	WindowManager.MouseWindow = CreateSystemWindow("MouseWindow" , WINDOW_FLAGS_NO_TITLEBAR , X , Y , GRAPHICS_MOUSE_WIDTH , GRAPHICS_MOUSE_HEIGHT , GRAPHICS_MOUSE_INVISIBLECOLOR , WINDOW_MAXCOUNT-2);
	Graphics::DrawCursor(&(WindowManager.MouseWindow->Layer) , 0 , 0);
	WindowManager.MouseWindow->InvisibleColorUsing = TRUE;
	WindowManager.MouseWindow->InvisibleColor = GRAPHICS_MOUSE_INVISIBLECOLOR;
	Window::UpdateWindow(WindowManager.MouseWindow);
	while(1) {
		if(Hal::Mouse::GetMouseData(&(DX) , &(DY) , &(Button)) == FALSE) {
        	continue;
        }

        if(Button & 0x01) {
        	Window = Window::GetWindowUsingCoord(X , Y);
		    Window::ChangeWindowToTop(Window);
        	if(Y-Window->Layer.Y1 < GRAPHICS_WINDOW_TITLEBARSIZE+5) {
	        	Window::MoveWindow(Window , Window->Layer.X1+DX , Window->Layer.Y1+DY);
	        	if((X > Window->Layer.X2-(2*2)-GRAPHICS_WINDOW_TITLEBARSIZE) && (X < Window->Layer.X2-(2*2)-GRAPHICS_WINDOW_TITLEBARSIZE+GRAPHICS_WINDOW_XBUTTONWIDTH) && (Y > Window->Layer.Y1+2+3) && (Y < Window->Layer.Y1+2+3+GRAPHICS_WINDOW_XBUTTONHEIGHT)) {
		        	//Tell'em this window has been removed by Window System
		        	continue;
				}
	        }
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

void WindowEventQueue::Initialize(int MaxSize) {
	if(MaxSize > QUEUE_MAXSIZE) {
        MaxSize = QUEUE_MAXSIZE;
    }
	this->MaxSize = MaxSize;
	this->Rear = 0;
	this->Front = 0;
	this->Buffer = (WINDOWEVENT*)Memory::malloc(MaxSize*sizeof(WINDOWEVENT));
}

BOOL WindowEventQueue::CheckEmpty(void) {
	if(Rear == Front) {
		return TRUE;
	}
	return FALSE;
}

BOOL WindowEventQueue::CheckFull(void) {
	if((Rear+1)%MaxSize == Front) {
		return TRUE;
	}
	return FALSE;
}

BOOL WindowEventQueue::SendEvent(WINDOWEVENT Event) {
	if(this->CheckFull() == TRUE) {
		return FALSE;
	}
	Rear = (Rear+1)%MaxSize;
	Buffer[Rear] = Event;
	return TRUE;
}

BOOL WindowEventQueue::GetEvent(WINDOWEVENT *Event) {
	if(this->CheckEmpty() == TRUE) {
		return FALSE;
	}
	Front = (Front+1)%MaxSize;
	*Event = Buffer[Front];
	return TRUE;
}