/*
Nama: Annisa Reida Kamilaini
NIM: 241524032
*/

#ifndef RIWAYAT_PEMESANAN_H
#define RIWAYAT_PEMESANAN_H

#include "tipe_data_global.h"
#include "tiket.h"

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