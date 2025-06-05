/*
Nama: Annisa Reida Kamilaini
NIM: 241524032
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RIWAYAT_PEMESANAN.h"

// Push ke stack riwayat pemesanan
void pushHistory(HistoryNode** top, Tiket data) {
    HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
    newNode->tiket = data;
    newNode->next = *top;
    *top = newNode;
}

// Pop dari stack riwayat pemesanan
void popHistory(HistoryNode** top) {
    if (*top == NULL) return;
    HistoryNode* temp = *top;
    *top = (*top)->next;
    free(temp);
}

// Cetak isi stack
void printHistory(HistoryNode* top) {
    puts("==============================");
    puts("         RIWAYAT PEMESANAN     ");
    puts("==============================");
    printf("\n");

    while (top != NULL) {
        printf("Tanggal: %02d %s %d\n", top->tiket.tanggal.tanggal,
            getNamaBulan(top->tiket.tanggal.bulan), top->tiket.tanggal.tahun);
        puts("------------------------------");
        printf("| Nama           : %-10s\n", top->tiket.nama);
		printf("| Rute           : %-10s ? %-10s\n", top->tiket.stasiun_awal, top->tiket.stasiun_tujuan);
		printf("| Jam Berangkat  : %02d:%02d\n", top->tiket.waktu.hour, top->tiket.waktu.min);
		printf("| Harga          : Rp%.0f\n", top->tiket.harga);

        puts("------------------------------");
        top = top->next;
    }
}

const char* getNamaBulan(int bulan) {
    const char* nama_bulan[] = {
        "", "Januari", "Februari", "Maret", "April", "Mei", "Juni",
        "Juli", "Agustus", "September", "Oktober", "November", "Desember"
    };
    return nama_bulan[bulan];
}