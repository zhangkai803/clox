#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"
// 反编译 chunk
void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstruction(Chunk* chunk, int offset);

#endif
