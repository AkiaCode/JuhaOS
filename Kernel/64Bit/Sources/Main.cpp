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
    for(i = 0; i < 10; i++) {
    	Window::CreateWindow("Hello world1" , WINDOW_FLAGS_DEFAULT , 100 , 100 , 200 , 100 , RGB(255 , 255 , 255));
    }

    while(1) {
    	;
    }
}

void WindowTask1(void) {
	WINDOW *Window;
    Window = Window::CreateWindow("Hello world1" , WINDOW_FLAGS_DEFAULT , 100 , 100 , 200 , 100 , RGB(255 , 255 , 255));
    while(1) {
    	;
    }
}

void WindowTask2(void) {
	WINDOW *Window;
    Window = Window::CreateWindow("Hello world2" , WINDOW_FLAGS_DEFAULT , 150 , 150 , 200 , 100 , RGB(255 , 255 , 255));
    while(1) {
    	;
    }
}

void WindowTask3(void) {
	WINDOW *Window;
    Window = Window::CreateWindow("Hello world3" , WINDOW_FLAGS_DEFAULT , 200 , 200 , 200 , 100 , RGB(255 , 255 , 255));
    while(1) {
    	;
    }
}

void WindowTask4(void) {
	WINDOW *Window;
    Window = Window::CreateWindow("Hello world4" , WINDOW_FLAGS_DEFAULT , 250 , 250 , 200 , 100 , RGB(255 , 255 , 255));
    while(1) {
    	;
    }
}

void WindowTask5(void) {
	WINDOW *Window;
    Window = Window::CreateWindow("Hello world5" , WINDOW_FLAGS_DEFAULT , 300 , 300 , 200 , 100 , RGB(255 , 255 , 255));
    while(1) {
    	;
    }
}