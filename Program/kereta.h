#ifndef KERETA_H
#define KERETA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipe_data_global.h"

struct nodeStasiun;

typedef struct {
    char nama[MAX];
    char namaStasiun[MAX];
    Jam kedatangan;
    Jam keberangkatan;
    struct nodeStasiun* nextStasiun;  
} Stasiun;

typedef struct nodeStasiun {
    Stasiun stasiun;
    struct nodeStasiun* nextStasiun;
} nodeStasiun;

typedef struct {
    char nama[MAX];
    nodeStasiun* stasiun;
} Rute;

typedef struct {
    char nama[MAX];
    char namaKereta[MAX];
    char stasiunAwal[MAX];
    char stasiunTujuan[MAX];
    Jam jamBerangkat;
    float harga;
    int kapasitas;
    Rute utama;
    Rute reverse;
} Kereta;

void readFileListKereta();
void printListKereta();
void readKereta(char namaKereta[]);
void readKeretaToList(char *namaKereta, Kereta *kereta);
void tampilJadwal(Kereta kereta);
void menuTampilJadwal();

void createListRute(Rute *r);
void printRuteKereta(Rute r);
void insertStasiun(Rute *r, Stasiun s);

void cariKeretaDenganRute(const char* stasiunAwal, const char* stasiunTujuan);

int cekRutePadaKeretaDipilih(const char* namaKereta, const char* awal, const char* tujuan, char* jamAwal, char* jamTiba);

#endif
