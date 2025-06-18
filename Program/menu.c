/*
Nama: Annisa Reida Kamilaini
NIM: 241524032
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#define clearScreen() system("cls")
#else
#define clearScreen() system("clear")
#endif

#include "menu.h"
#include "tiket.h"
#include "RIWAYAT_PEMESANAN.h"
#include "terminal_util.h"

HistoryNode* historyTop = NULL;

void dashboardUser(const User* user, PemesananNode** headPemesanan, Kereta daftarKereta[], int jumlahKereta) {
    int pilih;
    do {
        clearScreen();
        setWarna(WARNA_KUNING);
        printf("\n===== Dashboard Pengguna =====\n");
        setWarna(WARNA_HIJAU);
        printf("Selamat datang, %s\n", user->nama);
        resetWarna();
        printf("1. Pesan Tiket Kereta\n");
        printf("2. Lihat Riwayat Pemesanan\n");
        printf("3. Tambah Penumpang\n");
        printf("4. Daftar Penumpang Saya\n");
        printf("5. Batalkan Tiket\n");
        printf("0. Logout\n");
        printf("Pilih menu: ");
        scanf("%d", &pilih); getchar();

        clearScreen();
        switch (pilih) {
            case 1: {
                char namaKereta[100];

                FILE *f = fopen("D:\\GitHub\\SemogaSelamat\\Program\\listKereta.txt", "r");
                if (!f) {
                    setWarna(WARNA_MERAH);
                    printf("Gagal membuka list kereta.\n");
                    resetWarna();
                    break;
                }

                setWarna(WARNA_BIRU);
                printf("\n=== Daftar Kereta Tersedia ===\n");
                resetWarna();
                while (fgets(namaKereta, sizeof(namaKereta), f)) {
                    namaKereta[strcspn(namaKereta, "\n")] = 0;
                    printf("%d. %s\n", jumlahKereta + 1, namaKereta);
                    readKeretaToList(namaKereta, &daftarKereta[jumlahKereta]);
                    jumlahKereta++;
                }
                fclose(f);

                if (jumlahKereta == 0) {
                    setWarna(WARNA_MERAH);
                    printf("Tidak ada kereta tersedia.\n");
                    resetWarna();
                    break;
                }

                int pilihKereta;
                do {
                    printf("Pilih kereta (1 - %d): ", jumlahKereta);
                    scanf("%d", &pilihKereta); getchar();
                } while (pilihKereta < 1 || pilihKereta > jumlahKereta);

                Kereta keretaDipilih = daftarKereta[pilihKereta - 1];

                printf("\n--- Jadwal Kereta %s ---\n", keretaDipilih.namaKereta);
                printf("Rute Utama:\n");
                printRuteKereta(keretaDipilih.utama);
                printf("Rute Reverse:\n");
                printRuteKereta(keretaDipilih.reverse);

                prosesPemesananUser(user, headPemesanan, &keretaDipilih, 1, &historyTop);
                simpanPemesananKeFile(*headPemesanan, "data_pemesanan.txt");
                printf("\nTekan Enter untuk kembali ke dashboard..."); getchar();
                break;
            }
            case 2:
                printHistory(historyTop);
                printf("\nTekan Enter untuk kembali ke dashboard..."); getchar();
                break;
			case 3: {
			    char namaKereta[100];

                FILE *f = fopen("D:\\GitHub\\SemogaSelamat\\Program\\listKereta.txt", "r");
                if (!f) {
                    setWarna(WARNA_MERAH);
                    printf("Gagal membuka list kereta.\n");
                    resetWarna();
                    break;
                }

                setWarna(WARNA_BIRU);
                printf("\n=== Daftar Kereta Tersedia ===\n");
                resetWarna();
                while (fgets(namaKereta, sizeof(namaKereta), f)) {
                    namaKereta[strcspn(namaKereta, "\n")] = 0;
                    printf("%d. %s\n", jumlahKereta + 1, namaKereta);
                    readKeretaToList(namaKereta, &daftarKereta[jumlahKereta]);
                    jumlahKereta++;
                }
                fclose(f);

                if (jumlahKereta == 0) {
                    setWarna(WARNA_MERAH);
                    printf("Tidak ada kereta tersedia.\n");
                    resetWarna();
                    break;
                }

                int pilihKereta;
                do {
                    printf("Pilih kereta (1 - %d): ", jumlahKereta);
                    scanf("%d", &pilihKereta); getchar();
                } while (pilihKereta < 1 || pilihKereta > jumlahKereta);

                Kereta keretaDipilih = daftarKereta[pilihKereta - 1];

                printf("\n--- Jadwal Kereta %s ---\n", keretaDipilih.namaKereta);
                printf("Rute Utama:\n");
                printRuteKereta(keretaDipilih.utama);
                printf("Rute Reverse:\n");
                printRuteKereta(keretaDipilih.reverse);

                prosesPemesananUser(user, headPemesanan, &keretaDipilih, 1, &historyTop);
                simpanPemesananKeFile(*headPemesanan, "data_pemesanan.txt");
                printf("\nTekan Enter untuk kembali ke dashboard..."); getchar();
                break;
			}

            case 4:
                tampilkanPenumpang(user->email);
                printf("\nTekan Enter untuk kembali ke dashboard..."); getchar();
                break;
            case 5: {
                char namaKereta[MAX];
                printf("Masukkan nama kereta yang ingin dibatalkan: ");
                scanf(" %[^ ]", namaKereta);
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
                printf("\nTekan Enter untuk kembali ke dashboard..."); getchar(); getchar();
                break;
            }
            case 0:
                printf("Logout berhasil.\n");
                break;
            default:
                printf("Menu tidak tersedia.\n");
                printf("\nTekan Enter untuk kembali ke dashboard..."); getchar();
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
        clearScreen();
        setWarna(WARNA_BIRU);
        printf("\n===== Selamat Datang di Aplikasi Naik Kereta =====\n");
        resetWarna();
        printf("1. Login\n");
        printf("2. Registrasi\n");
        printf("0. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilih); getchar();

        clearScreen();
        switch (pilih) {
            case 1:
                formLogin(&userAktif);
                if (userAktif.email[0] != '\0') {
                    dashboardUser(&userAktif, &daftarPemesanan, daftarKereta, jumlahKereta);
                    simpanPemesananKeFile(daftarPemesanan, "data_pemesanan.txt");
                }
                break;
            case 2:
                formRegistrasi();
                printf("\nTekan Enter untuk kembali ke menu utama..."); getchar();
                break;
            case 0:
                printf("Terima kasih telah menggunakan Naik Kereta!\n");
                break;
            default:
                printf("Pilihan tidak tersedia.\n");
                printf("\nTekan Enter untuk kembali ke menu utama..."); getchar();
        }
    } while (pilih != 0);

    freeListPemesanan(&daftarPemesanan);
    freeListUser(&headUser);
}
