#ifndef _HAL_H_
#define _HAL_H_

#include <Types.h>
#include <Common.h>

#define KEYBOARD_ARROW_UP    0
#define KEYBOARD_ARROW_DOWN  0
#define KEYBOARD_ARROW_RIGHT 0
#define KEYBOARD_ARROW_LEFT  0

#define TIMER_CYCLE (25*1193180)/1000000

namespace Hal {
    extern "C" void WritePort(WORD Port , BYTE Data);
    extern "C" BYTE ReadPort(WORD Port);
    void InitSystem(void);
    void EnableA20(void);
    namespace Keyboard {
        typedef struct {
            Common::Queue ASCII;
            BOOL Shift;
            BOOL Capslock;
            BOOL Numlock;
            BOOL Scrolllock;
        }CONTROLLER;
        void Initialize(void);
        void ProcessAndPutToQueue(BYTE KeyCode);
    };
    namespace Mouse {
        typedef struct {
            int X;
            int Y;
            int Button;
            BOOL Status;
            int Phase;
            BYTE Buffer[3];
            Common::Queue Queue;
        }CONTROLLER;
        void Initialize(void);
        void ProcessAndPutToQueue(BYTE KeyCode);
        BOOL GetMouseData(int *X , int *Y , int *Button);
    };
    namespace Timer {
        typedef struct {
            BOOL Initialized;
            QWORD TickCount;
        }CONTROLLER;
        void Initialize(void);
        void Reinitialize(WORD Count);
        QWORD GetTickCount();
        void Interrupt(void);
    }
};

BYTE getch(void);
void delay(QWORD Millisecond);

#endif