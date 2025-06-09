#ifndef tiket_H
#define tiket_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#inclue "pemesanan.h"

typedef struct {
	char nama[MAX];
	char stasiun_awal[MAX];
	char stasiun_tujuan[MAX];
	float harga;
	Date tanggal;
	Jam waktu;
}Tiket;

typedef struct nodeTiket* ATiket;

typedef struct{
	Tiket tiket;
	ATiket next;
}nodeTiket;

void readFileTicket(ATiket *listTicket, char nama[MAX]);
void saveFileTicket(ATiket listTicket, char nama[MAX]);
void insertTiket(ATiket *listTicket, Tiket ticket);
void printListTiket(ATiket listTicket);
void cetakTiket(Tiket tiket);
ATiket createNodeTicket(Tiket t);

#endif