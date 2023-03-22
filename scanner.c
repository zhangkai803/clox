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
    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\n') {
            scanner.line++;
        }
        advance();
    }

    if (isAtEnd()) {
        return errorToken("Unterminated string.");
    }
    advance();
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

static TokenType makeKeyword() {
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
    return makeToken(makeKeyword());
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
    skipWhiteSpace();
    scanner.start = scanner.current;
    // printf(">> %c \n", peek());

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
    // case '/': return makeToken(TOKEN_SLASH);
    // 双字符部分
    case '!':
        if (match('=')) {
            return makeToken(TOKEN_BANG_EQUAL);
        }
        return makeToken(TOKEN_BANG);
    case '=':
        if (match('=')) {
            return makeToken(TOKEN_EQUAL_EQUAL);
        }
        return makeToken(TOKEN_EQUAL);
    case '<':
        if (match('=')) {
            return makeToken(TOKEN_LESS_EQUAL);
        }
        return makeToken(TOKEN_LESS);
    case '>':
        if (match('=')) {
            return makeToken(TOKEN_GREATER_EQUAL);
        }
        return makeToken(TOKEN_GREATER);
    case '/':
        if (match('/')) {
            while (!isAtEnd() && !match('\n')) {
                advance();
            }
        } else {
            return makeToken(TOKEN_SLASH);
        }
    case '"':
        return makeString();
    default:
        break;
    }
    if (isDigit(c)) {
        return makeNumber();
    }
    if (isAlpha(c)) {
        return makeIdentifier();
    }

    return errorToken("Unexpected character.");
}
