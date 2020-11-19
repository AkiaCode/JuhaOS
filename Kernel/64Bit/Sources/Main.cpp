#include <Common.h>
#include <TextScreen.h>
#include <string.h>
#include <Hal.h>
#include <Shell.h>
#include <Graphics.h>
#include <WindowSystem.h>
/*
JuhaOS project, started in 2020.08.24
(C) 2017-2020. all rights reserved by Juha.
*/

void WindowTask1(void);
void WindowTask2(void);

extern "C" void Main(void) {
    Hal::InitSystem();
    Task::CreateTask((QWORD)WindowTask1 , TASK_DEFAULT , "WindowTask#1" , "");
    Task::CreateTask((QWORD)WindowTask2 , TASK_DEFAULT , "WindowTask#2" , "");
    Window::WindowSystem();
    while(1) {
    	;
    }
}

void WindowTask1(void) {
	WINDOW *Window;
	WINDOWEVENT *Event;
	BYTE String[2] = {0 , };
	int i = 0;
    Window = Window::CreateWindow("Window #1" , WINDOW_FLAGS_DEFAULT , 100 , 100 , 200 , 100 , RGB(255 , 255 , 255));
    while(1) {
    	__asm__ ("cli");
    	if(Window->EventQueue.GetEvent(Event) == FALSE) {
    		;
    	}
    	else if(Event->Type == WINDOW_EVENT_KEYBOARD) {
    		String[0] = Event->KeyboardData;
    		if(String[0] == '\n') {
    			Window::DrawRectangle(Window , 10 , 10 , 10+(i*FONT_CONSOLAS_WIDTH) , 10+FONT_CONSOLAS_HEIGHT , Window->BackgroundColor);
    			i = 0;
    		}
    		Window::DrawText(Window , String , 10+(i*FONT_CONSOLAS_WIDTH) , 10 , 0x00);
    		i++;
    	}
		__asm__ ("sti");
    }
}

void WindowTask2(void) {
	WINDOW *Window;
	WINDOWEVENT *Event;
	BYTE String[2] = {0 , };
	int i = 0;
    Window = Window::CreateWindow("Window #2" , WINDOW_FLAGS_DEFAULT , 150 , 150 , 200 , 100 , RGB(255 , 255 , 255));
    while(1) {
    	__asm__ ("cli");
    	if(Window->EventQueue.GetEvent(Event) == FALSE) {
    		;
    	}
    	else if(Event->Type == WINDOW_EVENT_KEYBOARD) {
    		String[0] = Event->KeyboardData;
    		if(String[0] == '\n') {
    			Window::DrawRectangle(Window , 10 , 10 , 10+(i*FONT_CONSOLAS_WIDTH) , 10+FONT_CONSOLAS_HEIGHT , Window->BackgroundColor);
    			i = 0;
    		}
    		Window::DrawText(Window , String , 10+(i*FONT_CONSOLAS_WIDTH) , 10 , 0x00);
    		i++;
    	}
		__asm__ ("sti");
    }
}