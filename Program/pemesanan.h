/*
Nama: Hana Zahirah Syifa
NIM: 241524045
*/

#ifndef PEMESANAN_H
#define PEMESANAN_H

#define MAX 100
#define MAX_PENUMPANG 10

#include <stdlib.h>
#include "tipe_data_global.h"
#include "kereta.h"
#include "user.h"
#include "riwayat_pemesanan.h"
#include "kursi_kereta_UI.h"

typedef struct {
    char noIdentitas[MAX];  
    char nama[MAX];
    GenderType gender;
    IDType idType;
} DataPenumpang;

typedef struct {
    char emailPemesan[MAX]; 
    char stasiunAwal[MAX];
    char stasiunTujuan[MAX];
    Date hariBerangkat;
    int jumlahPenumpang;
    Kereta *kereta;
    DataPenumpang daftarPenumpang[MAX_PENUMPANG];
} Pemesanan;

typedef struct PemesananNode {
    Pemesanan info;
    struct PemesananNode* next;
} PemesananNode;

void prosesPemesananUser(const User* userLogin, PemesananNode** head, Kereta* kereta, int jumlahKereta, HistoryNode** historyTop);
void insertPemesanan(PemesananNode** head, Pemesanan data);
void printAllPemesanan(PemesananNode* head);
void freeListPemesanan(PemesananNode** head);
int validasiTanggal(Date d);
int validasiStasiun(const Kereta* k, const char* asal, const char* tujuan);
int cekKapasitasTersedia(PemesananNode* head, Kereta* kereta, Date tanggal, int jumlahDiminta);
void simpanPemesananKeFile(PemesananNode* head, const char* filename);
void loadPemesananDariFile(PemesananNode** head, const char* filename, Kereta daftarKereta[], int jumlahKereta);



#endif


