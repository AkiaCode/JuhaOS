#include <Hal.h>
#include <TextScreen.h>
#include <DescriptorTables.h>
#include <MemoryManagement.h>
#include <Task.h>
#include <FAT32.h>
#include <WindowSystem.h>

BOOL CheckInputBuffer(void);
BOOL CheckOutputBuffer(void);

static Hal::Keyboard::CONTROLLER KeyboardController;
static Hal::Mouse::CONTROLLER MouseController;
static Hal::Timer::CONTROLLER TimerController;

void Hal::InitSystem(void) {
    Hal::EnableA20();
    TextScreen::EnableCursor(0x00 , 0xFF);
    TextScreen::ClearScreen(0x07);
    TextScreen::printf("Loading : ");
    DescriptorTables::Initialize();
    Hal::Keyboard::Initialize();
    Hal::Mouse::Initialize();
    Memory::Initialize();
    Hal::Timer::Initialize();
    Task::Initialize();
    RAMDisk::Initialize(RAMDISK_MAXSIZE);
    if(FAT32::Initialize() == FALSE) {
        __asm__ ("cli");
        while(1) {
            __asm__ ("hlt");
        }
    }
    Window::Initialize();
    TextScreen::printf("Done\n");
}

BOOL CheckOutputBuffer(void) {
    if(Hal::ReadPort(0x64) & 0x01) {
        return FALSE;
    }
    return TRUE;
}

BOOL CheckInputBuffer(void) {
    if(Hal::ReadPort(0x64) & 0x02) {
        return FALSE;
    }
    return TRUE;
}

void Hal::EnableA20(void) {
    int i;
    BYTE Data;
    Hal::WritePort(0x64 , 0xD0);
    for(i = 0; i < 0xFFFF; i++) {
        if(CheckOutputBuffer() == FALSE) {
            break;
        }
    }
    Data = Hal::ReadPort(0x60);
    Data |= 0x01;
    for(i = 0; i < 0xFFFF; i++) {
        if(CheckInputBuffer() == TRUE) {
            break;
        }
    }
    WritePort(0x64 , 0xD1);
    WritePort(0x60 , Data);
}

void Hal::Keyboard::Initialize(void) {
    KeyboardController.Capslock = FALSE;
    KeyboardController.Shift = FALSE;
    KeyboardController.ASCII.Initialize(1024);
}

void Hal::Mouse::Initialize(void) {
    int i;
    BYTE Data;
    __asm__ ("cli");
    MouseController.Queue.Initialize(1024);
    Hal::WritePort(0x64 , 0xA8);
    Hal::WritePort(0x64 , 0xD4);
    for(i = 0; i < 0xFFFF; i++) {
        if(!(Hal::ReadPort(0x64) & 0x02)) {
            break;
        }
    }
    Hal::WritePort(0x60 , 0xF4);
    Hal::WritePort(0x64 , 0x20);
    for(i = 0; i < 0xFFFF; i++) {
        if(Hal::ReadPort(0x64) & 0x01) {
            break;
        }
    }
    Data = Hal::ReadPort(0x60);
    Data |= 0x02;
    Hal::WritePort(0x64 , 0x60);
    for(i = 0; i < 0xFFFF; i++) {
        if(!(Hal::ReadPort(0x64) & 0x02)) {
            break;
        }
    }
    Hal::WritePort(0x60 , Data);
    __asm__ ("sti");
    MouseController.Phase = 0;
}

void Hal::Timer::Initialize(void) {
    TimerController.TickCount = 0;
    Hal::WritePort(0x43 , 0x30);
    Hal::WritePort(0x43 , 0x34);
    Hal::WritePort(0x40 , TIMER_CYCLE);
    Hal::WritePort(0x40 , TIMER_CYCLE >> 0x08);
}


void Hal::Timer::Reinitialize(WORD Count) {
    Hal::WritePort(0x43 , 0x30);
    Hal::WritePort(0x43 , 0x34);
    Hal::WritePort(0x40 , Count);
    Hal::WritePort(0x40 , Count >> 0x08);
}

