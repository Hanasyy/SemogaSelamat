#ifndef tiket_H
#define tiket_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipe_data_global.h"     
#include "pemesanan.h"            
#define MAX 100

typedef struct {
    char nama[MAX];
    char stasiun_awal[MAX];
    char stasiun_tujuan[MAX];
    float harga;
    Date tanggal;
    Jam waktu;
} Tiket;

typedef struct nodeTiket {
    Tiket tiket;
    struct nodeTiket* next;
} nodeTiket;

typedef nodeTiket* ATiket;

void readFileTicket(ATiket* listTicket, char nama[MAX]);
void saveFileTicket(ATiket listTicket, const char* nama);
void insertTiket(ATiket* listTicket, Tiket ticket);
void printListTiket(ATiket listTicket);
void cetakTiket(Tiket tiket);
ATiket createNodeTicket(Tiket t);

#endif
