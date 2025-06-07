/*
Nama: Hana Zahirah Syifa
NIM: 241524045
*/

#ifndef PEMESANAN_H
#define PEMESANAN_H

#define MAX 100

typedef struct {
    int jam;
    int menit;
} Jam;

typedef struct {
    int hari;
    int bulan;
    int tahun;
} Date;

typedef struct {
    char namaKereta[MAX];
    char stasiunAwal[MAX];
    char stasiunTujuan[MAX];
    Jam jamBerangkat;
} Kereta;

typedef struct {
    char stasiunAwal[MAX];
    char stasiunTujuan[MAX];
    Date tanggalBerangkat;
    int jumlahPenumpang;
    Kereta kereta;
} Pemesanan;

typedef struct Node {
    Pemesanan info;
    struct Node* next;
} Node;

void insertPemesanan(Node** head, Pemesanan data);
void printAllPemesanan(Node* head);
void freeList(Node** head);

int bacaKeretaDariFile(const char* filename, Kereta daftar[], int maxKereta);
void tampilkanDaftarKereta(Kereta daftar[], int jumlah);
void tampilkanKeretaByRute(Kereta daftar[], int jumlah, const char* asal, const char* tujuan);
Kereta pilihKeretaDariFile(Kereta daftar[], int jumlah, const char* asal, const char* tujuan);

#endif

