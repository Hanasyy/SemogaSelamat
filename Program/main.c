#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

int main() {
    Node* head = NULL;
    int pilihan;
    char email[100], password[100];
    User u;

    do {
        printf("\n=== Menu ===\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Tampilkan Penumpang\n");
        printf("0. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &pilihan);
        getchar(); // untuk menangani newline

        switch (pilihan) {
            case 1:
                printf("Masukkan Nama: ");
                fgets(u.nama, sizeof(u.nama), stdin);
                u.nama[strcspn(u.nama, "\n")] = '\0'; // hapus newline

                printf("Masukkan Email: ");
                scanf("%s", u.email);

                printf("Masukkan Password: ");
                scanf("%s", password);
                hashPassword(password, u.password); // hash sebelum disimpan

                printf("Masukkan ID Type (0=KTP, 1=SIM): ");
                scanf("%d", (int*)&u.idType);

                printf("Masukkan No ID: ");
                scanf("%s", u.noID);

                printf("Masukkan No HP: ");
                scanf("%s", u.noHP);

                printf("Masukkan Gender (0=Male, 1=Female): ");
                scanf("%d", (int*)&u.gender);

                insertUser(&head, u);
                saveUserToFolder(u);
                printf("Registrasi berhasil.\n");
                break;

            case 2:
                printf("Masukkan Email: ");
                scanf("%s", email);
                printf("Masukkan Password: ");
                scanf("%s", password);
                loginUser(head, email, password);
                break;

            case 3:
                printf("Masukkan Email untuk menampilkan penumpang: ");
                scanf("%s", email);
                tampilkanPenumpang(email);
                break;

            case 0:
                printf("Keluar...\n");
                break;

            default:
                printf("Pilihan tidak valid.\n");
        }

    } while (pilihan != 0);

    return 0;
}

