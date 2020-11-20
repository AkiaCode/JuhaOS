#include <Shell.h>
#include <Common.h>
#include <ShellFunctions.h>
#include <Hal.h>

void Shell::Functions::clear(int argc , char **argv) {
    TextScreen::ClearScreen(0x07);
    return;
}

void Shell::Functions::help(int argc , char **argv) {
    int i;
    int j;
    int MaxLength = strlen(ShellFunctions[0].Command);
    if((strcmp(argv[1] , "me") == 0)||(strcmp(argv[1] , "us") == 0)) {
        TextScreen::SetColor(0x0E);
        TextScreen::printf("If you really need help, please call to 911 or 119\n");
        TextScreen::SetColor(0x07);
    }
    TextScreen::printf("Here's the commands which support in the shell.\n\n");
    for(i = 0; i < (int)(sizeof(ShellFunctions)/sizeof(Shell::CommandList)); i++) {
        if(strlen(ShellFunctions[i].Command) > MaxLength) {
            MaxLength = strlen(ShellFunctions[i].Command);
        }
    }
    for(j = 0; j < (int)(sizeof(ShellFunctions)/sizeof(Shell::CommandList)); j++) {
        TextScreen::printf("%s" , ShellFunctions[j].Command);
        for(i = 0; i < MaxLength-strlen(ShellFunctions[j].Command)+1; i++) {
            TextScreen::printf(" ");
        }
        TextScreen::printf("| %s\n" , ShellFunctions[j].Description);
    }
    return;
}

void Shell::Functions::print(int argc , char **argv) {
    int i;
    for(i = 1; i < argc+1; i++) {
        TextScreen::printf("%s " , argv[i]);
    }
    TextScreen::printf("\n");
}

void Shell::Functions::version(int argc , char **argv) {
    TextScreen::printf("JuhaOS version %s\n" , OS_VERSION);
}

void Shell::Functions::mem(int argc , char **argv) {
    QWORD UsingBlock = Memory::GetUsingBlock(); 
    QWORD Using = UsingBlock*MEMORY_BLOCKSIZE;
    TextScreen::printf("Total size : %dMB\n" , Memory::GetTotalSize()/1024/1024);
    if(Using < 1024) {
        TextScreen::printf("Using      : %dB\n" , Using);
    }
    else if((Using >= 1024) && (Using < 1024*1024)) {
        TextScreen::printf("Using      : %dKB\n" , Using/1024);
    }
    else if((Using >= 1024*1024) && (Using < 1024*1024*1024)) {
        TextScreen::printf("Using      : %dMB\n" , Using/1024/1024);
    }
    else if((Using >= 1024*1024*1024) && (Using < 1024*1024*1024*1024)) {
        TextScreen::printf("Using      : %dGB\n" , Using/1024/1024/1024);
    }
    return;
}

void Matrix(void) {
    QWORD i = 0;
    int j;
    BYTE *Buffer = (BYTE*)0xB8000;
    BYTE *Memory = (BYTE*)0x000;
    int X = Common::rand()%80;
    int Y = 0x00;
    int Count = (Common::rand()%5)+2;
    while(1) {
        for(j = 0; j < Count; j++) {
            Buffer[(Y*80*2)+(X*2)] = Common::rand()%255;
            Buffer[(Y*80*2)+(X*2)+1] = 0x0A;
            if(j == Count-1) {
                Buffer[(Y*80*2)+(X*2)+1] = 0x07;
            }
            Y += 1;
        }

        Y -= Count-1;
        delay(50);

        for(j = 0; j < Count; j++) {
            Buffer[((Y-Count-j)*80*2)+(X*2)] = ' ';
            Buffer[((Y-Count-j)*80*2)+(X*2)+1] = 0x07;
        }

        if(Y >= 25+Count) {
            Y = 0;
            X = Common::rand()%80;
            Count = (Common::rand()%5)+2;
        }
    }
}

void Shell::Functions::testmatrix(int argc , char **argv) {
    const int MaxTaskCount = 100;
    int i;
    QWORD ID;
    QWORD FirstID;
    QWORD TaskList[200];
    TextScreen::EnableCursor(0xFF , 0xFF);
    Common::srand(Hal::Timer::GetTickCount());
    TextScreen::ClearScreen(0x0A);
    delay(2000);
    for(i = 0; i < MaxTaskCount; i++) {
        ID = Task::CreateTask((QWORD)Matrix , TASK_DEFAULT , "MatrixTask" , "The task for matrix!");
        TaskList[i] = ID;
        delay(Common::rand()%500);
    }
    while(1) {
        if(getch() == '\b') {
            break;
        }
    }
    for(i = 0; i < MaxTaskCount; i++) {
        Task::EndTask(TaskList[i]);
    }
    TextScreen::EnableCursor(0x00 , 0xFF);
    TextScreen::SetColor(0x07);
}

