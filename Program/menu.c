#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "menu.h"
#include "pemesanan.h"
#include "kereta.h"
#include "RIWAYAT_PEMESANAN.h"

void dashboardUser(const User* user, PemesananNode** headPemesanan, Kereta daftarKereta[], int jumlahKereta) {
    int pilih;
    do {
        printf("\n===== KAI Access - Dashboard Pengguna =====\n");
        printf("Selamat datang, %s\n", user->nama);
        printf("1. Pesan Tiket Kereta\n");
        printf("2. Lihat Riwayat Pemesanan\n");
        printf("3. Tambah Penumpang\n");
        printf("4. Daftar Penumpang Saya\n");
        printf("5. Batalkan Tiket\n");
        printf("0. Logout\n");
        printf("Pilih menu: ");
        scanf("%d", &pilih); getchar();

        switch (pilih) {
            case 1:
                prosesPemesananUser(user, headPemesanan, daftarKereta, jumlahKereta);
                break;
            case 2:
                printAllPemesanan(*headPemesanan);
                break;
            case 3: {
                Penumpang p;
                printf("Nama Penumpang: ");
                fgets(p.nama, sizeof(p.nama), stdin);
                p.nama[strcspn(p.nama, "\n")] = '\0';
                printf("Tipe Identitas (0=KTP, 1=SIM): ");
                scanf("%d", (int*)&p.idType);
                printf("No Identitas: ");
                scanf("%s", p.noID);
                printf("Jenis Kelamin (0=Pria, 1=Wanita): ");
                scanf("%d", (int*)&p.gender);
                tambahPenumpang(user->email, p);
                break;
            }
            case 4:
                tampilkanPenumpang(user->email);
                break;
            case 5: {
                char namaKereta[MAX];
                printf("Masukkan nama kereta yang ingin dibatalkan: ");
                scanf(" %[^\n]", namaKereta);
                PemesananNode *curr = *headPemesanan, *prev = NULL;
                int found = 0;
                while (curr != NULL) {
                    if (strcmp(curr->info.emailPemesan, user->email) == 0 &&
                        strcmp(curr->info.kereta->namaKereta, namaKereta) == 0) {
                        found = 1;
                        if (prev == NULL) {
                            *headPemesanan = curr->next;
                        } else {
                            prev->next = curr->next;
                        }
                        free(curr);
                        printf("Tiket berhasil dibatalkan.\n");
                        break;
                    }
                    prev = curr;
                    curr = curr->next;
                }
                if (!found) {
                    printf("Tiket tidak ditemukan untuk dibatalkan.\n");
                }
                break;
            }
            case 0:
                printf("Logout berhasil.\n");
                break;
            default:
                printf("Menu tidak tersedia.\n");
        }
    } while (pilih != 0);
}

void menuUtama() {
    UserNode* headUser = NULL;
    PemesananNode* daftarPemesanan = NULL;
    Kereta daftarKereta[MAX];
    int jumlahKereta = 0;

    readFileListKereta(daftarKereta, &jumlahKereta);
    loadPemesananDariFile(&daftarPemesanan, "data_pemesanan.txt", daftarKereta, jumlahKereta);

    int pilih;
    char email[100], password[100];
    User userAktif;

    do {
        printf("\n===== Selamat Datang di Aplikasi KAI Access =====\n");
        printf("1. Login\n");
        printf("2. Registrasi\n");
        printf("0. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilih); getchar();

        switch (pilih) {
            case 1:
                printf("Email: ");
                scanf("%s", email);
                printf("Password: ");
                scanf("%s", password);
                if (loginUser(headUser, email, password)) {
                    if (loadUserFromFolder(email, &userAktif)) {
                        dashboardUser(&userAktif, &daftarPemesanan, daftarKereta, jumlahKereta);
                        simpanPemesananKeFile(daftarPemesanan, "data_pemesanan.txt");
                    }
                } else {
                    printf("Login gagal!\n");
                }
                break;
            case 2:
                printf("Nama Lengkap: ");
                fgets(userAktif.nama, sizeof(userAktif.nama), stdin);
                userAktif.nama[strcspn(userAktif.nama, "\n")] = '\0';
                printf("Email: ");
                scanf("%s", userAktif.email);
                printf("Password: ");
                scanf("%s", password);
                hashPassword(password, userAktif.password);
                printf("Tipe Identitas (0=KTP, 1=Paspor): ");
                scanf("%d", (int*)&userAktif.idType);
                printf("Nomor Identitas: ");
                scanf("%s", userAktif.noID);
                printf("No HP: ");
                scanf("%s", userAktif.noHP);
                printf("Jenis Kelamin (0=Laki-laki, 1=Perempuan): ");
                scanf("%d", (int*)&userAktif.gender);
                insertUser(&headUser, userAktif);
                saveUserToFolder(userAktif);
                break;
            case 0:
                printf("Terima kasih telah menggunakan KAI Access!\n");
                break;
            default:
                printf("Pilihan tidak tersedia.\n");
        }
    } while (pilih != 0);

    freeListPemesanan(&daftarPemesanan);
    freeListUser(&headUser);
}

