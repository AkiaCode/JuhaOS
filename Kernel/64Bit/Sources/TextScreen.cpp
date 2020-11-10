#include <TextScreen.h>
#include <string.h>
#include <Hal.h>

static TextScreen::TextScreenController ScreenController;

void TextScreen::EnableCursor(BYTE Start , BYTE End) {
    Hal::WritePort(0x3D4 , 0x0A);
    Hal::WritePort(0x3D5 , (Hal::ReadPort(0x3D5) & 0xC0)|Start);
    Hal::WritePort(0x3D4 , 0x0B);
    Hal::WritePort(0x3D5 , (Hal::ReadPort(0x3D5) & 0xE0)|End);
}

void TextScreen::ClearScreen(char Color) {
	int i;
	BYTE *Buffer = (BYTE*)TEXTSCREEN_BUFFER;
	ScreenController.Color = Color;
	for(i = 0; i < TEXTSCREEN_WIDTH*TEXTSCREEN_HEIGHT; i++) {
        *(Buffer++) = 0x00;
        *(Buffer++) = Color;
	}
	MoveCursor(0 , 0);
	return;
}

void TextScreen::GetCursor(int *X , int *Y) {
    *X = ScreenController.X;
    *Y = ScreenController.Y;
}

void TextScreen::MoveCursor(int X , int Y) {
    int Offset = (Y*TEXTSCREEN_WIDTH)+X;
    Hal::WritePort(0x3D4 , 0x0E);
    Hal::WritePort(0x3D5 , Offset >> 8);
    Hal::WritePort(0x3D4 , 0x0F);
    Hal::WritePort(0x3D5 , Offset & 0xFF);
    ScreenController.X = X;
    ScreenController.Y = Y;
}

void TextScreen::MoveCursorWithoutSaving(int X , int Y) {
    int Offset = (Y*TEXTSCREEN_WIDTH)+X;
    Hal::WritePort(0x3D4 , 0x0E);
    Hal::WritePort(0x3D5 , Offset >> 8);
    Hal::WritePort(0x3D4 , 0x0F);
    Hal::WritePort(0x3D5 , Offset & 0xFF);
}

void TextScreen::PrintCharacter(const BYTE Character) {
	int i;
    int Offset = ((ScreenController.Y*TEXTSCREEN_WIDTH)+ScreenController.X)*2;
	BYTE *Buffer = (BYTE*)TEXTSCREEN_BUFFER+Offset;
    switch(Character) {
        case '\n':
            ScreenController.X = 0;
            ScreenController.Y += 1;
            break;
        case '\r':
            ScreenController.X = 0;
            break;
        case '\b':
			if(ScreenController.X > 0) {
				ScreenController.X -= 1;
                Buffer = (BYTE*)TEXTSCREEN_BUFFER;
				Buffer[(((ScreenController.Y*TEXTSCREEN_WIDTH)+(ScreenController.X))*2)] = 0x00;
				Buffer[(((ScreenController.Y*TEXTSCREEN_WIDTH)+(ScreenController.X))*2)+1] = ScreenController.Color;
                Buffer += ((ScreenController.Y*TEXTSCREEN_WIDTH)+ScreenController.X)*2;
			}
            break;
        case '\t':
            for(i = 0; i < 5; i++) {
                *Buffer++ = ' ';
                ScreenController.X++;
            }
            break;
        default:
            *Buffer++ = Character;
            *Buffer++ = ScreenController.Color;
            ScreenController.X++;
            break;
    }
	if(ScreenController.X > TEXTSCREEN_WIDTH) {
		ScreenController.X = 0;
		ScreenController.Y += 1;
	}
    Buffer = (BYTE*)TEXTSCREEN_BUFFER;
	if(ScreenController.Y >= TEXTSCREEN_HEIGHT) {
		for(i = 0; i < TEXTSCREEN_WIDTH*(TEXTSCREEN_HEIGHT-1)*2; i++) {
			Buffer[i] = Buffer[i+(80*2)];
		}
		for(i = TEXTSCREEN_WIDTH*(TEXTSCREEN_HEIGHT-1)*2; i < TEXTSCREEN_WIDTH*(TEXTSCREEN_HEIGHT)*2;) {
			Buffer[i++] = ' ';
			Buffer[i++] = ScreenController.Color;
		}
		ScreenController.X = 0;
		ScreenController.Y = 24;
	}
    MoveCursorWithoutSaving(ScreenController.X , ScreenController.Y);
}

void TextScreen::PrintString(const char *Buffer) {
	int i;
	for(i = 0; Buffer[i] != '\0'; i++) {
		PrintCharacter(Buffer[i]);
	}
}

void TextScreen::printf(const char *Format , ...) {
	va_list ap;
	char Buffer[0x4000];
	va_start(ap , Format);
	vsprintf(Buffer , Format , ap);
	PrintString(Buffer);
	va_end(ap);
}

void TextScreen::SetColor(char Color) {
    ScreenController.Color = Color;
}

char TextScreen::GetColor(void) {
    return ScreenController.Color;
}