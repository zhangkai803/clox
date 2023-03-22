#include <stdio.h>
#include "common.h"
#include "scanner.h"
#include "string.h"

Scanner scanner;

void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static bool isAtEnd() {
    return *scanner.current == '\0';
}

static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

static Token errorToken(const char* message) {
    Token token;
    token.type = TOKEN_ERRPR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner.line;
    return token;
}

static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

static char peek() {
    return *scanner.current;
}

static bool check(char c) {
    return peek() == c;
}

static bool match(char c) {
    if (isAtEnd()) {
        return false;
    }
    if (check(c)) {
        advance();
        return true;
    }
    return false;
}

static char peekNext() {
    if (isAtEnd()) {
        return '\0';
    }
    return scanner.current[1];
}

static Token makeString() {
    /**
     * 这个方法的实现和书中不一样
     * 按自己的思路改了一下 功能完全一样
     * 核心是把 peek() != '"' 改成了 match('"')
     * 因为 match 就是用作匹配的
     * 不一样的地方就是 match 匹配成功时会自动往前走一步
    */
    while (!isAtEnd() && !match('"')) {
        if (peek() == '\n') {
            scanner.line++;
        }
        advance();  // 这里一定要往前 因为 match 只有匹配成功时才往前
    }

    if (isAtEnd()) {
        return errorToken("Unterminated string.");
    }
    /**
     * 下面这行注释了
     * 能走到这里 说明 isAtEnd == false，没有到文件最后
     * 所以肯定是 match 成功匹配到双引号
     * match 匹配成功时 会自动往前走一步
     * 所以这里到 advance 要注释
     */
    // advance();
    return makeToken(TOKEN_STRING);
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static Token makeNumber() {
    while (isDigit(peek())) {
        advance();
    }
    if (peek() == '.' && isDigit(peekNext())) {
        advance();
        while (isDigit(peek())) {
            advance();
        }
    }
    return makeToken(TOKEN_NUMBER);
}

static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {
    if (scanner.current - scanner.start == start + length && memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

static TokenType matchKeyword() {
    /**
     * 字典树 - -
    */
    char c = scanner.start[0];
    switch (c)
    {
    case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
    case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
    case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
    case 'f':
        if (scanner.current - scanner.start > 1) {
            switch (scanner.start[1]) {
            case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
            case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
            case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
            }
        }
        break;
    case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
    case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
    case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
    case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
    case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
    case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
    case 't':
        if (scanner.current - scanner.start > 1) {
            switch (scanner.start[1]) {
            case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
            case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
            }
        }
        break;
    case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
    case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    default:
        break;
    }
    return TOKEN_IDENTIFIER;
}

static Token makeIdentifier() {
    while (isAlpha(peek()) || isDigit(peek())) {
        advance();
    }
    return makeToken(matchKeyword());
}

void skipWhiteSpace() {
    // 空白字符掠过
    for (;;) {
        char c = peek();

        switch (c) {
        case ' ':
        case '\r':
        case '\t':
            advance();
            break;
        // 换行字符 行数+1
        case '\n':
            scanner.line++;
            advance();
            break;
        default:
            return;
        }
    }
}

Token scanToken() {
    /**
     * 下面这两行的顺序不能变
     * 在跳过空格时会调整 current 的值
     * 一定要先完成跳过 再重置 start 才能正确指向下一个 token 的开头
    */
    skipWhiteSpace();
    scanner.start = scanner.current;

    if (isAtEnd()) {
        return makeToken(TOKEN_EOF);
    }

    char c = advance();
    switch (c)
    {
    // 单字符部分
    case '(': return makeToken(TOKEN_LEFT_PAREN);
    case ')': return makeToken(TOKEN_LEFT_PAREN);
    case '{': return makeToken(TOKEN_LEFT_PAREN);
    case '}': return makeToken(TOKEN_LEFT_PAREN);
    case ',': return makeToken(TOKEN_COMMA);
    case '.': return makeToken(TOKEN_DOT);
    case ';': return makeToken(TOKEN_SEMECOLON);
    case '+': return makeToken(TOKEN_PLUS);
    case '-': return makeToken(TOKEN_MINUS);
    case '*': return makeToken(TOKEN_STAR);
    // 双字符部分
    case '!':
        // 遇到 ! 要看下是不是 !=
        if (match('=')) {
            return makeToken(TOKEN_BANG_EQUAL);
        }
        return makeToken(TOKEN_BANG);
    case '=':
        // 遇到 = 要看下是不是 ==
        if (match('=')) {
            return makeToken(TOKEN_EQUAL_EQUAL);
        }
        return makeToken(TOKEN_EQUAL);
    case '<':
        // 遇到 < 要看下是不是 <=
        if (match('=')) {
            return makeToken(TOKEN_LESS_EQUAL);
        }
        return makeToken(TOKEN_LESS);
    case '>':
        // 遇到 > 要看下是不是 >=
        if (match('=')) {
            return makeToken(TOKEN_GREATER_EQUAL);
        }
        return makeToken(TOKEN_GREATER);
    case '/':
        // 遇到 / 要看下是不是 //
        if (match('/')) {
            while (!isAtEnd() && !match('\n')) {
                advance();
            }
        } else {
            return makeToken(TOKEN_SLASH);
        }
    case '"':
        // 遇到双引号 是字符串字面量
        return makeString();
    default:
        break;
    }
    // 下面是一些批量的字符判断
    // 匹配到 0-9 是数值字面量
    if (isDigit(c)) {
        return makeNumber();
    }
    // 匹配到字母 有可能是关键字或者变量名
    if (isAlpha(c)) {
        return makeIdentifier();
    }

    return errorToken("Unexpected character.");
}
