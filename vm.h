#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;  // 指令块/序列
    uint8_t* ip;  // 下一条要执行的指令所在的位置 并非是指令块的索引 是直接指向指令的指针
    Value stack[STACK_MAX];  // 数组实现的栈
    Value* stackTop;  // 存栈顶指针 解引用比计算索引偏移快
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPLE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);
void push(Value value);
Value pop();

#endif
