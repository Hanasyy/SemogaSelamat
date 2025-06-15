/*
Nama: Hana Zahirah Syifa
NIM: 241524045
*/

#ifndef PEMESANAN_H
#define PEMESANAN_H

#define MAX 100

#include "tipe_data_global.h"

typedef struct {
    char namaKereta[MAX];
    char stasiunAwal[MAX];
    char stasiunTujuan[MAX];
    Jam jamBerangkat;
} Kereta;

typedef struct {
    char stasiunAwal[MAX];
    char stasiunTujuan[MAX];
    Date hariBerangkat;
    int jumlahPenumpang;
    Kereta kereta;
} Pemesanan;

typedef struct PemesananNode {
    Pemesanan info;
    struct PemesananNode* next;
} PemesananNode;

void insertPemesanan(PemesananNode** head, Pemesanan data);
void printAllPemesanan(PemesananNode* head);
void freeList(PemesananNode** head);

int bacaKeretaDariFile(const char* filename, Kereta daftar[], int maxKereta);
void tampilkanDaftarKereta(Kereta daftar[], int jumlah);
void tampilkanKeretaByRute(Kereta daftar[], int jumlah, const char* asal, const char* tujuan);
Kereta pilihKeretaDariFile(Kereta daftar[], int jumlah, const char* asal, const char* tujuan);

#endif

