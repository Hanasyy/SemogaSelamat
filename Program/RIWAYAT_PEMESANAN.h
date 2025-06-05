/*
Nama: Annisa Reida Kamilaini
NIM: 241524032
*/

#ifndef RIWAYAT_PEMESANAN_H
#define RIWAYAT_PEMESANAN_H

// Struct untuk waktu (nyesuain di laporan buat percobaan)
typedef struct {
    int hour;
    int min;
} Time;

// Struct untuk tanggal (nyesuain di laporan buat percobaan)
typedef struct {
    int tanggal;
    int bulan;
    int tahun;
} Date;

// Struct untuk data tiket (nyesuain di laporan buat percobaan)
typedef struct {
    char nama[100];
    char stasiun_awal[100];
    char stasiun_tujuan[100];
    float harga;
    Date tanggal;
    Time waktu;
} Tiket;

// Stack node untuk riwayat pemesanan
typedef struct HistoryNode {
    Tiket tiket;
    struct HistoryNode* next;
} HistoryNode;

void pushHistory(HistoryNode** top, Tiket data);
void popHistory(HistoryNode** top);
void printHistory(HistoryNode* top);

const char* getNamaBulan(int bulan);

#endif