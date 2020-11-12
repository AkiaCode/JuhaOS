#include <WindowSystem.h>
#include <MemoryManagement.h>
#include <Hal.h>
#include <Task.h>

static Window::Manager WindowManager;

static WINDOW *CreateSystemWindow(const char *Title , int X , int Y , int Width , int Height , int Priority , BOOL Move) {
	int i;
	for(i = 0; i < WINDOW_MAXCOUNT; i++) {
		if(WindowManager.Windows[i].Using == FALSE) {
			WindowManager.Windows[i].Using = TRUE;
			break;
		}
	}
	WindowManager.Windows[i].Buffer = (WORD*)Memory::malloc(Width*Height*sizeof(WORD));
	WindowManager.Windows[i].Layer = CreateLayer(WindowManager.Windows[i].Buffer , Width , Height , -1);
	ChangeLayerHeight(WindowManager.Windows[i].Layer , Priority);
	if(Move == TRUE) {
		MoveLayer(WindowManager.Windows[i].Layer , X , Y);
	}
	return &(WindowManager.Windows[i]);
}

void Window::Initialize(void) {
    int i;
    VBEMODEINFOBLOCK *Block = (VBEMODEINFOBLOCK*)VBEMODEINFOBLOCK_STARTADDRESS;
    for(i = 0; i < WINDOW_MAXCOUNT; i++) {
        WindowManager.Windows[i].Using = FALSE;
    }
    WindowManager.WindowsCount = 1;
    InitLayerSystem((WORD*)Block->Address , Block->Width , Block->Height);
    WindowManager.BackgroundWindow = CreateSystemWindow("Background window" , 0 , 0 , Block->Width , Block->Height , 0x00 , FALSE);
    Graphics::DrawRectangle(WindowManager.BackgroundWindow->Layer , 0 , 0 , Block->Width , Block->Height , WINDOW_DEFAULTWALLPAPERCOLOR , TRUE);
    Task::CreateTask((QWORD)MouseWindow , TASK_SYSTEM , "MouseTask" , "Task for Mouse system(GUI)");
    delay(1000);
}

WINDOW *Window::CreateWindow(const char *Title , WORD Flags , int X , int Y , int Width , int Height , WORD BackgroundColor) {
	int i;
	for(i = 0; i < WINDOW_MAXCOUNT; i++) {
		if(WindowManager.Windows[i].Using == FALSE) {
			WindowManager.Windows[i].Using = TRUE;
			break;
		}
	}
	WindowManager.Windows[i].Buffer = (WORD*)Memory::malloc(Width*Height*sizeof(WORD));
	WindowManager.Windows[i].Layer = CreateLayer(WindowManager.Windows[i].Buffer , Width , Height , WINDOW_INVISIBLECOLOR);
	ChangeLayerHeight(WindowManager.Windows[i].Layer , WindowManager.WindowsCount);
	if(Flags == WINDOW_FLAGS_DEFAULT) {
		Graphics::DrawWindow(WindowManager.Windows[i].Layer , 0 , 0 , Width , Height , Title , BackgroundColor , FALSE);
	}
	if((Flags & WINDOW_FLAGS_NO_BACKGROUND) == WINDOW_FLAGS_NO_TITLEBAR) {
		MoveLayer(WindowManager.Windows[i].Layer , X , Y);
		WindowManager.WindowsCount++;
		return &(WindowManager.Windows[i]);
	}
	if((Flags & WINDOW_FLAGS_NO_TITLEBAR) == WINDOW_FLAGS_NO_TITLEBAR) {
		Graphics::DrawRectangle(WindowManager.Windows[i].Layer , 0 , 0 , Width , Height , BackgroundColor , FALSE);
	}
	MoveLayer(WindowManager.Windows[i].Layer , X , Y);
	WindowManager.WindowsCount++;
	return &(WindowManager.Windows[i]);
}

void Window::MouseWindow(void) {
	int X;
	int Y;
	int Button;
	int DX;
	int DY;
    VBEMODEINFOBLOCK *Block = (VBEMODEINFOBLOCK*)VBEMODEINFOBLOCK_STARTADDRESS;
    X = (Block->Width-GRAPHICS_MOUSE_WIDTH)/2;
    Y = (Block->Height-GRAPHICS_MOUSE_HEIGHT)/2;
    WindowManager.MouseWindow = CreateSystemWindow("Mouse window" , (Block->Width-GRAPHICS_MOUSE_WIDTH)/2 , (Block->Height-GRAPHICS_MOUSE_HEIGHT)/2 , GRAPHICS_MOUSE_WIDTH , GRAPHICS_MOUSE_HEIGHT , WINDOW_MAXCOUNT-2 , TRUE);
    Graphics::DrawCursor(WindowManager.MouseWindow->Layer , 0 , 0 , FALSE);
    UpdateLayer(WindowManager.MouseWindow->Layer , 0 , 0 , GRAPHICS_MOUSE_WIDTH , GRAPHICS_MOUSE_HEIGHT);
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
        MoveLayer(WindowManager.MouseWindow->Layer , X , Y);
    }
}