#include <WindowSystem.h>
#include <Task.h>
#include <Graphics.h>
#include <Hal.h>
#include <string.h>

static Window::Manager WindowManager;

void Window::Initialize(void) {
	int i;
	QWORD WindowCount;
	VBEMODEINFOBLOCK *Block = (VBEMODEINFOBLOCK*)VBEMODEINFOBLOCK_STARTADDRESS;
    WORD *Address = (WORD*)Block->Address;
	WindowManager.DefaultBackgroundColor = WINDOW_DEFAULTWALLPAPERCOLOR;
    for(i = 0; i < Block->Width*Block->Height; i++) {
    	Address[i] = WindowManager.DefaultBackgroundColor;
    }
    for(i = 0; i < WINDOW_MAXCOUNT; i++) {
    	WindowManager.Windows[i].ID = 0x00;
    	WindowManager.Windows[i].Using = FALSE;
    }
    WindowManager.WindowsCount = 0;
    WindowManager.VideoMemory = (WORD*)Block->Address;
    WindowManager.Width = Block->Width;
    WindowManager.Height = Block->Height;
    Task::CreateTask((QWORD)Window::UpdateTask , TASK_SYSTEM , "Updater" , "Window manager updater");
    CreateWindow("MouseWindow" , "Window for mouse system" , (QWORD)MouseTask , (Block->Width+32)/2 , (Block->Height+32)/2, GRAPHICS_MOUSE_WIDTH , GRAPHICS_MOUSE_HEIGHT , WINDOW_NONE);
}

QWORD Window::CreateWindow(const char *Title , const char *Description , QWORD EntryPoint , QWORD Flags , int X , int Y , int Width , int Height) {
    int i;
    QWORD TaskFlags = TASK_DEFAULT;
    for(i = 0; i < WINDOW_MAXCOUNT; i++) {
        if(WindowManager.Windows[i].Using == FALSE) {
            break;
        }
    }
    WindowManager.Windows[i].Using = TRUE;
    WindowManager.Windows[i].X = X;
    WindowManager.Windows[i].Y = Y;
    WindowManager.Windows[i].Width = Width;
    WindowManager.Windows[i].Height = Height;
    WindowManager.Windows[i].WindowBuffer = (WORD*)Memory::malloc(Width*Height*sizeof(WORD));
    if((Flags & TASK_SYSTEM) == TASK_SYSTEM) {
        TaskFlags = TASK_SYSTEM;
    }
    WindowManager.Windows[i].ID = Task::CreateTask(EntryPoint , TaskFlags , Title , Description);
    Graphics::DrawRectangle(WindowManager.Windows[i].WindowBuffer , 0 , 0 , X+Width , Y+Height , WINDOW_DEFAULTBACKGROUND);
	if((Flags & WINDOW_TITLEBAR) == WINDOW_TITLEBAR) {
		Graphics::DrawWindow(WindowManager.Windows[i].WindowBuffer , 0 , 0 , Width , Height , Title);
	}
    Window::MoveWindow(WindowManager.Windows[i].ID , X , Y);
    WindowManager.Windows[i].Priority = WindowManager.WindowsCount;
    WindowManager.WindowsCount++;
    return WindowManager.Windows[i].ID;
}
 
void Window::MoveWindow(QWORD ID , int X , int Y) {
    int i;
    for(i = 0; i < WINDOW_MAXCOUNT; i++) {
        if((WindowManager.Windows[i].ID == ID) && (WindowManager.Windows[i].Using == TRUE)) {
            break;
        }
    }
    WindowManager.Windows[i].X = X;
    WindowManager.Windows[i].Y = Y;
}

void Window::Update(void) {
    int i;
    for(i = 0; i < WINDOW_MAXCOUNT; i++) {
        if(WindowManager.Windows[i].Using == TRUE) {
            memcpy(WindowManager.VideoMemory+((WORD)((WindowManager.Windows[i].Y*WindowManager.Width)+WindowManager.Windows[i].X)) , WindowManager.Windows[i].WindowBuffer , WindowManager.Windows[i].Width*WindowManager.Windows[i].Height*sizeof(WORD));
        }
    }
}

WORD *Window::GetCurrentWindowBuffer(void) {
    int i;
    for(i = 0; i < WINDOW_MAXCOUNT; i++) {
        if(WindowManager.Windows[i].ID == Task::GetCurrentTaskID()) {
            break;
        }
    }
    return WindowManager.Windows[i].WindowBuffer;
}

void Window::UpdateTask(void) {
    while(1) {
        delay(25);
    }
}

void Window::MouseTask(void) {
	int X;
	int Y;
	int DX;
	int DY;
	int Button;
	QWORD ID;
	VBEMODEINFOBLOCK *Block = (VBEMODEINFOBLOCK*)VBEMODEINFOBLOCK_STARTADDRESS;
    X = (Block->Width+32)/2;
    Y = (Block->Height+32)/2;
    Window::MoveWindow(Task::GetCurrentTaskID() , X , Y);
    Graphics::DrawCursor(GetCurrentWindowBuffer() , 0 , 0);
    Button = 0x00;
    while(1) {
        if(Hal::Mouse::GetMouseData(&(DX) , &(DY) , &(Button)) == FALSE) {
        	continue;
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
        Window::MoveWindow(Task::GetCurrentTaskID() , X , Y);
    	Graphics::DrawCursor(GetCurrentWindowBuffer() , 0 , 0);
        Window::Update();
    }
}