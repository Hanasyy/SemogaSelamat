#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "kereta.h"

Kereta listKereta[MAX];
int jumlahKereta = 0;

void readFileListKereta() {
    FILE *f = fopen("listKereta.txt", "r");
    if (!f) return;

    char nama[MAX];
    while (fgets(nama, MAX, f)) {
        nama[strcspn(nama, "\n")] = 0;
        strcpy(listKereta[jumlahKereta].namaKereta, nama);
        readKeretaToList(nama, &listKereta[jumlahKereta]);
        jumlahKereta++;
    }
    fclose(f);
}

void printListKereta() {
    printf("List Kereta:\n");
    for (int i = 0; i < jumlahKereta; i++) {
        printf("%s\n", listKereta[i].namaKereta);
    }
}

void readKeretaToList(char *namaKereta, Kereta *kereta) {
    char path[256];
    snprintf(path, sizeof(path), "C:/Users/reida/Downloads/KAI_ACCESS_FINAL_CLEAN_OK/%s.txt", namaKereta);

    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Gagal membuka file %s\n", path);
        return;
    }

    Rute rUtama, rReverse;
    createListRute(&rUtama);
    createListRute(&rReverse);

    char jenisRute[MAX], namaStasiun[MAX];
    Jam jamKedatangan, jamKeberangkatan;

    while (fscanf(file, "%[^\t]\t%[^\t]\t%d:%d\t%d:%d\n",
                  jenisRute, namaStasiun,
                  &jamKedatangan.jam, &jamKedatangan.menit,
                  &jamKeberangkatan.jam, &jamKeberangkatan.menit) != EOF) {

        Stasiun s;
        strcpy(s.namaStasiun, namaStasiun);
        s.kedatangan = jamKedatangan;
        s.keberangkatan = jamKeberangkatan;
        s.nextStasiun = NULL;

        if (strcmp(jenisRute, "UTAMA") == 0) {
            insertStasiun(&rUtama, s);
        } else if (strcmp(jenisRute, "REVERSE") == 0) {
            insertStasiun(&rReverse, s);
        }
    }

    fclose(file);
    strcpy(kereta->namaKereta, namaKereta);
    kereta->utama = rUtama;
    kereta->reverse = rReverse;
    kereta->kapasitas = 40; 
}

void tampilJadwal(Kereta kereta) {
    printf("Jadwal Kereta: %s\n", kereta.namaKereta);
    printf("Rute Utama:\n");
    printRuteKereta(kereta.utama);
    printf("Rute Reverse:\n");
    printRuteKereta(kereta.reverse);
}

void createListRute(Rute *r) {
    r->stasiun = NULL;
}

void insertStasiun(Rute *r, Stasiun s) {
    nodeStasiun *newNode = (nodeStasiun *)malloc(sizeof(nodeStasiun));
    newNode->stasiun = s;
    newNode->nextStasiun = NULL;

    if (r->stasiun == NULL) {
        r->stasiun = newNode;
    } else {
        nodeStasiun *curr = r->stasiun;
        while (curr->nextStasiun != NULL) {
            curr = curr->nextStasiun;
        }
        curr->nextStasiun = newNode;
    }
}

void printRuteKereta(Rute r) {
    nodeStasiun *current = r.stasiun;
    int i = 1;
    while (current != NULL) {
        printf("%2d. Stasiun: %s, Kedatangan: %02d:%02d, Keberangkatan: %02d:%02d\n",
               i++, current->stasiun.namaStasiun,
               current->stasiun.kedatangan.jam, current->stasiun.kedatangan.menit,
               current->stasiun.keberangkatan.jam, current->stasiun.keberangkatan.menit);
        current = current->nextStasiun;
    }
}

void trimNewline(char* str) {
    str[strcspn(str, "\n")] = 0;
}

int cekRutePadaKeretaDipilih(const char* namaKereta, const char* awal, const char* tujuan, char* jamAwal, char* jamTiba) {
    char path[256];
    snprintf(path, sizeof(path), "C:/Users/reida/Downloads/KAI_ACCESS_FINAL_CLEAN_OK/%s.txt", namaKereta);

    FILE* f = fopen(path, "r");
    if (!f) return 0;

    char line[256];
    int ditemukanAwal = 0;
    while (fgets(line, sizeof(line), f)) {
        char arah[20], stasiun[100], keberangkatan[10], kedatangan[10];
        sscanf(line, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\n]",
               arah, stasiun, kedatangan, keberangkatan);

        if (!ditemukanAwal && strcasecmp(stasiun, awal) == 0) {
            strcpy(jamAwal, keberangkatan);
            ditemukanAwal = 1;
        } else if (ditemukanAwal && strcasecmp(stasiun, tujuan) == 0) {
            strcpy(jamTiba, kedatangan);
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}
