#ifndef KERETA_H
#define KERETA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipe_data_global.h"

struct nodeStasiun;

typedef struct {
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
    nodeStasiun* stasiun;
} Rute;

typedef struct {
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
void tampilJadwal(Kereta kereta);
void menuTampilJadwal();

void createListRute(Rute *r);
void insertStasiun(Rute *r, Stasiun s);

#endif
