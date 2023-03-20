/**
 * 定义代码的表示形式
*/
#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"

// 指令对应的操作码
typedef enum {
    OP_RETURN,
} OpCode;

// 字节码序列 动态数组
typedef struct {
    int count;  // 当前元素数量
    int capacity;  // 容量
    uint8_t* code;
} Chunk;

// 初始化
void initChunk(Chunk* chunk);
// 释放 chunk
void freeChunk(Chunk* chunk);
// 添加
void writeChunk(Chunk* chunk, uint8_t byte);
// 判断是否满了
bool chunkFull(Chunk* chunk);

# endif
