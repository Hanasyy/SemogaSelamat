#include "user.h"
#include <ctype.h>

void formRegistrasi() {
    User u;
    char konfirmasi[MAX], hashed[MAX];

    printf("\n=== REGISTRASI USER BARU ===\n");

    do {
        printf("Nama lengkap       : "); fgets(u.nama, MAX, stdin); u.nama[strcspn(u.nama, "\n")] = 0;
    } while (strlen(u.nama) == 0);

    do {
        printf("Email              : "); fgets(u.email, MAX, stdin); u.email[strcspn(u.email, "\n")] = 0;
        if (!validasiEmail(u.email)) printf("Format email tidak valid.\n");
    } while (!validasiEmail(u.email));

    do {
        printf("No Handphone       : "); fgets(u.noHP, MAX, stdin); u.noHP[strcspn(u.noHP, "\n")] = 0;
    } while (strlen(u.noHP) == 0);

    printf("Tipe Identitas (1=KTP, 2=Paspor): "); scanf("%d", (int*)&u.idType); getchar();
    do {
        printf("Nomor Identitas    : "); fgets(u.noID, MAX, stdin); u.noID[strcspn(u.noID, "\n")] = 0;
    } while (strlen(u.noID) == 0);

    printf("Jenis Kelamin (1=Laki-laki, 2=Perempuan): "); scanf("%d", (int*)&u.gender); getchar();

    do {
        printf("Password (min 8 char, kombinasi huruf & angka): "); fgets(u.password, MAX, stdin); u.password[strcspn(u.password, "\n")] = 0;
        printf("Konfirmasi Password : "); fgets(konfirmasi, MAX, stdin); konfirmasi[strcspn(konfirmasi, "\n")] = 0;
        if (strcmp(u.password, konfirmasi) != 0) printf("Password tidak cocok. Ulangi.\n");
        else if (!validasiPassword(u.password)) printf("Password tidak valid.\n");
    } while (strcmp(u.password, konfirmasi) != 0 || !validasiPassword(u.password));

    char hashedPassword[65];
    hashPassword(u.password, hashedPassword);
    strcpy(u.password, hashedPassword);
    saveUserToFolder(u);
    printf("Registrasi berhasil. Silakan login.\n");
}

void formLogin(User* hasilLogin) {
    char email[MAX], pass[MAX], hashed[MAX];
    printf("\n=== LOGIN USER ===\n");
    printf("Email    : "); fgets(email, MAX, stdin); email[strcspn(email, "\n")] = 0;
    printf("Password : "); fgets(pass, MAX, stdin); pass[strcspn(pass, "\n")] = 0;

    hashPassword(pass, hashed);
    if (loadUserFromFolder(email, hasilLogin) == 1 && strcmp(hasilLogin->password, hashed) == 0) {
        printf("Login berhasil. Selamat datang, %s!\n", hasilLogin->nama);
    } else {
        printf("Login gagal. Email atau password salah.\n");
        hasilLogin->email[0] = '\0';
    }
}

void hashPassword(const char* password, char* hashed) {
    uint8_t hash[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, (const uint8_t*)password, strlen(password));
    sha256_final(&ctx, hash);

    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        sprintf(hashed + (i * 2), "%02x", hash[i]);
    }
    hashed[64] = '\0';
}

int validasiEmail(const char* email) {
    return strchr(email, '@') && strchr(email, '.');
}

