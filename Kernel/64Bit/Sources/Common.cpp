#include <Common.h>
#include <string.h>
#include <TextScreen.h>
#include <Hal.h>

void Common::Queue::Initialize(int MaxSize) {
    this->MaxSize = MaxSize;
    this->Rear = 0;
    this->Front = 0;
}

BOOL Common::Queue::CheckEmpty(void) {
	if(Rear == Front) {
		return TRUE;
	}
	return FALSE;
}

BOOL Common::Queue::CheckFull(void) {
	if((Rear+1)%MaxSize == Front) {
		return TRUE;
	}
	return FALSE;
}

BOOL Common::Queue::Enqueue(BYTE Data) {
	if(this->CheckFull() == TRUE) {
		return FALSE;
	}
	Rear = (Rear+1)%MaxSize;
	Buffer[Rear] = Data;
	return TRUE;
}

BYTE Common::Queue::Dequeue(void) {
	if(this->CheckEmpty() == TRUE) {
		return 0;
	}
	Front = (Front+1)%MaxSize;
	return Buffer[Front];
}

void Common::Arguments::Initialize(char *String) {
    this->Length = strlen(String);
    this->CurrentPosition = 0;
    this->String = String;
}

int Common::Arguments::GetArg(char *Argument) {
    int i;
    int Length;
    if(this->Length <= this->CurrentPosition) {
        return 0;
    }
    for(i = this->CurrentPosition; i < this->Length; i++) {
        if(this->String[i] == ' ') {
            break;
        }
    }
    memcpy(Argument , this->String+this->CurrentPosition , i);
    Length = i-this->CurrentPosition;
    Argument[Length] = '\0';
    this->CurrentPosition += Length+1;
    return Length;
}

int Common::gets(char *Buffer) {
    int i;
    int Index = 0;
    char AnotherBuffer;
    memset(Buffer , 0 , sizeof(Buffer));
    while(1) {
        AnotherBuffer = getch();
        switch(AnotherBuffer) {
            case '\n':
                Buffer[Index] = 0x00;
                TextScreen::printf("\n");
                return Index;
            case '\t':
                for(i = 0; i < 5; i++) {
                    Buffer[Index+i] = ' ';
                }
                Index += i;
                break;
            case '\b':
                if(Index != 0) {
                    Index--;
                    Buffer[Index] = 0x00;
                    TextScreen::printf("\b");
                }
                break;
            default:
                Buffer[Index] = AnotherBuffer;
                TextScreen::printf("%c" , AnotherBuffer);
                Index++;
                break;
        }
    }
}

static QWORD RandomSeed = 0x31415926;

void Common::srand(QWORD Seed) {
    RandomSeed = Seed;
}

QWORD Common::rand(void) {
    RandomSeed = (RandomSeed*412153+5571031) >> 16;
    return RandomSeed;
}