QWORD Hal::Timer::GetTickCount(void) {
    return TimerController.TickCount;
}

void Hal::Timer::Interrupt(void) {
    TimerController.TickCount++;
}

void delay(QWORD Millisecond) {
    int i;
    QWORD LastTick = Hal::Timer::GetTickCount();
    while(1) {
        if(Hal::Timer::GetTickCount()-LastTick >= Millisecond*25) {
            break;
        }
    }
}

void Hal::Mouse::ProcessAndPutToQueue(BYTE Code) {
    DWORD X;
    DWORD Y;
    DWORD Button;
    MouseController.Queue.Enqueue(Code);
}

BOOL Hal::Mouse::GetMouseData(int *X , int *Y , int *Button) {
    BYTE Code = MouseController.Queue.Dequeue();
    if(MouseController.Phase == 0) {
        MouseController.Phase = 1;
        return FALSE; 
    }
    if(MouseController.Phase == 1) {
        if((Code & 0xC8) == 0x08) {
            MouseController.Buffer[0] = Code;
            MouseController.Phase = 2;
        }
        return FALSE; 
    }
    if(MouseController.Phase == 2) {
        MouseController.Buffer[1] = Code;
        MouseController.Phase = 3;
        return FALSE; 
    }
    if(MouseController.Phase == 3) {
        MouseController.Buffer[2] = Code;
        MouseController.Phase = 1;
        *Button = MouseController.Buffer[0] & 0x07;
        *X = MouseController.Buffer[1];
        *Y = MouseController.Buffer[2];
        if((MouseController.Buffer[0] & 0x10) != 0x00) {
            *X |= 0xFFFFFF00;
        }
        if((MouseController.Buffer[0] & 0x20) != 0x00) {
            *Y |= 0xFFFFFF00;
        }
        *Y = -(*Y);
        return TRUE; 
    }
    return FALSE;
}

