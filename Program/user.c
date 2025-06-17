/*
Nama: Hana Zahirah Syifa
NIM: 241524045
*/

#include "user.h"


void hashPassword(const char* password, char* hashed) {
    uint8_t hash[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, (const uint8_t*)password, strlen(password));
    sha256_final(&ctx, hash);

    int i = 0;
    while (i < SHA256_BLOCK_SIZE) {
        sprintf(hashed + (i * 2), "%02x", hash[i]);
        i++;
    }
    hashed[64] = '\0';
}

void insertUser(UserNode** head, User u) {
    UserNode* newNode = (UserNode*) malloc(sizeof(UserNode));
    if (!newNode) {
        perror("Gagal alokasi memori user");
        exit(1);
    }
    newNode->info = u;
    newNode->next = NULL;
    if (*head == NULL) {
        *head = newNode;
    } else {
        UserNode* temp = *head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }
}

static UserNode* cariUserByEmail(UserNode* head, const char* email) {
    while (head != NULL) {
        if (strcmp(head->info.email, email) == 0) return head;
        head = head->next;
    }
    return NULL;
}

int loginUser(UserNode* headUser, const char* email, const char* rawPassword) {
    char hashed[65];
    hashPassword(rawPassword, hashed);
    UserNode* userNode = cariUserByEmail(headUser, email);
    if (userNode != NULL && strcmp(userNode->info.password, hashed) == 0) {
        printf("Login berhasil. Selamat datang, %s!\n", userNode->info.nama);
        return 1;
    }
    printf("Email atau password salah.\n");
    return 0;
}

void gantiKarakter(char* str, char dari, char ke) {
    int i = 0;
    while (str[i]) {
        if (str[i] == dari) str[i] = ke;
        i++;
    }
}

void buatFolderUser(const char* email) {
    char folder[256];
    snprintf(folder, sizeof(folder), "users\\%s", email);
    _mkdir("users");          
    _mkdir(folder);
}

void saveUserToFolder(User u) {
    buatFolderUser(u.email);
    char path[300];
    snprintf(path, sizeof(path), "users\\%s\\profile.txt", u.email);
    FILE* f = fopen(path, "w");
    if (!f) {
        perror("Gagal membuka file user profile");
        return;
    }
    fprintf(f, "Nama: %s\n", u.nama);
    fprintf(f, "Email: %s\n", u.email);
    fprintf(f, "IDType: %d\n", u.idType);
    fprintf(f, "NoID: %s\n", u.noID);
    fprintf(f, "NoHP: %s\n", u.noHP);
    fprintf(f, "Gender: %d\n", u.gender);
    fprintf(f, "Password: %s\n", u.password);
    fclose(f);
}

int loadUserFromFolder(const char* email, User* u) {
    char path[300];
    snprintf(path, sizeof(path), "users\\%s\\profile.txt", email);
    FILE* f = fopen(path, "r");
    if (!f) return 0;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "Nama:", 5) == 0) {
            sscanf(line + 6, " %[^\n]", u->nama);
        } else if (strncmp(line, "Email:", 6) == 0) {
            sscanf(line + 7, "%s", u->email);
        } else if (strncmp(line, "IDType:", 7) == 0) {
            int t; sscanf(line + 8, "%d", &t); u->idType = (IDType)t;
        } else if (strncmp(line, "NoID:", 5) == 0) {
            sscanf(line + 6, "%s", u->noID);
        } else if (strncmp(line, "NoHP:", 5) == 0) {
            sscanf(line + 6, "%s", u->noHP);
        } else if (strncmp(line, "Gender:", 7) == 0) {
            int g; sscanf(line + 8, "%d", &g); u->gender = (GenderType)g;
        } else if (strncmp(line, "Password:", 9) == 0) {
            sscanf(line + 10, "%s", u->password);
        }
    }
    fclose(f);
    return 1;
}

