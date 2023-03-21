#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"
// 反编译 chunk
void disassembleChunk(Chunk* chunk, const char* name);
// 反编译 chunk 中 offset 位置的指令 并返回下一条指令的偏移量
int disassembleInstruction(Chunk* chunk, int offset);

#endif
