/**
 * 定义代码的表示形式
*/
#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

// 指令对应的操作码
typedef enum {
    OP_CONSTANT,  // 生成常数
    OP_RETURN,  // return 指令
} OpCode;

// 字节码序列 动态数组
typedef struct {
    int count;  // 当前元素数量
    int capacity;  // 容量
    uint8_t* code;
    int* lines;  // 行号
    ValueArray constants;  // 保存常量
} Chunk;

// 初始化
void initChunk(Chunk* chunk);
// 释放 chunk
void freeChunk(Chunk* chunk);
// 添加
void writeChunk(Chunk* chunk, uint8_t byte, int line);
// 判断是否满了
bool chunkFull(Chunk* chunk);
// 添加一个常量 返回添加之后的索引
int addConstant(Chunk* chunk, Value value);

# endif