void tambahPenumpang(const char* email, Penumpang p) {
    char folder[256];
    snprintf(folder, sizeof(folder), "users\\%s", email);
    buatFolderUser(email);
    char path[300];
    snprintf(path, sizeof(path), "%s\\penumpang.txt", folder);

    FILE* f = fopen(path, "a");
    if (!f) {
        perror("Gagal membuka file penumpang untuk ditambah");
        return;
    }
    fprintf(f, "Nama: %s\n", p.nama);
    fprintf(f, "IDType: %d\n", p.idType);
    fprintf(f, "NoID: %s\n", p.noID);
    fprintf(f, "Gender: %d\n", p.gender);
    fprintf(f, "---\n");
    fclose(f);
}

void simpanPenumpangKeFile(const char* path, PenumpangNode* head) {
    FILE* f = fopen(path, "w");
    if (!f) {
        perror("Gagal membuka file penumpang untuk simpan ulang");
        return;
    }
    PenumpangNode* cur = head;
    while (cur) {
        fprintf(f, "Nama: %s\n", cur->info.nama);
        fprintf(f, "IDType: %d\n", cur->info.idType);
        fprintf(f, "NoID: %s\n", cur->info.noID);
        fprintf(f, "Gender: %d\n", cur->info.gender);
        fprintf(f, "---\n");
        cur = cur->next;
    }
    fclose(f);
}

PenumpangNode* loadPenumpangDariFile(const char* email) {
    char folder[256];
    snprintf(folder, sizeof(folder), "users\\%s", email);
    char path[300];
    snprintf(path, sizeof(path), "%s\\penumpang.txt", folder);

    FILE* f = fopen(path, "r");
    if (!f) return NULL;

    PenumpangNode* head = NULL;
    PenumpangNode* tail = NULL;

    Penumpang p;
    char line[256];
    int fieldCounter = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "Nama:", 5) == 0) {
            sscanf(line + 6, " %[^\n]", p.nama);
            fieldCounter = 1;
        } else if (strncmp(line, "IDType:", 7) == 0) {
            int t; sscanf(line + 8, "%d", &t); p.idType = (IDType)t;
            fieldCounter = 2;
        } else if (strncmp(line, "NoID:", 5) == 0) {
            sscanf(line + 6, "%s", p.noID);
            fieldCounter = 3;
        } else if (strncmp(line, "Gender:", 7) == 0) {
            int g; sscanf(line + 8, "%d", &g); p.gender = (GenderType)g;
            fieldCounter = 4;
        } else if (strncmp(line, "---", 3) == 0) {
            if (fieldCounter == 4) {
                PenumpangNode* node = (PenumpangNode*)malloc(sizeof(PenumpangNode));
                if (!node) {
                    perror("Gagal alokasi memori penumpang");
                    fclose(f);
                    return head;
                }
                node->info = p;
                node->next = NULL;
                if (!head) {
                    head = tail = node;
                } else {
                    tail->next = node;
                    tail = node;
                }
                fieldCounter = 0;
            }
        }
    }
    fclose(f);
    return head;
}

void freePenumpangList(PenumpangNode** head) {
    PenumpangNode* cur = *head;
    while (cur) {
        PenumpangNode* next = cur->next;
        free(cur);
        cur = next;
    }
    *head = NULL;
}

void tampilkanPenumpang(const char* email) {
    PenumpangNode* head = loadPenumpangDariFile(email);
    if (!head) {
        printf("Belum ada penumpang tambahan.\n");
        return;
    }
    PenumpangNode* cur = head;
    int idx = 1;
    while (cur) {
        printf("%d. Nama: %s, IDType: %d, NoID: %s, Gender: %d\n",
            idx++, cur->info.nama, cur->info.idType, cur->info.noID, cur->info.gender);
        cur = cur->next;
    }
    freePenumpangList(&head);
}


void freeListUser(UserNode** head) {
    UserNode* current = *head;
    while (current != NULL) {
        UserNode* temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

