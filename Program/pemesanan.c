/*
Nama: Hana Zahirah Syifa
NIM: 241524045
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pemesanan.h"

#define MAX_KERETA 100

void insertPemesanan(PemesananNode** head, Pemesanan data) {
    PemesananNode* newPemesananNode = (PemesananNode*)malloc(sizeof(PemesananNode));
    newPemesananNode->info = data;
    newPemesananNode->next = *head;
    *head = newPemesananNode;
}

void printAllPemesanan(PemesananNode* head) {
    PemesananNode* curr = head;
    int i = 1;
    while (curr != NULL) {
        Pemesanan p = curr->info;
        printf("\nPemesanan %d:\n", i++);
        printf("Stasiun Asal      : %s\n", p.stasiunAwal);
        printf("Stasiun Tujuan    : %s\n", p.stasiunTujuan);
        printf("hari Berangkat : %02d-%02d-%d\n", p.hariBerangkat.hari, p.hariBerangkat.bulan, p.hariBerangkat.tahun);
        printf("Jumlah Penumpang  : %d\n", p.jumlahPenumpang);
        printf("Kereta            : %s (%s -> %s, %02d:%02d)\n",
               p.kereta.namaKereta,
               p.kereta.stasiunAwal,
               p.kereta.stasiunTujuan,
               p.kereta.jamBerangkat.jam,
               p.kereta.jamBerangkat.menit);
        curr = curr->next;
    }
}

void freeList(PemesananNode** head) {
    PemesananNode* temp;
    while (*head) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

int bacaKeretaDariFile(const char* filename, Kereta daftar[], int maxKereta) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Gagal membuka file %s\n", filename);
        return 0;
    }

    int count = 0;
    while (count < maxKereta) {
        int ret = fscanf(file, "%s %s %s %d %d",
            daftar[count].namaKereta,
            daftar[count].stasiunAwal,
            daftar[count].stasiunTujuan,
            &daftar[count].jamBerangkat.jam,
            &daftar[count].jamBerangkat.menit);
        if (ret != 5) {
            break;
        }
        count++;
    }

    fclose(file);
    return count;
}

void tampilkanDaftarKereta(Kereta daftar[], int jumlah) {
    printf("\n--- Daftar Kereta Tersedia ---\n");
    int i = 0;
    while (i < jumlah) {
        printf("%d. %s (%s -> %s) - %02d:%02d\n", i + 1,
               daftar[i].namaKereta,
               daftar[i].stasiunAwal,
               daftar[i].stasiunTujuan,
               daftar[i].jamBerangkat.jam,
               daftar[i].jamBerangkat.menit);
        i++;
    }
}

void tampilkanKeretaByRute(Kereta daftar[], int jumlah, const char* asal, const char* tujuan) {
    printf("\n--- Daftar Kereta dari %s ke %s ---\n", asal, tujuan);
    int found = 0;
    int i = 0;
    while (i < jumlah) {
        if (strcmp(daftar[i].stasiunAwal, asal) == 0 && strcmp(daftar[i].stasiunTujuan, tujuan) == 0) {
            printf("%d. %s - %02d:%02d\n", i + 1,
                   daftar[i].namaKereta,
                   daftar[i].jamBerangkat.jam,
                   daftar[i].jamBerangkat.menit);
            found = 1;
        }
        i++;
    }
    if (!found) {
        printf("Tidak ada kereta untuk rute ini.\n");
    }
}

Kereta pilihKeretaDariFile(Kereta daftar[], int jumlah, const char* asal, const char* tujuan) {
    int pilihan;
    Kereta filter[MAX_KERETA];
    int indeks[MAX_KERETA];
    int count = 0;
    int i = 0;

    while (i < jumlah) {
        if (strcmp(daftar[i].stasiunAwal, asal) == 0 && strcmp(daftar[i].stasiunTujuan, tujuan) == 0) {
            filter[count] = daftar[i];
            indeks[count] = i;
            count++;
        }
        i++;
    }

    if (count == 0) {
        printf("Tidak ada kereta tersedia untuk rute ini.\n");
        Kereta kosong = {"", "", "", {0, 0}};
        return kosong;
    }

    printf("\n--- Pilih Kereta ---\n");
    i = 0;
    while (i < count) {
        printf("%d. %s - %02d:%02d\n", i + 1,
               filter[i].namaKereta,
               filter[i].jamBerangkat.jam,
               filter[i].jamBerangkat.menit);
        i++;
    }

    printf("Pilih kereta (1-%d): ", count);
    scanf("%d", &pilihan);
    while (pilihan < 1 || pilihan > count) {
        printf("Pilihan tidak valid. Pilih lagi (1-%d): ", count);
        scanf("%d", &pilihan);
    }

    return filter[pilihan - 1];
}

