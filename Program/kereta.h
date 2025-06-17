#ifndef kereta_H
#define kereta_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipe_data_global.h"

typedef struct {
	char namaKereta[MAX];
    char stasiunAwal[MAX];
    char stasiunTujuan[MAX];
    Jam jamBerangkat;
    float harga;
    Rute utama;
    Rute reverse;
}Kereta;

typedef struct {
	char namaStasiun[MAX];
	Jam kedatangan;
	Jam keberangkatan;
	nodeStasiun nextStasiun;
}Stasiun;

typedef struct nodeStasiun *Stasiun;

typedef struct {
	nodeStasiun stasiun;
}Rute;

void readFileListKereta();
void printListKereta();
void readKereta(char namaKereta[]);
void tampilJadwal(Kereta kereta);
void menuTampilJadwal();

void createListRute(Rute *r);
void insertStasiun(Rute *r, nodeStasiun s);

#endif