int validasiPassword(const char* pass) {
    if (strlen(pass) < 8) return 0;
    int huruf = 0, angka = 0;
    for (int i = 0; pass[i]; i++) {
        if (isalpha(pass[i])) huruf = 1;
        if (isdigit(pass[i])) angka = 1;
    }
    return huruf && angka;
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

void buatFolderUser(const char* email) {
    char folder[256];
    snprintf(folder, sizeof(folder), "users/%s", email);
    _mkdir("users");
    _mkdir(folder);
}

void saveUserToFolder(User u) {
    buatFolderUser(u.email);
    char path[300];
    snprintf(path, sizeof(path), "users/%s/profile.txt", u.email);
    FILE* f = fopen(path, "w");
    if (!f) {
        perror("Gagal membuka file user profile");
        return;
    }
    fprintf(f, "%s\n%s\n%d\n%s\n%s\n%d\n%s\n",
            u.nama, u.email, u.idType, u.noID, u.noHP, u.gender, u.password);
    fclose(f);
}

int loadUserFromFolder(const char* email, User* u) {
    char path[300];
    snprintf(path, sizeof(path), "users/%s/profile.txt", email);
    FILE* f = fopen(path, "r");
    if (!f) return 0;

    fgets(u->nama, MAX, f); u->nama[strcspn(u->nama, "\n")] = 0;
    fgets(u->email, MAX, f); u->email[strcspn(u->email, "\n")] = 0;
    fscanf(f, "%d\n", (int*)&u->idType);
    fgets(u->noID, MAX, f); u->noID[strcspn(u->noID, "\n")] = 0;
    fgets(u->noHP, MAX, f); u->noHP[strcspn(u->noHP, "\n")] = 0;
    fscanf(f, "%d\n", (int*)&u->gender);
    fgets(u->password, 65, f); u->password[strcspn(u->password, "\n")] = 0;

    fclose(f);
    return 1;
}

void tambahPenumpang(const char* email, Penumpang p) {
    char path[300];
    snprintf(path, sizeof(path), "users/%s/penumpang.txt", email);
    FILE* f = fopen(path, "a");
    if (!f) {
        perror("Gagal membuka file penumpang");
        return;
    }
    fprintf(f, "%s\n%d\n%s\n%d\n---\n",
            p.nama, p.idType, p.noID, p.gender);
    fclose(f);
}

PenumpangNode* loadPenumpangDariFile(const char* email) {
    char path[300];
    snprintf(path, sizeof(path), "users/%s/penumpang.txt", email);
    FILE* f = fopen(path, "r");
    if (!f) return NULL;

    PenumpangNode* head = NULL;
    PenumpangNode* tail = NULL;
    Penumpang p;
    char line[256];

    while (fgets(line, sizeof(line), f)) {
        if (strcmp(line, "---\n") == 0) {
            PenumpangNode* newNode = (PenumpangNode*)malloc(sizeof(PenumpangNode));
            newNode->info = p;
            newNode->next = NULL;
            if (!head) head = tail = newNode;
            else { tail->next = newNode; tail = newNode; }
        } else {
            static int stage = 0;
            switch (stage) {
                case 0: strcpy(p.nama, strtok(line, "\n")); break;
                case 1: p.idType = atoi(line); break;
                case 2: strcpy(p.noID, strtok(line, "\n")); break;
                case 3: p.gender = atoi(line); break;
            }
            stage = (stage + 1) % 4;
        }
    }
    fclose(f);
    return head;
}

void tampilkanPenumpang(const char* email) {
    PenumpangNode* head = loadPenumpangDariFile(email);
    if (!head) {
        printf("Belum ada penumpang yang disimpan.\n");
        return;
    }
    PenumpangNode* curr = head;
    int i = 1;
    while (curr) {
        printf("%d. Nama: %s, ID: %s, Gender: %s\n", i++,
               curr->info.nama, curr->info.noID,
               curr->info.gender == LAKI_LAKI ? "Laki-laki" : "Perempuan");
        curr = curr->next;
    }
    freePenumpangList(&head);
}

void simpanPenumpangKeFile(const char* path, PenumpangNode* head) {
    FILE* f = fopen(path, "w");
    if (!f) return;
    while (head) {
        fprintf(f, "%s\n%d\n%s\n%d\n---\n",
                head->info.nama, head->info.idType, head->info.noID, head->info.gender);
        head = head->next;
    }
    fclose(f);
}

void freePenumpangList(PenumpangNode** head) {
    PenumpangNode* curr = *head;
    while (curr) {
        PenumpangNode* temp = curr;
        curr = curr->next;
        free(temp);
    }
    *head = NULL;
}

void freeListUser(UserNode** head) {
    UserNode* curr = *head;
    while (curr) {
        UserNode* temp = curr;
        curr = curr->next;
        free(temp);
    }
    *head = NULL;
}
