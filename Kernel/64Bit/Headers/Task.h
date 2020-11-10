#ifndef _MULTITASKING_H_
#define _MULTITASKING_H_

#include <Types.h>

#define TASK_DEFAULT 0x00
#define TASK_SYSTEM 0x01
#define TASK_NONE 0xFF

#define TASK_MAINTASKID 0x8000000
#define TASK_IDTOOFFSET(ID) (ID-TASK_MAINTASKID)

#define TASK_STACKSIZE 8192
#define TASK_MAXCOUNT 1024

#pragma pack(push , 1)

namespace Task {

    typedef struct {
        QWORD GS;
        QWORD FS;
        QWORD ES;
        QWORD DS;
        QWORD R15;
        QWORD R14;
        QWORD R13;
        QWORD R12;
        QWORD R11;
        QWORD R10;
        QWORD R9;
        QWORD R8;
        QWORD RSI;
        QWORD RDI;
        QWORD RDX;
        QWORD RCX;
        QWORD RBX;
        QWORD RAX;
        QWORD RBP;
        QWORD RIP;
        QWORD CS;
        QWORD RFlags;
        QWORD RSP;
        QWORD SS;
    }REGISTERS;

    struct TASKINFO {
        QWORD ID;
        QWORD Flags;
        BOOL Using;

        BYTE *Stack;
        REGISTERS Registers;

        char Name[512];
        char Description[512];
    };

    class Manager {
        public:
            QWORD CurrentTaskID;
            QWORD NextTaskID;
            
            QWORD CurrentTaskIndex;
            QWORD NextTaskIndex;

            QWORD RunningTaskCount;
            QWORD TaskCount;
            TASKINFO *Tasks;
            QWORD Timer;
    };
    extern "C" void SwitchRegisters(REGISTERS *Current , REGISTERS *Next);

    void Initialize(void);
    QWORD CreateTask(QWORD EntryPoint , QWORD Flags , const char *Name , const char *Description);
    BOOL EndTask(QWORD ID);
    void Exit(void);
    void SetupTaskRegisters(REGISTERS *Registers , BYTE *Stack , QWORD EntryPoint);
    TASKINFO GetTaskInfo(QWORD ID);
    QWORD GetCurrentTaskID(void);
    QWORD GetRunningTaskCount(void);
    Task::TASKINFO *GetTaskList(void);
    DWORD GetProcessorUsing(void);
    void SwitchTask(void);
    QWORD ChangeIDToOffset(QWORD ID);

    void System(void);
    void IDLE(void);
}

#pragma pack(pop)

#endif