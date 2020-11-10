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

void MouseTask(void);
void WindowTask(void);

extern "C" void Main(void) {
	int i;
	DWORD Color;
    VBEMODEINFOBLOCK *Block = (VBEMODEINFOBLOCK*)VBEMODEINFOBLOCK_STARTADDRESS;
    WORD *Address = (WORD*)Block->Address;
    Hal::InitSystem();
    while(1) {
        Common::srand(Hal::Timer::GetTickCount());
    	if(getch()) {
            Window::CreateWindow("Hello world" , "Test window" , (QWORD)WindowTask , WINDOW_TITLEBAR , Common::rand()%Block->Width , Common::rand()%Block->Height , 300 , 150);
        }
    }
}

void WindowTask(void) {
    while(1) {
        ;
    }
}