#include <WindowSystem.h>
#include <MemoryManagement.h>
#include <Hal.h>
#include <Task.h>

static Window::Manager WindowManager;

static WINDOW *CreateSystemWindow(const char *Title , int X , int Y , int Width , int Height , int Priority , BOOL Move) {
	int i;
	WINDOW *Window = (WINDOW*)Memory::malloc(sizeof(WINDOW));
	Window->Buffer = (WORD*)Memory::malloc(Width*Height*sizeof(WORD));
	Window->Layer = CreateLayer(Window->Buffer , FALSE , Width , Height , GRAPHICS_MOUSE_INVISIBLECOLOR);
	ChangeLayerHeight(Window->Layer , Priority);
	if(Move == TRUE) {
		MoveLayer(Window->Layer , X , Y);
	}
	return Window;
}

void Window::Initialize(void) {
    int i;
    VBEMODEINFOBLOCK *Block = (VBEMODEINFOBLOCK*)VBEMODEINFOBLOCK_STARTADDRESS;
    WindowManager.WindowsCount = 1;
    InitLayerSystem((WORD*)Block->Address , Block->Width , Block->Height);
    WindowManager.BackgroundWindow = CreateSystemWindow("Background window" , 0 , 0 , Block->Width , Block->Height , 0x00 , FALSE);
    Graphics::DrawRectangle(WindowManager.BackgroundWindow->Layer , 0 , 0 , Block->Width , Block->Height , WINDOW_DEFAULTWALLPAPERCOLOR , TRUE);
    Task::CreateTask((QWORD)MouseWindow , TASK_SYSTEM , "MouseTask" , "Task for Mouse system(GUI)");
    Task::CreateTask((QWORD)EventWindow , TASK_SYSTEM , "EventTask" , "GUI event processor");
    delay(50);
}

WINDOW *Window::CreateWindow(const char *Title , WORD Flags , int X , int Y , int Width , int Height , WORD BackgroundColor) {
	int i;
	WINDOW *Window = (WINDOW*)Memory::malloc(sizeof(WINDOW));
	Window->Buffer = (WORD*)Memory::malloc(Width*Height*sizeof(WORD));
	Window->Flags = Flags;
	Window->Layer = CreateLayer(Window->Buffer , FALSE , Width , Height , WINDOW_INVISIBLECOLOR);
	if(Flags == WINDOW_FLAGS_DEFAULT) {
		Graphics::DrawWindow(Window->Layer , 0 , 0 , Width , Height , Title , BackgroundColor , FALSE);
	}
	if((Flags & WINDOW_FLAGS_NO_BACKGROUND) == WINDOW_FLAGS_NO_BACKGROUND) {
		MoveLayer(Window->Layer , X , Y);
		ChangeLayerHeight(Window->Layer , WindowManager.WindowsCount);
		UpdateLayer(Window->Layer , 0 , 0 , Width , Height);
		WindowManager.WindowsCount++;
		return Window;
	}
	if((Flags & WINDOW_FLAGS_NO_TITLEBAR) == WINDOW_FLAGS_NO_TITLEBAR) {
		Graphics::DrawRectangle(Window->Layer , 0 , 0 , Width , Height , BackgroundColor , FALSE);
	}
	MoveLayer(Window->Layer , X , Y);
	ChangeLayerHeight(Window->Layer , WindowManager.WindowsCount);
	UpdateLayer(Window->Layer , 0 , 0 , Width , Height);
	WindowManager.WindowsCount++;
	return Window;
}

void Window::EventWindow(void) {
	while(1) {
		;
	}
}

WINDOW *GetWindowByCoord(int X , int Y) {
	int i;
	int Priority = 0;
	int X1;
	int Y1;
	int X2;
	int Y2;
	WINDOW *Window = NULL;
	for(i = WINDOW_MAXCOUNT-1; i > 0; i--) {
		X1 = WindowManager.Windows[i].Layer->VX1;
		Y1 = WindowManager.Windows[i].Layer->VY1; 
		X2 = X1+WindowManager.Windows[i].Layer->BXSize;
		Y2 = Y1+WindowManager.Windows[i].Layer->BYSize;
		if((X > X1)||(Y > Y2)||(X < X2)||(Y < Y2)) {
			Window = &(WindowManager.Windows[i]);
			break;
		}
	}
	return Window;
}

