#ifndef clox_scanner_h
#define clox_scanner_h

typedef enum {
    // 字符
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,  // ( )
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,  // { }
    TOKEN_COMMA, TOKEN_DOT, TOKEN_SEMECOLON,  // , . ;
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH,  // + - * /

    // 操作符
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
    TOKEN_ERRPR  // 错误标记
} TokenType;

typedef struct
{
    const char* start;
    const char* current;
    int line;
} Scanner;

typedef struct
{
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;


void initScanner(const char* source);
Token scanToken();

#endif
