#include <stdio.h>
#include "kursi_kereta_UI.h"

void tampilkanKursi(int total_kursi, int kursi_terisi) {
    int baris = (total_kursi + 3) / 4; // asumsi 4 kursi per baris
    int no = 1;
    printf("\nVisualisasi Kursi Kereta:\n");
    printf("Legenda: [ ] = kosong  [X] = terisi\n\n");

    for (int i = 0; i < baris; i++) {
        for (int j = 0; j < 4; j++) {
            if (no > total_kursi) break;
            if (no <= kursi_terisi)
                printf("[X] ");
            else
                printf("[ ] ");
            no++;
        }
        printf("\n");
    }
}
