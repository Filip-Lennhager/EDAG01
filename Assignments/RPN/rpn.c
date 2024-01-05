#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define STACK_SIZE 10

int stack[STACK_SIZE];
int top = -1;
char lastChar; // Global variable to store the last character read

void push(int number, FILE *fout, int *errorFlag, int lineCount) {
    if (top >= STACK_SIZE - 1) {
        fprintf(fout, "line %d: error at %c", lineCount, lastChar); // Error caused by lastChar
        *errorFlag = 1;
    } else {
        stack[++top] = number;
    }
}

int pop(FILE *fout, int *errorFlag, int lineCount) {
    if (top < 0) {
        fprintf(fout, "line %d: error at \\n", lineCount); // Stack underflow
        *errorFlag = 1;
        return 0;
    }
    return stack[top--];
}

void applyOperation(char operator, FILE *fout, int *errorFlag, int lineCount) {
    if (top < 1) {
        fprintf(fout, "line %d: error at %c", lineCount, operator); // Not enough elements
        *errorFlag = 1;
        return;
    }

    int y = pop(fout, errorFlag, lineCount);
    int x = pop(fout, errorFlag, lineCount);

    if (*errorFlag) return;

    switch(operator) {
        case '+': push(x + y, fout, errorFlag, lineCount); break;
        case '-': push(x - y, fout, errorFlag, lineCount); break;
        case '*': push(x * y, fout, errorFlag, lineCount); break;
        case '/':
            if (y == 0) {
                fprintf(fout, "line %d: error at /", lineCount); // Divide by zero
                *errorFlag = 1;
                return;
            }
            push(x / y, fout, errorFlag, lineCount);
            break;
        default:
            fprintf(fout, "line %d: error at %c", lineCount, operator);
            *errorFlag = 1;
    }
}

void resetStack(void) {
    top = -1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <inputfile> <outputfile>\n", argv[0]);
        return 1;
    }

    FILE *fin = fopen(argv[1], "r");
    FILE *fout = fopen(argv[2], "w");

    if (fin == NULL || fout == NULL) {
        perror("Error opening file");
        return 1;
    }

    int ch, number, lineCount = 1, errorFlag = 0;

    while (1) {
        ch = fgetc(fin);
        lastChar = ch;

        if (ch == EOF) {
            if (!feof(fin)) { // Read error
                fprintf(fout, "line %d: read error\n", lineCount);
            }
            break;
        }

        if (ch == '\n') {
            if (errorFlag) {
                fprintf(fout, "\n");
                errorFlag = 0;
            } else if (top == 0) {
                fprintf(fout, "line %d: %d\n", lineCount, pop(fout, &errorFlag, lineCount));
            } else {
                fprintf(fout, "line %d: error at \\n\n", lineCount);
            }
            lineCount++;
            resetStack();
            continue;
        }

        if (errorFlag) {
            if (ch == '\n') {
                fprintf(fout, "\n");
                lineCount++;
                errorFlag = 0;
                resetStack();
            }
            continue;
        }

        if (isdigit(ch)) {
            ungetc(ch, fin);
            fscanf(fin, "%d", &number);
            push(number, fout, &errorFlag, lineCount);
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            applyOperation(ch, fout, &errorFlag, lineCount);
        } else if (!isspace(ch)) {
            fprintf(fout, "line %d: error at %c", lineCount, ch);
            errorFlag = 1;
        }
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
