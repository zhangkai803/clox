/**
 * 表示 lox 中的值
*/
#ifndef clox_value_h
#define clox_value_h

#include "common.h"

// 表示单个值
typedef double Value;

// 用数组表示多个值
typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
bool valueArrayFull(ValueArray* array);
void printValue(Value value);

#endif