#ifndef USER_ADT_H
#define USER_ADT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define MAX 100

typedef enum { KTP = 1, PASPOR } IDType;
typedef enum { LAKI_LAKI = 1, PEREMPUAN } GenderType;

typedef struct {
    char nama[MAX];
    char email[MAX];
    IDType idType;
    char noID[MAX];
    char noHP[MAX];
    GenderType gender;
    char password[65]; 
} User;

typedef struct Node {
    User info;
    struct Node* next;
} Node;

void hashPassword(const char* password, char* hashed);
void insertUser(Node** head, User u);
int loginUser(Node* head, const char* email, const char* rawPassword);
void saveUsersToText(Node* head, const char* filename);
void loadUsersFromText(Node** head, const char* filename);
void printUser(User u);
void freeList(Node** head);

#endif

