#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manajemen_antrian.h"
#include "terminal_util.h"

void initAntrian(AksesAntrian** head) {
    *head = NULL;
}

int compareTime(Jam a, Jam b) {
    if (a.jam < b.jam) return -1;
    if (a.jam > b.jam) return 1;
    if (a.menit < b.menit) return -1;
    if (a.menit > b.menit) return 1;
    return 0;
}

int enqueueAkses(AksesAntrian** head, Pemesanan p, const char* pemesan, Jam waktu, int* sisaKursi) {
    if (*sisaKursi < p.jumlahPenumpang) {
        setWarna(WARNA_MERAH);
        printf("Pemesanan oleh %s gagal. Dibutuhkan %d kursi, sisa hanya %d.\n", pemesan, p.jumlahPenumpang, *sisaKursi);
        resetWarna();
        return 0;
    }

    AksesAntrian* newNode = (AksesAntrian*)malloc(sizeof(AksesAntrian));
    strcpy(newNode->pemesan, pemesan);
    newNode->jumlahTiket = p.jumlahPenumpang;
    newNode->waktuAkses = waktu;
    newNode->pemesanan = p;
    newNode->next = NULL;

    if (*head == NULL || compareTime(waktu, (*head)->waktuAkses) < 0) {
        newNode->next = *head;
        *head = newNode;
    } else {
        AksesAntrian* curr = *head;
        while (curr->next != NULL && compareTime(curr->next->waktuAkses, waktu) <= 0) {
            curr = curr->next;
        }
        newNode->next = curr->next;
        curr->next = newNode;
    }

    *sisaKursi -= p.jumlahPenumpang;

    setWarna(WARNA_HIJAU);
    printf("Pemesanan oleh %s berhasil (%d tiket).\n", pemesan, p.jumlahPenumpang);
    resetWarna();

    return 1;
}

void printAksesAntrian(AksesAntrian* head) {
    int no = 1;
    setWarna(WARNA_BIRU);
    printf("\n===== DAFTAR ANTRIAN PEMESANAN =====\n");
    resetWarna();
    while (head != NULL) {
        setWarna(WARNA_KUNING);
        printf("[%d] %s - %d tiket - Akses: %02d:%02d\n", no++, head->pemesan, head->jumlahTiket, head->waktuAkses.jam, head->waktuAkses.menit);
        resetWarna();
        printf("Rute: %s -> %s, Tgl: %02d/%02d/%d\n", head->pemesanan.stasiunAwal, head->pemesanan.stasiunTujuan,
               head->pemesanan.hariBerangkat.hari, head->pemesanan.hariBerangkat.bulan, head->pemesanan.hariBerangkat.tahun);
        head = head->next;
    }
}

void freeAksesAntrian(AksesAntrian** head) {
    AksesAntrian* temp;
    while (*head) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}
