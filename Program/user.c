#include "user.h"

void hashPassword(const char* password, char* hashed) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)password, strlen(password), hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hashed + (i * 2), "%02x", hash[i]);
    }
    hashed[64] = '\0';
}

void insertUser(Node** head, User u) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->info = u;
    newNode->next = NULL;
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* temp = *head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }
}

int loginUser(Node* head, const char* email, const char* rawPassword) {
    char hashed[65];
    hashPassword(rawPassword, hashed);
    while (head != NULL) {
        if (strcmp(head->info.email, email) == 0 &&
            strcmp(head->info.password, hashed) == 0) {
            printf("Login berhasil. Selamat datang, %s!\n", head->info.nama);
            return 1;
        }
        head = head->next;
    }
    printf("Email atau password salah.\n");
    return 0;
}

void saveUsersToText(Node* head, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("Gagal membuka file");
        return;
    }
    while (head != NULL) {
        User u = head->info;
        fprintf(f, "Nama: %s\n", u.nama);
        fprintf(f, "Email: %s\n", u.email);
        fprintf(f, "ID Type: %d\n", u.idType);
        fprintf(f, "No ID: %s\n", u.noID);
        fprintf(f, "No HP: %s\n", u.noHP);
        fprintf(f, "Gender: %d\n", u.gender);
        fprintf(f, "Password: %s\n", u.password);
        fprintf(f, "---\n"); // Pemisah user
        head = head->next;
    }
    fclose(f);
}


void loadUsersFromText(Node** head, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return;

    User u;
    char line[256];

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "Nama:", 5) == 0) {
            sscanf(line + 6, "%[^\n]", u.nama);
        } else if (strncmp(line, "Email:", 6) == 0) {
            sscanf(line + 7, "%[^\n]", u.email);
        } else if (strncmp(line, "ID Type:", 8) == 0) {
            sscanf(line + 9, "%d", (int*)&u.idType);
        } else if (strncmp(line, "No ID:", 6) == 0) {
            sscanf(line + 7, "%[^\n]", u.noID);
        } else if (strncmp(line, "No HP:", 6) == 0) {
            sscanf(line + 7, "%[^\n]", u.noHP);
        } else if (strncmp(line, "Gender:", 7) == 0) {
            sscanf(line + 8, "%d", (int*)&u.gender);
        } else if (strncmp(line, "Password:", 9) == 0) {
            sscanf(line + 10, "%[^\n]", u.password);
        } else if (strncmp(line, "---", 3) == 0) {
            insertUser(head, u);
        }
    }
    fclose(f);
}

void printUser(User u) {
    printf("Nama: %s\n", u.nama);
    printf("Email: %s\n", u.email);
    printf("ID Type: %s\n", u.idType == KTP ? "KTP" : "PASPOR");
    printf("No ID: %s\n", u.noID);
    printf("No HP: %s\n", u.noHP);
    printf("Jenis Kelamin: %s\n", u.gender == LAKI_LAKI ? "Laki-laki" : "Perempuan");
    printf("Password (hashed): %s\n", u.password);
    printf("---\n");
}


void freeList(Node** head) {
    Node* current = *head;
    while (current) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

