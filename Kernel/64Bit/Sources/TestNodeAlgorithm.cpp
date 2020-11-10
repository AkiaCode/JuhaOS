#include <iostream>
#include <stdlib.h>

struct ONENODE {
    unsigned long ID;
    int Data;
    ONENODE *Next;
};

class NODE {
    public:

        void Initialize(int Count);
        unsigned long Insert(int Data);
        void Delete(unsigned long ID);
        ONENODE *GetNode(void);
        int GetNodeCount(void);
    private:
        int MaxNodeCount;
        int CurrentCount;
        ONENODE *Node;
};

int main(void) {
    int i;
    NODE Node;
    ONENODE *OneNode;
    Node.Initialize(10);
    Node.Insert(10);
    Node.Insert(20);
    Node.Insert(30);
    Node.Insert(40);
    Node.Insert(50);
    Node.Insert(60);
    Node.Insert(70);
    Node.Insert(80);
    Node.Insert(90);
    Node.Insert(100);
    Node.Insert(110);
    OneNode = Node.GetNode();
    for(i = 1; i < 15; i++) {
        if(Node.GetNodeCount() == 1) {
            std::cout << "Not switching ~_~" << std::endl;
            continue;
        }
        if((i%Node.GetNodeCount()) == 0) {
            std::cout << OneNode->Data << "->" << Node.GetNode()->Data << std::endl;
            OneNode = Node.GetNode();
            continue;
        }
        std::cout << OneNode->Data << "->" << OneNode->Next->Data << std::endl;
        OneNode = OneNode->Next;
    }
}

void NODE::Initialize(int Count) {
    int i;
    ONENODE *NextNode;
    CurrentCount = 0;
    MaxNodeCount = Count;
    this->Node = (ONENODE*)malloc(sizeof(ONENODE));
    this->Node->Data = 0x00;
    this->Node->Next = (ONENODE*)malloc(sizeof(ONENODE));
    this->Node->ID = 0x00;
    NextNode = this->Node;
    NextNode = NextNode->Next;

    for(i = 0; i < Count; i++) {
        NextNode->Data = 0x00;
        NextNode->ID = i+1;
        NextNode->Next = (ONENODE*)malloc(sizeof(ONENODE));
        NextNode = NextNode->Next;
    }
}

unsigned long NODE::Insert(int Data) {
    int i;
    ONENODE *NextNode = this->Node;
    if(CurrentCount == 0) {
        NextNode->Data = Data;
        CurrentCount++;
        return NextNode->ID;
    }
    for(i = 0; i < CurrentCount; i++) {
        NextNode = NextNode->Next;
    }
    NextNode->Data = Data;
    CurrentCount++;
    return NextNode->ID;
}

void NODE::Delete(unsigned long ID) {
    int i;
    int Index;
    ONENODE *NextNode = this->Node;
    if(ID == 0) {
        return;
    }
    for(i = 0; i < CurrentCount; i++) {
        if(NextNode->Next->ID == ID) {
            break;
        }
        NextNode = NextNode->Next;
    }
    NextNode->Next = NextNode->Next->Next;
    CurrentCount--;
}

ONENODE *NODE::GetNode(void) {
    return this->Node;
}

int NODE::GetNodeCount(void) {
    return CurrentCount;
}