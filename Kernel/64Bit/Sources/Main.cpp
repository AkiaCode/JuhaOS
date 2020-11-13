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

cmtSbWFUbHIgd25ycmh0bHZybGVoIGdrcms=
ZGphYWtyayBhb3Nza2YgcmhkcW5ha3MgZ2tmayBnbw==
ZGxzdG9kZGwgZGxmamdycCBSbWFXbHJna3NyanN3bmYgYWhmZmtUZWs=
cmtSbWFUbHIgd25ycmh0bHZybGVoIGdrcms=
U0dWc2NDQnRaU3dnY0d4bFlYTmxMaTR1SURBeE1DMDBNakF6TFRNeE5ERXNJRVJwYzJOdmNtUTZTblZvWVNNMU5ETTM=
d25na2RtbCBkbHN0b2RkbXMgcm9ya3hlaw==
MGE2Njc1NjM2YjVmNmQ3OTVmNmM2OTY2NjU1ZjczNjM3MjY1Nzc1ZjY5NzQ1ZjY5NWY3NzYxNmU2ZTYxNWY3Mzc1Njk2MzY5NjQ2NTVmNzI2OTY3Njg3NDVmNjY3NTYzNmI2OTZlNjc1ZjZlNmY3NzVmM2EyOQ==
*/

void WindowTask(void);

extern "C" void Main(void) {
    Hal::InitSystem();
    Task::CreateTask((QWORD)WindowTask , TASK_DEFAULT , "WindowTask1" , "");
    while(1) {
    	;
    }
}

void WindowTask(void) {
    WINDOW *Window;
    Window = Window::CreateWindow("Hello world" , WINDOW_FLAGS_DEFAULT , 100 , 100 , 400 , 250 , RGB(255 , 255 , 255));
    while(1) {
    	;
    }
}