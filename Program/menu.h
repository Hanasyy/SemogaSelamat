/*
Nama: Annisa Reida Kamilaini
NIM: 241524032
*/

#ifndef MENU_H
#define MENU_H

#include "user.h"
#include "pemesanan.h"
#include "kereta.h"

void menuUtama();
void dashboardUser(const User* user, PemesananNode** headPemesanan, Kereta daftarKereta[], int jumlahKereta);

#endif
