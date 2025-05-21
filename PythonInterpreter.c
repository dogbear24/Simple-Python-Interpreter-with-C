//Supports simple print statements, Integers and operators



#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


//Forward Declarations
int parse_term();
int parse_factor();
int parse_expression();
void interpreter(const char* input);
int parse_number();
void skip_whitespace();
char advance();
char peek();


/**
 * Lexer
 */

//Gobal State
const char* src; //The current line of code to interpret
int pos; //The current index

//Helpers
char peek() { return src[pos]; } //Looks ahead

char advance() { return src[pos++]; } //Moves to next character

void skip_whitespace() {
    while (isspace(peek())) advance();
}

int parse_number() {
    int val = 0;
    while (isdigit(peek())) {
        val = val * 10 + (advance() - '0'); // Converts to int with ASCII code
    }
    return val;
}


/**
 * Parser (Recursive Descent)
 */

int parse_term() {
    int value = parse_factor();
    while (1) {
        skip_whitespace();
        if (peek() == '*' || peek() == '/') {
            char op = advance();
            int rhs = parse_factor();
            if (op == '*') {
                value *= rhs;
            } else {
                if (rhs == 0) {
                    printf("Can't divide by 0");
                    exit(1);
                }
                value /= rhs;
            }
        } else {
            break;
        }
    }
    return value;
}

int parse_factor() {
    skip_whitespace();
    if (isdigit(peek())) {
        return parse_number();
    } else if (peek() == '(') {
        advance();
        int value = parse_expression();
        if (peek() == ')') advance();
        return value;
    }
    printf("Invalid Context Free Grammar: %c\n", peek());
    exit(1);
}

int parse_expression() {
    int value = parse_term();
    while (1) {
        skip_whitespace();
        if (peek() == '+' || peek() == '-') {
            char op = advance();
            int rhs = parse_term();
            if (op == '+') value += rhs;
            else value -= rhs;
        } else {
            break;
        }
    }
    return value;
}

// Starting Point

void interpreter(const char* input) {
    src = input;
    pos = 0;

    skip_whitespace();
    int length = 5;
    if (strncmp(src + pos, "print", length) == 0) {
        pos += 5;
        skip_whitespace();
        if (peek() == '(') {
            advance();
            int result = parse_expression();
            if (peek() == ')') advance();
            printf("%d\n", result);
        } else {
            printf("Syntax error: expected '('\n");
        }
    } else {
        printf("Only 'print(expr)' is supported.\n");
    }
}



// Main

int main() {
    char line[256];
    printf("Simple Python Interpreter\n> ");
    while (fgets(line, sizeof(line), stdin)) {
        interpreter(line);
        printf("> ");
    }
    return 0;

}
