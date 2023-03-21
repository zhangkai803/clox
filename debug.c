#include <stdio.h>
#include "debug.h"

static int constantInstruction(const char* name, Chunk* chunk, int offset);
static int simpleInstruction(const char* name, int offset);

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);
    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

int disassembleInstruction(Chunk* chunk, int offset) {
    // 先打印指令偏移
    printf("%04d ", offset);

    // 打印行号
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        // 如果当前指令 与上一条指令在同一行 那么就展示一条竖线
        printf("   | ");
    } else {
        printf("%4d ", chunk->lines[offset]);
    }

    // 打印指令内容
    uint8_t instruction = chunk->code[offset];
    switch (instruction)
    {
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    case OP_ADD:
        return simpleInstruction("OP_ADD", offset);
    case OP_SUBTRACT:
        return simpleInstruction("OP_SUBTRACT", offset);
    case OP_MULTIPLY:
        return simpleInstruction("OP_MULTIPLY", offset);
    case OP_DIVIDE:
        return simpleInstruction("OP_DIVIDE", offset);
    case OP_NAGATE:
        return simpleInstruction("OP_NAGATE", offset);
    case OP_CONSTANT:
        return constantInstruction("OP_CONSTANT", chunk, offset);
    default:
        printf("Unknow opcode %d\n", instruction);
        return offset + 1;
    }
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);  // 左对齐留16个字符的位置打印操作码
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}
