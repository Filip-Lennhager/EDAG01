#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100
#define HASH_TABLE_SIZE 1000

typedef struct Node {
    char word[MAX_WORD_LENGTH];
    int count;
    struct Node* next;
}Node;

Node* hashTable[HASH_TABLE_SIZE];

// Hash function to map a string to an index in the hash table
unsigned int hash(char* str) {
    unsigned int value = 0;
    for (; *str; ++str) {
        value = value * 37 + *str;
    }
    return value % HASH_TABLE_SIZE;
}

int isPrime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

void insertOrUpdate(char* word) {
    int index = hash(word);
    Node* node = hashTable[index];

    // Search for the word in the list at hash index
    while (node != NULL) {
        if (strcmp(node->word, word) == 0) {
            node->count++;
            printf("counted %s\n", word);
            return; // Word found and count incremented
        }
        node = node->next;
    }

    // Word not found, create a new node and insert it at the beginning of the list
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->word, word);
    newNode->count = 1;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
    printf("added %s\n", word);
}

// Function to delete a word from the hash table
int delete(char* word) {
    int index = hash(word);
    Node* node = hashTable[index];
    Node* prev = NULL;

    while (node != NULL) {
        if (strcmp(node->word, word) == 0) {
            if (prev == NULL) {
                hashTable[index] = node->next;
            } else {
                prev->next = node->next;
            }
            free(node);
            return 1; // Word found and deleted
        }
        prev = node;
        node = node->next;
    }

    return 0; // Word not found
}

void findMostFrequent(void) {
    char result[MAX_WORD_LENGTH] = {0};
    int maxCount = 0;

    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        for (Node* node = hashTable[i]; node != NULL; node = node->next) {
            if (node->count > maxCount || (node->count == maxCount && strcmp(node->word, result) < 0)) {
                strcpy(result, node->word);
                maxCount = node->count;
            }
        }
    }

    if (maxCount > 0) {
        printf("result: %s %d\n", result, maxCount);
    } else {
        printf("No valid words found.\n");
    }
}

int main(void) {
    char word[MAX_WORD_LENGTH];
    int lineCount = 0;

    while (fgets(word, MAX_WORD_LENGTH, stdin)) {
        word[strcspn(word, "\n")] = '\0'; // Remove newline character
        lineCount++;

        if (isPrime(lineCount)) {
            printf("trying to delete %s: ", word);
            if (delete(word)) {
                printf("deleted\n");
            } else {
                printf("not found\n");
            }
        } else {
            insertOrUpdate(word); // This function now handles both finding and inserting
        }
    }

    findMostFrequent();

    // Cleanup code
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        Node* node = hashTable[i];
        while (node != NULL) {
            Node* temp = node;
            node = node->next;
            free(temp);
        }
    }
    return 0;
}

