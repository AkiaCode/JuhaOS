#ifndef _COMMON_H_
#define _COMMON_H_

#include <Types.h>
#include <DescriptorTables.h>
#include <TextScreen.h>
#include <MemoryManagement.h>
#include <Task.h>

#define QUEUE_MAXSIZE 8192
#define ARGUMENTS_STRING_MAXSIZE 8192

namespace Common {
	class Queue {
		public:
			void Initialize(int MaxSize);
			BOOL CheckEmpty(void);
			BOOL CheckFull(void);
			BOOL Enqueue(BYTE Data);
			BYTE Dequeue(void);
		private:
			int MaxSize;
			int Front;
			int Rear;
			BYTE Buffer[QUEUE_MAXSIZE];
	};
	class Arguments {
		public:
			void Initialize(char *String);
			int GetArg(char *Argument);
		private:
			char *String;
			int Length;
			int CurrentPosition;
	};
    int gets(char *Buffer);
    void srand(QWORD Seed);
	QWORD rand(void);
};

#endif