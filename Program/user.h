/*
Nama: Hana Zahirah Syifa
NIM: 241524045
*/

#ifndef USER_H
#define USER_H
#include "sha256.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <direct.h>

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

typedef struct {
    char nama[MAX];
    IDType idType;
    char noID[MAX];
    GenderType gender;
} Penumpang;

typedef struct PenumpangNode {
    Penumpang info;
    struct PenumpangNode* next;
} PenumpangNode;

typedef struct UserNode {
    User info;
    struct UserNode* next;
} UserNode;


void hashPassword(const char* password, char* hashed);
void insertUser(UserNode** head, User u);
int loginUser(UserNode* headUser, const char* email, const char* rawPassword);
void saveUserToFolder(User u);
int loadUserFromFolder(const char* email, User* u);
void buatFolderUser(const char* email);
void gantiKarakter(char* str, char dari, char ke);
void tambahPenumpang(const char* email, Penumpang p);
void tampilkanPenumpang(const char* email);
void simpanPenumpangKeFile(const char* path, PenumpangNode* head);
PenumpangNode* loadPenumpangDariFile(const char* email);
void freePenumpangList(PenumpangNode** head);
static UserNode* cariUserByEmail(UserNode* head, const char* email);



#endif

