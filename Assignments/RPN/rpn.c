#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define STACK_SIZE 10

int stack[STACK_SIZE];
int top = -1;
char lastChar; // Global variable to store the last character read

int push(int number, int *errorFlag, int lineCount) {
    if (top >= STACK_SIZE - 1) {
        printf("line %d: error at %c", lineCount, lastChar); // Error caused by lastChar
        *errorFlag = 1;
    } else {
        stack[++top] = number;
    }
    return 0;
}

int pop(int *errorFlag, int lineCount) {
    if (top < 0) {
        printf("line %d: error at \\n", lineCount); // Stack underflow
        *errorFlag = 1;
        return 0;
    }
    return stack[top--];
}

int applyOperation(char operator, int *errorFlag, int lineCount) {
    if (top < 1) {
        printf("line %d: error at %c", lineCount, operator); // Not enough elements
        *errorFlag = 1;
        return 1;
    }

    int y = pop(errorFlag, lineCount);
    int x = pop(errorFlag, lineCount);

    if (*errorFlag) return 1;

    switch(operator) {
        case '+': push(x + y, errorFlag, lineCount); break;
        case '-': push(x - y, errorFlag, lineCount); break;
        case '*': push(x * y, errorFlag, lineCount); break;
        case '/':
            if (y == 0) {
                printf("line %d: error at /", lineCount); // Divide by zero
                *errorFlag = 1;
                 return 1;
            }
            push(x / y, errorFlag, lineCount);
            break;
        default:
            printf("line %d: error at %c", lineCount, operator);
            *errorFlag = 1;
    }
    return 0;
}

int resetStack(void) {
    top = -1;
    return 0;
}

int main(void) {
    int ch, number, lineCount = 1, errorFlag = 0;

    while (1) {
        ch = getchar();
        lastChar = ch;

        if (ch == EOF) {
            if (!feof(stdin)) { // Read error
                printf("line %d: read error\n", lineCount);
            }
            break;
        }

        if (ch == '\n') {
            if (errorFlag) {
                printf("\n");
                errorFlag = 0;
            } else if (top == 0) {
                printf("line %d: %d\n", lineCount, pop(&errorFlag, lineCount));
            } else {
                printf("line %d: error at \\n\n", lineCount);
            }
            lineCount++;
            resetStack();
            continue;
        }

        if (errorFlag) {
            if (ch == '\n') {
                printf("\n");
                lineCount++;
                errorFlag = 0;
                resetStack();
            }
            continue;
        }

        if (isdigit(ch)) {
            ungetc(ch, stdin);
            int result = scanf("%d", &number);
            if (result != 1) {
                // Handle the error appropriately
                printf("line %d: error reading number\n", lineCount);
                errorFlag = 1;
                continue;
            }
            push(number, &errorFlag, lineCount);
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            applyOperation(ch, &errorFlag, lineCount);
        } else if (!isspace(ch)) {
            printf("line %d: error at %c", lineCount, ch);
            errorFlag = 1;
        }
    }

    return 0;
}
