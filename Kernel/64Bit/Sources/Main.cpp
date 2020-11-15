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

extern "C" void Main(void) {
	WINDOW *Window1;
	WINDOW *Window2;
	WINDOW *Window3;
	WINDOW *Window4;
	WINDOW *Window5;
    Hal::InitSystem();
    Window1 = Window::CreateWindow("Window #1" , WINDOW_FLAGS_DEFAULT , 100 , 100 , 200 , 100 , RGB(255 , 255 , 255));
    Window2 = Window::CreateWindow("Window #2" , WINDOW_FLAGS_DEFAULT , 150 , 150 , 200 , 100 , RGB(255 , 255 , 255));
    Window3 = Window::CreateWindow("Window #3" , WINDOW_FLAGS_DEFAULT , 200 , 200 , 200 , 100 , RGB(255 , 255 , 255));
    Window4 = Window::CreateWindow("Window #4" , WINDOW_FLAGS_DEFAULT , 250 , 250 , 200 , 100 , RGB(255 , 255 , 255));
    Window5 = Window::CreateWindow("Window #5" , WINDOW_FLAGS_DEFAULT , 300 , 300 , 200 , 100 , RGB(255 , 255 , 255));
    while(1) {
    	;
    }
}