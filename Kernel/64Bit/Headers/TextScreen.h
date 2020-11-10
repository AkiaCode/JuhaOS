#ifndef _TEXTSCREEN_H_
#define _TEXTSCREEN_H_

#include <Types.h>
#include <stdarg.h>

#pragma pack(push , 1)

namespace TextScreen {

    #define TEXTSCREEN_BUFFER 0xB8000
    #define TEXTSCREEN_WIDTH 80
    #define TEXTSCREEN_HEIGHT 25

    typedef struct {
        int X;
        int Y;
        char Color;
    }TextScreenController;

    void EnableCursor(BYTE Start , BYTE End);
    void ClearScreen(char Color);
    void SetColor(char Color);
    char GetColor(void);
    void MoveCursor(int X , int Y);
    void GetCursor(int *X , int *Y);
    void MoveCursorWithoutSaving(int X , int Y);
    void PrintCharacter(const BYTE Character);
    void PrintString(const char *Buffer);
    void printf(const char *Format , ...);
};

#pragma pack(pop)

#endif