void Window::MouseWindow(void) {
	int i;
	int X;
	int Y;
	int Button;
	int DX;
	int DY;
	int WindowX;
	int WindowY;
	BOOL WindowMoving = FALSE;
    WINDOW *Window;
    WINDOW *TempBuffer;
    VBEMODEINFOBLOCK *Block = (VBEMODEINFOBLOCK*)VBEMODEINFOBLOCK_STARTADDRESS;
    LAYERMANAGER *LayerManager;
    LAYER *Layer;
    X = (Block->Width-GRAPHICS_MOUSE_WIDTH)/2;
    Y = (Block->Height-GRAPHICS_MOUSE_HEIGHT)/2;
    WindowManager.MouseWindow = CreateSystemWindow("Mouse window" , (Block->Width-GRAPHICS_MOUSE_WIDTH)/2 , (Block->Height-GRAPHICS_MOUSE_HEIGHT)/2 , GRAPHICS_MOUSE_WIDTH , GRAPHICS_MOUSE_HEIGHT , 5 , TRUE);
	Graphics::DrawCursor(WindowManager.MouseWindow->Layer , 0 , 0 , TRUE);
    MoveLayer(WindowManager.MouseWindow->Layer , X , Y);
    while(1) {
        if(Hal::Mouse::GetMouseData(&(DX) , &(DY) , &(Button)) == FALSE) {
        	continue;
        }
        if(Button == 1) {
        	if(WindowMoving == FALSE) {
	        	LayerManager = GetLayerManager();
	        	for(i = LayerManager->Top-1; i > 0; i--) {
	        		Layer = LayerManager->Layers[i];
	        		if(((X-Layer->VX1) >= 0) && ((X-Layer->VX1) < Layer->BXSize) && ((Y-Layer->VY1) >= 0) && ((Y-Layer->VY1) < Layer->BYSize)) {
	        			ChangeLayerHeight(Layer , LayerManager->Top-1);
	        			WindowMoving = TRUE;
	        			break;
	        		}
	        	}
	        }
	        else {
	        	MoveLayer(Layer , Layer->VX1+DX , Layer->VY1+DY);
	        }
        }
        else {
        	WindowMoving = FALSE;
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

void Window::DrawPixel(WINDOW *Window , int X , int Y , WORD Color) {
	if((Window->Flags & WINDOW_FLAGS_NO_TITLEBAR) == WINDOW_FLAGS_NO_TITLEBAR) {
		Graphics::DrawPixel(Window->Layer , X , Y , Color , TRUE);
	}
	else {
		Graphics::DrawPixel(Window->Layer , X+2 , Y+GRAPHICS_WINDOW_TITLEBARSIZE+4 , Color , TRUE);
	}
}

void Window::DrawRectangle(WINDOW *Window , int X1 , int Y1 , int X2 , int Y2 , WORD Color) {
	if((Window->Flags & WINDOW_FLAGS_NO_TITLEBAR) == WINDOW_FLAGS_NO_TITLEBAR) {
		Graphics::DrawRectangle(Window->Layer , X1 , Y1 , X2 , Y2 , Color , TRUE);
	}
	else {
		Graphics::DrawRectangle(Window->Layer , X1+2 , Y1+GRAPHICS_WINDOW_TITLEBARSIZE+4 , X2+2 , Y2+GRAPHICS_WINDOW_TITLEBARSIZE+4 , Color , TRUE);
	}
}

void Window::DrawText(WINDOW *Window , const char *Text , int X , int Y , WORD Color) {
	if((Window->Flags & WINDOW_FLAGS_NO_TITLEBAR) == WINDOW_FLAGS_NO_TITLEBAR) {
		Graphics::DrawText(Window->Layer , Text , X , Y , Color , TRUE);
	}
	else {
		Graphics::DrawText(Window->Layer , Text , X+2 , Y+GRAPHICS_WINDOW_TITLEBARSIZE+4 , Color , TRUE);
	}
}

void Window::DrawLine(WINDOW *Window , int X1 , int Y1 , int X2 , int Y2 , WORD Color) {
	if((Window->Flags & WINDOW_FLAGS_NO_TITLEBAR) == WINDOW_FLAGS_NO_TITLEBAR) {
		Graphics::DrawLine(Window->Layer , X1 , Y1 , X2 , Y2 , Color , TRUE);
	}
	else {
		Graphics::DrawLine(Window->Layer , X1+2 , Y1+GRAPHICS_WINDOW_TITLEBARSIZE+4 , X2+2 , Y2+GRAPHICS_WINDOW_TITLEBARSIZE+4 , Color , TRUE);
	}
}