void Hal::Keyboard::ProcessAndPutToQueue(BYTE KeyCode) {
    BYTE KeyCodeLow;
	static const BYTE Default[0xFF] = {
		0 , 0 , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' , '-' , '=' , '\b' , 0 , 
		'q' , 'w' , 'e' , 'r' , 't' , 'y' , 'u' , 'i' , 'o' , 'p' , '[' , ']' , '\n' , 0 , 'a' , 's' , 
		'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l', ';', '\'', '`',   0,   '\\', 'z', 'x', 'c', 'v',
		'b' , 'n' , 'm' , ',' , '.' , '/' , 0 , '*' , 0 , ' ' , 0 , 0, 0 , 0 , 0 , 0 , 
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , KEYBOARD_ARROW_UP , 0 , 0 , KEYBOARD_ARROW_LEFT , 0 , KEYBOARD_ARROW_RIGHT , 0 , 
		0 , KEYBOARD_ARROW_DOWN , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  , 
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	};
	static const BYTE Shift[0xFF] = {
		0 , 0 , '!' , '@' , '#' , '$' , '%' , '^' , '&' , '*' , '(' , ')' , '_' , '+' , '\b' , 0 , 
		'Q' , 'W' , 'E' , 'R' , 'T' , 'Y' , 'U' , 'I' , 'O' , 'P' , '{' , '}' , '\n' , 0 , 'A' , 'S' , 
		'D' , 'F' , 'G' , 'H' , 'J' , 'K' , 'L' , ':' , '"' , '~' ,   0,   '|', 'Z', 'X' , 'C' , 'V' , 
		'B' , 'N' , 'M' , '<' , '>' , '?' , 0 , '*' , 0 , ' ' , 0 , 0, 0 , 0 , 0 , 0 , 
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , KEYBOARD_ARROW_UP , 0 , 0 , KEYBOARD_ARROW_LEFT , 0 , KEYBOARD_ARROW_RIGHT , 0 , 
		0 , KEYBOARD_ARROW_DOWN , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  , 
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
	};
	static const BYTE Capslock[0xFF] = {
		0 , 0 , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' , '-' , '=' , '\b' , 0 , 
		'Q' , 'W' , 'E' , 'R' , 'T' , 'Y' , 'U' , 'I' , 'O' , 'P' , '[' , ']' , '\n' , 0 , 'A' , 'S' , 
		'D' , 'F' , 'G' , 'H' , 'J' , 'K' , 'L', ';', '\'', '`',   0,   '\\', 'Z', 'X', 'C', 'V',
		'B' , 'N' , 'M' , ',' , '.' , '/' , 0 , '*' , 0 , ' ' , 0 , 0, 0 , 0 , 0 , 0 , 
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , KEYBOARD_ARROW_UP , 0 , 0 , KEYBOARD_ARROW_LEFT , 0 , KEYBOARD_ARROW_RIGHT , 0 , 
		0 , KEYBOARD_ARROW_DOWN , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  , 
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
	};
	static const BYTE CapslockShift[0xFF] = {
		0 , 0 , '!' , '@' , '#' , '$' , '%' , '^' , '&' , '*' , '(' , ')' , '_' , '+' , '\b' , 0 , 
		'q' , 'w' , 'e' , 'r' , 't' , 'y' , 'u' , 'i' , 'o' , 'p' , '{' , '}' , '\n' , 0 , 'a' , 's' , 
		'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l', ':', '"', '~',   0,   '|', 'z', 'x', 'c', 'v',
		'b' , 'n' , 'm' , '<' , '>' , '?' , 0 , '*' , 0 , ' ' , 0 , 0, 0 , 0 , 0 , 0 , 
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , KEYBOARD_ARROW_UP , 0 , 0 , KEYBOARD_ARROW_LEFT , 0 , KEYBOARD_ARROW_RIGHT , 0 , 
		0 , KEYBOARD_ARROW_DOWN , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  , 
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
	};
    if(KeyCode & 0x80) {
        KeyCodeLow = KeyCode & 0x7F;
        if((KeyCodeLow == 42)||(KeyCodeLow == 54)) {
            KeyboardController.Shift = FALSE;
        }
    }
    else {
        if((KeyCode == 42)||(KeyCode == 54)) {
            KeyboardController.Shift = TRUE;
        }
        else if(KeyCode == 58) {
            if(KeyboardController.Capslock == TRUE) {
                KeyboardController.Capslock = FALSE;
            }
            else {
                KeyboardController.Capslock = TRUE;
            }
        }
        else if(KeyCode == 69) {
            if(KeyboardController.Numlock == TRUE) {
                KeyboardController.Numlock = FALSE;
            }
            else {
                KeyboardController.Numlock = TRUE;
            }
        }
        else if(KeyCode == 69) {
            if(KeyboardController.Scrolllock == TRUE) {
                KeyboardController.Scrolllock = FALSE;
            }
            else {
                KeyboardController.Scrolllock = TRUE;
            }
        }
        if(KeyboardController.Shift == TRUE) {
            if(KeyboardController.Capslock == TRUE) {
                KeyboardController.ASCII.Enqueue(CapslockShift[KeyCode]);
            }
            else {
                KeyboardController.ASCII.Enqueue(Shift[KeyCode]);
            }
        }
        else if(KeyboardController.Capslock == TRUE) {
            KeyboardController.ASCII.Enqueue(Capslock[KeyCode]);
        }
        else {
            KeyboardController.ASCII.Enqueue(Default[KeyCode]);
        }
    }
}

BYTE getch(void) {
	BYTE ASCIICode;
	while(1) {
		if(KeyboardController.ASCII.CheckEmpty() == FALSE) {
            ASCIICode = KeyboardController.ASCII.Dequeue();
			if(ASCIICode != 0) {
				break;
			}
		}
	}
	return ASCIICode;
}