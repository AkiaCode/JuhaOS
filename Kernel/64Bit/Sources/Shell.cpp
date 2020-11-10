#include <Shell.h>
#include <Common.h>
#include <TextScreen.h>
#include <string.h>

#include <ShellFunctions.h>

void Shell::Start(void) {
    int i;
    int Length;
    char Buffer[4096];
    char **argv;
    int argc;
    BOOL Founded;
    Common::Arguments Args;
    TextScreen::printf("JuhaOS version %s\n" , OS_VERSION);
    TextScreen::printf("(C) 2017-2020. all rights reserved by Juha.\n\n");
    while(1) {
        TextScreen::printf("SHELL > ");
        Length = Common::gets(Buffer);
        if(Length == 0) {
            continue;
        }
        Args.Initialize(Buffer);
        argc = 0;
        for(i = 0; i < strlen(Buffer); i++) {
            if(Buffer[i] == ' ') {
                argc++;
            }
        }
        i = 0;
        argv = (char**)Memory::malloc(argc*sizeof(char*));
        while(1) {
            memset(argv[i] , 0 , sizeof(argv[i]));
            argv[i] = (char*)Memory::malloc(Length);
            if(Args.GetArg(argv[i]) == 0) {
                break;
            }
            i++;
        }
        Founded = FALSE;
        if(strcmp(argv[0] , "wa") == 0) {
            TextScreen::printf("sans\n");
            for(i = 0; i <= argc; i++) {
                Memory::free(argv[i]);
            }
            Memory::free(argv);
            continue;
        }
        else if(strcmp(argv[0] , "undertale") == 0) {
            TextScreen::printf("is super awesome\n");
            for(i = 0; i <= argc; i++) {
                Memory::free(argv[i]);
            }
            Memory::free(argv);
            continue;
        }
        else if(strcmp(argv[0] , "deltarune") == 0) {
            TextScreen::printf("is also super awesome\n");
            for(i = 0; i <= argc; i++) {
                Memory::free(argv[i]);
            }
            Memory::free(argv);
            continue;
        }
        else if(strcmp(argv[0] , "cls") == 0) {
            TextScreen::printf("Sorry, but this is not a Windows.\n");
            for(i = 0; i <= argc; i++) {
                Memory::free(argv[i]);
            }
            Memory::free(argv);
            continue;
        }
        for(i = 0; i < (int)(sizeof(ShellFunctions)/sizeof(Shell::CommandList)); i++) {
            if(strcmp(ShellFunctions[i].Command , argv[0]) == 0) {
                ShellFunctions[i].Execution(argc , argv);
                Founded = TRUE;
            }
        }
        if(Founded == FALSE) {
            TextScreen::printf("There's no command like that in this shell : \"%s\"\n" , argv[0]);
        }
        for(i = 0; i <= argc; i++) {
            Memory::free(argv[i]);
        }
        Memory::free(argv);
    }
} 