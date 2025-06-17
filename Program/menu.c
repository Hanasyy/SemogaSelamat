/*
Nama: Hana Zahirah Syifa
NIM: 241524045
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "menu.h"
#include "pemesanan.h"
void dashboardPengguna(const User* user) {
    int pilihan;
    do {
        printf("\n=== Dashboard Pengguna (%s) ===\n", user->email);
        printf("1. Tambah Penumpang\n");
        printf("2. Tampilkan Penumpang\n");
        printf("3. Lakukan Pemesanan Tiket\n");
        printf("4. Lihat Riwayat Pemesanan\n");
        printf("0. Logout\n");
        printf("Pilih: ");
        scanf("%d", &pilihan);
        getchar(); // newline

        switch (pilihan) {
            case 1: {
                Penumpang p;
                printf("Nama Penumpang: ");
                fgets(p.nama, sizeof(p.nama), stdin);
                p.nama[strcspn(p.nama, "\n")] = '\0';

                printf("ID Type (0=KTP, 1=SIM): ");
                scanf("%d", (int*)&p.idType);

                printf("No ID: ");
                scanf("%s", p.noID);

                printf("Gender (0=Male, 1=Female): ");
                scanf("%d", (int*)&p.gender);

                tambahPenumpang(user->email, p);
                printf("Penumpang berhasil ditambahkan.\n");
                break;
            }

            case 2:
                tampilkanPenumpang(user->email);
                break;
                
            case 3:
                prosesPemesananUser(user, headPemesanan, daftarKereta, jumlahKereta);
                break;

            case 4:
                printAllPemesanan(*headPemesanan);
                break;

            case 0:
                printf("Logout berhasil.\n");
                break;

            default:
                printf("Pilihan tidak valid.\n");
        }

    } while (pilihan != 0);
}


void menuUtama() {
    UserNode* head = NULL;
    int pilihan;
    char email[100], password[100];
    User u;

    do {
        printf("\n=== Menu Utama ===\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("0. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &pilihan);
        getchar(); // newline

        switch (pilihan) {
            case 1:
                printf("Masukkan Nama: ");
                fgets(u.nama, sizeof(u.nama), stdin);
                u.nama[strcspn(u.nama, "\n")] = '\0';

                printf("Masukkan Email: ");
                scanf("%s", u.email);

				printf("Masukkan Password: ");
                scanf("%s", password);
                hashPassword(password, u.password);

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

                if (loginUser(headUser, email, password)) {
                    if (loadUserFromFolder(email, &u)) {
                        dashboardPengguna(&u, &listPemesanan, daftarKereta, jumlahKereta);
                        simpanPemesananKeFile(listPemesanan, "data_pemesanan.txt");
                    } else {
                        printf("Gagal memuat data user dari folder.\n");
                    }
                } else {
                    printf("Login gagal. Email atau password salah.\n");
                }
                break;

            case 0:
                printf("Keluar dari program...\n");
                freeListPemesanan(&listPemesanan);
                freeListUser(&headUser);
                break;		

            default:
                printf("Pilihan tidak valid.\n");
        }

    } while (pilihan != 0);
}

