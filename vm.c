#include <stdio.h>
#include "common.h"
#include "vm.h"
#include "compiler.h"
#include "debug.h"

// 声明全局的 VM 我们只需要一个虚拟机
VM vm;

static void resetStack() {
    vm.stackTop = vm.stack;
}

void initVM() {
    resetStack();
}

void freeVM() {}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

/**
 * 解释器核心方法
*/
static InterpretResult run () {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
        double b = pop(); \
        double a = pop(); \
        push(a op b); \
    } while (false)  // 使用 do-while(false) 的作用是为了避免宏展开时 只运行第一条指令

    for (;;) {

#ifdef DEBUG_TRACE_EXECUTION
    // 如果有开debug
    // 输出堆栈信息
    printf("vm.stack: ");
    for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
        printf("[ ");
        printValue(*slot);
        printf(" ]");
    }
    printf("\n");
    // 输出指令字节码
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif

        uint8_t instruction = READ_BYTE();
        switch (instruction) {
            case OP_CONSTANT: {
                // 声明常量 入栈
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_ADD:
                BINARY_OP(+);
                break;
            case OP_SUBTRACT:
                BINARY_OP(-);
                break;
            case OP_MULTIPLY:
                BINARY_OP(*);
                break;
            case OP_DIVIDE:
                BINARY_OP(/);
                break;
            case OP_NAGATE:
                // 拿出来 负操作 再塞回去
                push(-pop());
                break;
            case OP_RETURN: {
                // 返回值 出栈并打印
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#undef BINARY_OP
#undef READ_CONSTANT
#undef READ_BYTE
}

InterpretResult interpret(const char* source) {
    // 初始化一个空的字节码块
    Chunk chunk;
    initChunk(&chunk);

    // 编译源代码 把字节码写入到块中
    if (!compile(source, &chunk)) {
        // 如果编译出错 返回错误 并释放字节码块的内存
        freeChunk(&chunk);
        return INTERPRET_COMPLE_ERROR;
    }

    // 编译完成后
    // 把字节码块交给虚拟机
    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    // 解释执行
    InterpretResult result = run();

    // 执行结束释放字节码块的内存
    freeChunk(&chunk);
    return result;
}
