/*
Nama: Annisa Reida Kamilaini
NIM: 241524032
*/

#ifndef MANAJEMEN_ANTRIAN_H
#define MANAJEMEN_ANTRIAN_H

#include "tipe_data_global.h"
#include "pemesanan.h"
#include "user.h"
#include "RIWAYAT_PEMESANAN.h"
#include "tiket.h"


typedef struct AksesAntrian {
    char pemesan[100];              // Nama user pemesan
    int jumlahTiket;               // Jumlah tiket yang dipesan user
    Jam waktuAkses;              // Waktu pemesanan
    Pemesanan pemesanan;          // Data lengkap pemesanan
    struct AksesAntrian* next;
} AksesAntrian;

void initAntrian(AksesAntrian** head);
int enqueueAkses(AksesAntrian** head, Pemesanan p, const char* pemesan, Jam waktu, int* sisaKursi);
void printAksesAntrian(AksesAntrian* head);
void freeAksesAntrian(AksesAntrian** head);
void prosesPemesananAntrian(const char* email, Pemesanan p, HistoryNode** riwayatStack, ATiket* listFileTiket);

#endif
