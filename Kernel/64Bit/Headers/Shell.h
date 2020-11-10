#ifndef _SHELL_H_
#define _SHELL_H_

#include <Types.h>
#include <string.h>

#pragma pack(push , 1)

namespace Shell {
    typedef struct {
        const char *Command;
        const char *Description;
        void(*Execution)(int argc , char **argv);
    }CommandList;
    void Start(void);
};

#pragma pack(pop)

#endif