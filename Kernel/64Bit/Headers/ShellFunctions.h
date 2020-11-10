#ifndef _SHELLFUNCTIONS_H_
#define _SHELLFUNCTIONS_H_

#include <Shell.h>

namespace Shell::Functions {
    void clear(int argc , char **argv);
    void help(int argc , char **argv);
    void print(int argc , char **argv);
    void version(int argc , char **argv);
    void mem(int argc , char **argv);
    void testmatrix(int argc , char **argv);
    void endtask(int argc , char **argv);
    void endtaskall(int argc , char **argv);
    void tasklist(int argc , char **argv);
    void ShellDelay(int argc , char **argv);
}

static Shell::CommandList ShellFunctions[] = {
    {"help" , "It's help" , Shell::Functions::help} , 
    {"clear" , "Clear the shell" , Shell::Functions::clear} , 
    {"print" , "Print some messages in the shell" , Shell::Functions::print} , 
    {"version" , "Get OS version" , Shell::Functions::version} , 
    {"mem" , "Check your PC's memory" , Shell::Functions::mem} , 
    {"matrix" , "M A T R I X " , Shell::Functions::testmatrix} , 
    {"endtask" , "End the task" , Shell::Functions::endtask} , 
    {"endtaskall" , "End all task" , Shell::Functions::endtaskall} , 
    {"tasklist" , "Get running tasks list" , Shell::Functions::tasklist} , 
    {"delay" , "Delay for millisecond" , Shell::Functions::ShellDelay} , 
};

#endif