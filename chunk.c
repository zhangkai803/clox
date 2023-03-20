#include <stdlib.h>
#include "memory.h"
#include "chunk.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    initChunk(chunk);
}

bool chunkFull(Chunk* chunk) {
    return (chunk->count + 1) > chunk->capacity;
}

void writeChunk(Chunk* chunk, uint8_t byte) {
    if (chunkFull(chunk)) {
        // 如果满了
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;
}