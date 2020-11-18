#include <Task.h>
#include <string.h>
#include <TextScreen.h>
#include <MemoryManagement.h>
#include <DescriptorTables.h>
#include <Hal.h>

static Task::Manager TaskManager;

void Task::Initialize(void) {
    int i;
    __asm__ ("cli");
    TaskManager.Tasks = (TASKINFO*)Memory::malloc(TASK_MAXCOUNT*sizeof(TASKINFO));
    for(i = 0; i < TASK_MAXCOUNT; i++) {
        TaskManager.Tasks[i].ID = TASK_MAINTASKID+i;
        TaskManager.Tasks[i].Flags = TASK_NONE;
        TaskManager.Tasks[i].Using = FALSE;
    }
    TaskManager.TaskCount = 1;
    TaskManager.CurrentTaskIndex = 0x00;
    TaskManager.NextTaskIndex = 0x01;

    TaskManager.Tasks[0].ID = TASK_MAINTASKID;
    TaskManager.Tasks[0].Flags = TASK_SYSTEM;
    TaskManager.Tasks[0].Using = TRUE;
    TaskManager.Tasks[0].Stack = 0x00;
    strcpy(TaskManager.Tasks[0].Name , "MAIN");
    strcpy(TaskManager.Tasks[0].Description , "This is kernel system, DO NOT TOUCH IT");
    SetupTaskRegisters(&(TaskManager.Tasks[0].Registers) , 0x00 , 0x00);
    __asm__ ("sti");
    CreateTask((QWORD)IDLE , TASK_SYSTEM , "SYSTEM" , "A task for control/manage the system");
    CreateTask((QWORD)System , TASK_SYSTEM , "IDLE" , "IDLE task");
}

QWORD Task::CreateTask(QWORD EntryPoint , QWORD Flags , const char *Name , const char *Description) {
    int i;
    QWORD ID;
    __asm__ ("cli");
    if(TaskManager.TaskCount >= TASK_MAXCOUNT-1) {
        __asm__ ("sti");
        return 0x00;
    }
    for(i = 0; i < TASK_MAXCOUNT; i++) {
        if(TaskManager.Tasks[i].Using == FALSE) {
            TaskManager.Tasks[i].Using = TRUE;
            break;
        }
    }
    ID = TaskManager.Tasks[i].ID;
    strcpy(TaskManager.Tasks[i].Name , Name);
    strcpy(TaskManager.Tasks[i].Description , Description);
    TaskManager.Tasks[i].Flags = Flags;
    TaskManager.Tasks[i].Stack = (BYTE*)Memory::malloc(TASK_STACKSIZE);
    SetupTaskRegisters(&(TaskManager.Tasks[i].Registers) , TaskManager.Tasks[i].Stack , EntryPoint);

    __asm__ ("sti");
    delay(50);
    TaskManager.TaskCount++;
    return ID;
}

void Task::SetupTaskRegisters(Task::REGISTERS *Registers , BYTE *Stack , QWORD EntryPoint) {
    __asm__ ("cli");
    memset(Registers , 0 , sizeof(Registers));
    Registers->RBP = (QWORD)Stack+TASK_STACKSIZE;
    Registers->RSP = (QWORD)Stack+TASK_STACKSIZE;
    Registers->CS = 0x08;
    Registers->DS = 0x10;
    Registers->ES = 0x10;
    Registers->FS = 0x10;
    Registers->GS = 0x10;
    Registers->SS = 0x10;
    Registers->RIP = EntryPoint;
    Registers->RFlags |= 0x3200;
    __asm__ ("sti");
}

QWORD Task::GetCurrentTaskID(void) {
    return TaskManager.Tasks[TaskManager.CurrentTaskIndex].ID;
}

QWORD Task::GetRunningTaskCount(void) {
    return TaskManager.TaskCount;
}

Task::TASKINFO *Task::GetTaskList(void) {
    QWORD i;
    Task::TASKINFO *List = (TASKINFO*)Memory::malloc(TASK_MAXCOUNT*sizeof(TASKINFO));
    List = TaskManager.Tasks;
    return List;
}


Task::TASKINFO Task::GetTaskInfo(QWORD ID) {
    return TaskManager.Tasks[ChangeIDToOffset(ID)];
}

void Task::SwitchTask(void) {
    QWORD CurrentTaskIndex = TaskManager.CurrentTaskIndex;
    QWORD NextTaskIndex = TaskManager.NextTaskIndex;
    char *ContextAddress = (char*)IST_STARTADDRESS+IST_SIZE-sizeof(REGISTERS);
    if(TaskManager.TaskCount == 1) {
        return;
    }
    TaskManager.CurrentTaskIndex++;
    TaskManager.NextTaskIndex++;
    if(TaskManager.CurrentTaskIndex >= TaskManager.TaskCount) {
        TaskManager.CurrentTaskIndex = 0x00;
    }
    if(TaskManager.NextTaskIndex >= TaskManager.TaskCount) {
        TaskManager.NextTaskIndex = 0x00;
    }
    if((TaskManager.Tasks[TaskManager.NextTaskIndex].Flags == TASK_NONE)||(TaskManager.Tasks[TaskManager.NextTaskIndex].Using == FALSE)) {
        return;
    }
    memcpy(&(TaskManager.Tasks[CurrentTaskIndex].Registers) , ContextAddress , sizeof(REGISTERS)); 
    memcpy(ContextAddress , &(TaskManager.Tasks[NextTaskIndex].Registers) , sizeof(REGISTERS));
}

BOOL Task::EndTask(QWORD ID) {
    QWORD i;
    __asm__ ("cli");
    if(!((ID >= TASK_MAINTASKID) && (ID <= TASK_MAINTASKID+TASK_MAXCOUNT))) {
        __asm__ ("sti");
        return FALSE;
    }
    if((TaskManager.Tasks[ChangeIDToOffset(ID)].Flags == TASK_SYSTEM)||(TaskManager.Tasks[ChangeIDToOffset(ID)].Flags == TASK_NONE)) {
        __asm__ ("sti");
        return FALSE;
    }
    TaskManager.TaskCount--;
    if(ChangeIDToOffset(ID) == TaskManager.TaskCount) {
        Memory::free(TaskManager.Tasks[ChangeIDToOffset(ID)].Stack);
        TaskManager.Tasks[ChangeIDToOffset(ID)].Flags = TASK_NONE;
        TaskManager.Tasks[ChangeIDToOffset(ID)].Using = FALSE;
        __asm__ ("sti");
        return TRUE;
    }
    Memory::free(TaskManager.Tasks[ChangeIDToOffset(ID)].Stack);
    for(i = ChangeIDToOffset(ID); i < TaskManager.TaskCount; i++) {
        memcpy(&(TaskManager.Tasks[i]) , &(TaskManager.Tasks[i+1]) , sizeof(TASKINFO));
    }
    Memory::free(TaskManager.Tasks[i].Stack);
    TaskManager.Tasks[i].ID = ID;
    TaskManager.Tasks[i].Flags = TASK_NONE;
    TaskManager.Tasks[i].Using = FALSE;
    __asm__ ("sti");
    return TRUE;
}

void Task::Exit(void) {
    Task::EndTask(Task::GetCurrentTaskID());
}

QWORD Task::ChangeIDToOffset(QWORD ID) {
    QWORD i;
    for(i = 0; i < TASK_MAXCOUNT; i++) {
        if(ID == TaskManager.Tasks[i].ID) {
            return i;
        }
    }
    return 0x00;
}

void Task::System(void) {
    while(1) {
        ;
    }
}

void Task::IDLE(void) {
    while(1) {
        ;
    }
}