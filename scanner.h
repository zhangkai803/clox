#ifndef clox_scanner_h
#define clox_scanner_h

typedef enum {
    // 单字符
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,  // ( )
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,  // { }
    TOKEN_COMMA, TOKEN_DOT, TOKEN_SEMICOLON,  // , . ;
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH,  // + - * /

    // 连字符
    TOKEN_BANG, TOKEN_BANG_EQUAL,  // ! !=
    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,  // = ==
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,  // > >=
    TOKEN_LESS, TOKEN_LESS_EQUAL,  // < <=

    // 字面量
    TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_STRING,  // variable number string

    // 关键字
    TOKEN_AND, TOKEN_OR,  // and or
    TOKEN_IF, TOKEN_ELSE,  // if else
    TOKEN_FOR, TOKEN_WHILE,  // for while
    TOKEN_CLASS, TOKEN_FUN, TOKEN_VAR,  // class fun var
    TOKEN_TRUE, TOKEN_FALSE, TOKEN_NIL,  // true false nil
    TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,  // print return super this

    // 其他
    TOKEN_EOF,  // end of file 结束标记
    TOKEN_ERROR  // 错误标记
} TokenType;

void printTokenType(TokenType type);

typedef struct {
    const char* current;  // 当前扫描到的位置
    const char* start;  // 开始扫描的位置
    int line;  // 当前扫描到的行号
} Scanner;

typedef struct {
    TokenType type;  // 符号类型
    const char* start;  // 符号开始位置
    int length;  // 符号长度
    int line;  // 符号所在行
} Token;


void initScanner(const char* source);
Token scanToken();

#endif