void Shell::Functions::endtask(int argc , char **argv) {
    QWORD i;
    QWORD TaskID;
    QWORD EndID;
    if(argc == 0) {
        TextScreen::printf("Usage:%s [Start ID] [End ID]\n" , argv[0]);
        return;
    }
    if(argc == 1) {
        if((argv[1][0] == '0') && (argv[1][1] == 'x')) {
            TaskID = atol(argv[1]+2);
        }
        else {
            TaskID = atol(argv[1]);
        }
        if(Task::EndTask(TaskID) == FALSE) {
            TextScreen::printf("Task 0x%X is not running or system task.\n" , TaskID);
            return;
        }
        TextScreen::printf("End the task 0x%X\n" , TaskID);
    }
    if(argc == 2) {
        if((argv[1][0] == '0') && (argv[1][1] == 'x')) {
            TaskID = atol(argv[1]+2);
        }
        else {
            TaskID = atol(argv[1]);
        }
        if((argv[2][0] == '0') && (argv[2][1] == 'x')) {
            EndID = atol(argv[2]+2);
        }
        else {
            EndID = atol(argv[2]);
        }
        for(i = 0; i < EndID-TaskID; i++) {
            if(Task::EndTask(TaskID+i) == FALSE) {
                TextScreen::printf("Task 0x%X is not running or system task.\n" , TaskID+i);
            }
            else {
                TextScreen::printf("End the task 0x%X\n" , TaskID+i);
            }
        }
    }
}

void Shell::Functions::endtaskall(int argc , char **argv) {
    int i;
    const QWORD TaskCount = Task::GetRunningTaskCount();
    for(i = 0; i < TASK_MAXCOUNT; i++) {
        Task::EndTask(TASK_MAINTASKID+i);
    }
}
 
void Shell::Functions::tasklist(int argc , char **argv) {
    QWORD i;
    QWORD j;
    BYTE ColorSaver;
    int MaxNameLength;
    const QWORD TaskCount = Task::GetRunningTaskCount();
    Task::TASKINFO *TaskInfo = Task::GetTaskList();
    MaxNameLength = strlen(TaskInfo[0].Name);
    TextScreen::printf("Current running task count : %d\n" , Task::GetRunningTaskCount());
    for(i = 0; i < TaskCount; i++) {
        if(MaxNameLength < strlen(TaskInfo[i+1].Name)) {
            MaxNameLength = strlen(TaskInfo[i+1].Name);
        }
    }
    TextScreen::printf("ID      (Type   ) | Name");
    for(i = 0; i < (QWORD)MaxNameLength-4; i++) {
        TextScreen::printf(" ");
    }
    TextScreen::printf(" | Description\n");
    TextScreen::printf("--------------------------------------------\n");
    for(i = 0; i < TaskCount; i++) {
        ColorSaver = TextScreen::GetColor();
        if(TaskInfo[i].Flags == TASK_SYSTEM) {
            TextScreen::SetColor(0x04+(ColorSaver>>4<<4));
        }
        if(((i%5) == 0) && (i >= 5)) {
            TextScreen::printf("Press any key to continue(q to exit):");
            if(getch() == 'q') {
                TextScreen::printf("\n");
                return;
            }
            TextScreen::printf("\n");
        }
        TextScreen::printf("0x%X" , TaskInfo[i].ID);
        if(TaskInfo[i].Flags == TASK_SYSTEM) {
            TextScreen::printf("(System )");
        }
        else if(TaskInfo[i].Flags == TASK_DEFAULT) {
            TextScreen::printf("(Default)");
        }
        TextScreen::printf(" | %s" , TaskInfo[i].Name);
        for(j = 0; j < MaxNameLength-strlen(TaskInfo[i].Name); j++) {
            TextScreen::printf(" ");
        }
        TextScreen::printf(" | %s\n" , TaskInfo[i].Description);
        TextScreen::SetColor(ColorSaver);
    }
}

void Shell::Functions::ShellDelay(int argc , char **argv) {
    QWORD Millisecond;
    if(argc == 0) {
        TextScreen::printf("Usage:delay [Millisecond]\n");
        return;
    }
    Millisecond = atoi(argv[1]);
    TextScreen::printf("Delay for %dms\n" , Millisecond);
    delay(Millisecond);
}