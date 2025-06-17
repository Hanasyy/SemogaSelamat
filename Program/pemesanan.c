/*
Nama: Hana Zahirah Syifa
NIM: 241524045
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pemesanan.h"
#include "kursi_kereta_UI.h"
#include "manajemen_antrian.h"
#include "tiket.h"


void insertPemesanan(PemesananNode** head, Pemesanan data) {
    PemesananNode* newPemesananNode = (PemesananNode*)malloc(sizeof(PemesananNode));
    newPemesananNode->info = data;
    newPemesananNode->next = *head;
    *head = newPemesananNode;
}

void printAllPemesanan(PemesananNode* head) {
    PemesananNode* temp = head;
    int i = 1;

    while (temp != NULL) {
        printf("\nPemesanan ke-%d\n", i++);
        printf("Email Pemesan: %s\n", temp->info.emailPemesan);
        printf("Rute: %s -> %s\n", temp->info.stasiunAwal, temp->info.stasiunTujuan);
        printf("Tanggal Berangkat: %02d-%02d-%04d\n",
               temp->info.hariBerangkat.hari, temp->info.hariBerangkat.bulan, temp->info.hariBerangkat.tahun);
        printf("Kereta: %s\n", temp->info.kereta->namaKereta);
        printf("Jumlah Penumpang: %d\n", temp->info.jumlahPenumpang);

        int j = 0;
        while (j < temp->info.jumlahPenumpang) {
            DataPenumpang* p = &temp->info.daftarPenumpang[j];
            printf(" - %s (%s, %s)\n", p->nama,
                   p->gender == LAKI_LAKI ? "Laki-laki" : "Perempuan",
                   p->idType == KTP ? "KTP" : "Paspor");
            j++;
        }

        temp = temp->next;
    }
}

void freeListPemesanan(PemesananNode** head) {
    while (*head != NULL) {
        PemesananNode* temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

int validasiTanggal(Date d) {
    if (d.hari < 1 || d.hari > 31 || d.bulan < 1 || d.bulan > 12 || d.tahun < 2024) return 0;
    return 1;
}

int validasiStasiun(const Kereta* k, const char* asal, const char* tujuan) {
    int foundAsal = 0;
    int foundTujuan = 0;

    nodeStasiun* current = k->utama.stasiun;
    while (current != NULL) {
        if (strcmp(current->stasiun.namaStasiun, asal) == 0)
            foundAsal = 1;
        if (strcmp(current->stasiun.namaStasiun, tujuan) == 0)
            foundTujuan = 1;

        current = current->nextStasiun;
    }

    return foundAsal && foundTujuan;
}

int cekKapasitasTersedia(PemesananNode* head, Kereta* kereta, Date tanggal, int jumlahDiminta) {
    int totalTerpesan = 0;
    PemesananNode* temp = head;

    while (temp != NULL) {
        if (temp->info.kereta == kereta &&
            temp->info.hariBerangkat.hari == tanggal.hari &&
            temp->info.hariBerangkat.bulan == tanggal.bulan &&
            temp->info.hariBerangkat.tahun == tanggal.tahun
			) {
            totalTerpesan += temp->info.jumlahPenumpang;
        }
        temp = temp->next;
    }

    return (kereta->kapasitas - totalTerpesan) >= jumlahDiminta;
}

void simpanPemesananKeFile(PemesananNode* head, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) return;

    PemesananNode* temp = head;
    while (temp != NULL) {
        Pemesanan* p = &temp->info;
        fprintf(f, "%s;%s;%s;%d-%d-%d;%s;%d\n",
                p->emailPemesan, p->stasiunAwal, p->stasiunTujuan,
                p->hariBerangkat.hari, p->hariBerangkat.bulan, p->hariBerangkat.tahun,
                p->kereta->namaKereta, p->jumlahPenumpang);

        int i = 0;
        while (i < p->jumlahPenumpang) {
            fprintf(f, "%s|%s|%d|%d\n", p->daftarPenumpang[i].noIdentitas,
                    p->daftarPenumpang[i].nama,
                    (int)p->daftarPenumpang[i].gender,
                    (int)p->daftarPenumpang[i].idType);
            i++;
        }

        temp = temp->next;
    }

    fclose(f);
}

void loadPemesananDariFile(PemesananNode** head, const char* filename, Kereta daftarKereta[], int jumlahKereta) {
    FILE* f = fopen(filename, "r");
    if (!f) return;

    char baris[1024];
    while (fgets(baris, sizeof(baris), f)) {
        Pemesanan p;
        char namaKereta[MAX];
        sscanf(baris, "%[^;];%[^;];%[^;];%d-%d-%d;%[^;];%d",
               p.emailPemesan, p.stasiunAwal, p.stasiunTujuan,
               &p.hariBerangkat.hari, &p.hariBerangkat.bulan, &p.hariBerangkat.tahun,
               namaKereta, &p.jumlahPenumpang);

        p.kereta = NULL;
        int i = 0;
        while (i < jumlahKereta) {
            if (strcmp(daftarKereta[i].namaKereta, namaKereta) == 0) {
                p.kereta = &daftarKereta[i];
                break;
            }
            i++;
        }

        int j = 0;
        while (j < p.jumlahPenumpang && fgets(baris, sizeof(baris), f)) {
            sscanf(baris, "%[^|]|%[^|]|%d|%d",
                   p.daftarPenumpang[j].noIdentitas,
                   p.daftarPenumpang[j].nama,
                   (int*)&p.daftarPenumpang[j].gender,
                   (int*)&p.daftarPenumpang[j].idType);
            j++;
        }

        insertPemesanan(head, p);
    }

    fclose(f);
}

void prosesPemesananUser(const User* userLogin, PemesananNode** head, Kereta daftarKereta[], int jumlahKereta) {
    Pemesanan p;
    strcpy(p.emailPemesan, userLogin->email);

    printf("Masukkan stasiun awal: ");
    scanf(" %[^\n]", p.stasiunAwal);
    printf("Masukkan stasiun tujuan: ");
    scanf(" %[^\n]", p.stasiunTujuan);

    printf("Masukkan tanggal keberangkatan (dd mm yyyy): ");
    scanf("%d %d %d", &p.hariBerangkat.hari, &p.hariBerangkat.bulan, &p.hariBerangkat.tahun);

    if (!validasiTanggal(p.hariBerangkat)) {
        printf("Tanggal tidak valid!\n");
        return;
    }

    printListKereta();
    char namaKeretaDipilih[MAX];
    printf("Masukkan nama kereta yang dipilih: ");
    scanf(" %[^\n]", namaKeretaDipilih);

    int i, ditemukan = 0;
    for (i = 0; i < jumlahKereta; i++) {
        if (strcmp(namaKeretaDipilih, daftarKereta[i].namaKereta) == 0) {
            p.kereta = &daftarKereta[i];
            ditemukan = 1;
            break;
        }
    }

    if (!ditemukan) {
        printf("Kereta tidak ditemukan!\n");
        return;
    }

    printf("Jumlah penumpang: ");
    scanf("%d", &p.jumlahPenumpang);
    getchar();

    ATiket listTiket = NULL;

    for (int i = 0; i < p.jumlahPenumpang; i++) {
        printf("Data Penumpang ke-%d:\n", i + 1);
        printf("Nama: ");
        fgets(p.daftarPenumpang[i].nama, sizeof(p.daftarPenumpang[i].nama), stdin);
        p.daftarPenumpang[i].nama[strcspn(p.daftarPenumpang[i].nama, "\n")] = '\0';

        printf("Jenis ID (0=KTP, 1=SIM): ");
        scanf("%d", (int*)&p.daftarPenumpang[i].idType);
        printf("Nomor ID: ");
        scanf("%s", p.daftarPenumpang[i].noIdentitas);
        printf("Jenis Kelamin (0=Pria, 1=Wanita): ");
        scanf("%d", (int*)&p.daftarPenumpang[i].gender);
        getchar();

       
        int kursiTerisi = 0;
        tampilkanKursi(p.kereta->kapasitas, kursiTerisi);
        int noKursi;
        printf("Pilih nomor kursi untuk penumpang ini (0-%d): ", p.kereta->kapasitas - 1);
        scanf("%d", &noKursi);
        getchar();

     
        Tiket t;
        strcpy(t.nama, p.daftarPenumpang[i].nama);
        strcpy(t.stasiun_awal, p.stasiunAwal);
        strcpy(t.stasiun_tujuan, p.stasiunTujuan);
        t.harga = p.kereta->harga;
        t.tanggal = p.hariBerangkat;
        t.waktu = p.kereta->jamBerangkat;
        insertTiket(&listTiket, t);

    }

    insertPemesanan(head, p);
    printf("Pemesanan berhasil dan semua tiket telah dibuat.\n");
}


