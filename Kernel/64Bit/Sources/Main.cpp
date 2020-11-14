#include <Common.h>
#include <TextScreen.h>
#include <string.h>
#include <Hal.h>
#include <Shell.h>
#include <Graphics.h>
#include <WindowSystem.h>
/*
JuhaOS project, start:2020.08.24
(C) 2017-2020. all rights reserved by Juha.

*/

void WindowTask1(void);
void WindowTask2(void);
void WindowTask3(void);
void WindowTask4(void);
void WindowTask5(void);

extern "C" void Main(void) {
	int i;
	int j;
    Hal::InitSystem();
    Task::CreateTask((QWORD)WindowTask1 , TASK_DEFAULT , "WindowTask1" , "");
    Task::CreateTask((QWORD)WindowTask2 , TASK_DEFAULT , "WindowTask2" , "");
    Task::CreateTask((QWORD)WindowTask3 , TASK_DEFAULT , "WindowTask3" , "");
    Task::CreateTask((QWORD)WindowTask4 , TASK_DEFAULT , "WindowTask4" , "");
    Task::CreateTask((QWORD)WindowTask5 , TASK_DEFAULT , "WindowTask5" , "");
    	
    while(1) {
    	;
    }
}

void WindowTask1(void) {
	WINDOW *Window;
    Window = Window::CreateWindow("Hello world1" , WINDOW_FLAGS_DEFAULT , 100 , 100 , 400 , 250 , RGB(255 , 255 , 255));
    while(1) {
    	;
    }
}

void WindowTask2(void) {
	WINDOW *Window;
    Window = Window::CreateWindow("Hello world2" , WINDOW_FLAGS_DEFAULT , 200 , 200 , 400 , 250 , RGB(255 , 255 , 255));
    while(1) {
    	;
    }
}

void WindowTask3(void) {
	WINDOW *Window;
    Window = Window::CreateWindow("Hello world3" , WINDOW_FLAGS_DEFAULT , 300 , 300 , 400 , 250 , RGB(255 , 255 , 255));
    while(1) {
    	;
    }
}

void WindowTask4(void) {
	WINDOW *Window;
    Window = Window::CreateWindow("Hello world4" , WINDOW_FLAGS_DEFAULT , 400 , 400 , 400 , 250 , RGB(255 , 255 , 255));
    while(1) {
    	;
    }
}

void WindowTask5(void) {
	WINDOW *Window;
    Window = Window::CreateWindow("Hello world5" , WINDOW_FLAGS_DEFAULT , 500 , 500 , 400 , 250 , RGB(255 , 255 , 255));
    while(1) {
    	;
    }
}