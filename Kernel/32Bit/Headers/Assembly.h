#ifndef _ASSEMBLY_H_
#define _ASSEMBLY_H_

#include <Types.h>

void WritePort(WORD Port , BYTE Data);
BYTE ReadPotr(WORD Port);
void SwitchTo64BitAndJump(void);

#endif