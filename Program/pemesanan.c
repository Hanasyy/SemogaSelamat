/*
Nama: Hana Zahirah Syifa
NIM: 241524045
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pemesanan.h"
#include "kursi_kereta_UI.h"
#include "manajemen_antrian.h"
#include "tiket.h"
#include "riwayat_pemesanan.h"


void insertPemesanan(PemesananNode** head, Pemesanan data) {
    PemesananNode* newPemesananNode = (PemesananNode*)malloc(sizeof(PemesananNode));
    newPemesananNode->info = data;
    newPemesananNode->next = *head;
    *head = newPemesananNode;
}

void printAllPemesanan(PemesananNode* head) {
    PemesananNode* temp = head;
    int i = 1;

    while (temp != NULL) {
        printf("\nPemesanan ke-%d\n", i++);
        printf("Email Pemesan: %s\n", temp->info.emailPemesan);
        printf("Rute: %s -> %s\n", temp->info.stasiunAwal, temp->info.stasiunTujuan);
        printf("Tanggal Berangkat: %02d-%02d-%04d\n",
               temp->info.hariBerangkat.hari, temp->info.hariBerangkat.bulan, temp->info.hariBerangkat.tahun);
        printf("Kereta: %s\n", temp->info.kereta->namaKereta);
        printf("Jumlah Penumpang: %d\n", temp->info.jumlahPenumpang);

        int j = 0;
        while (j < temp->info.jumlahPenumpang) {
            DataPenumpang* p = &temp->info.daftarPenumpang[j];
            printf(" - %s (%s, %s)\n", p->nama,
                   p->gender == LAKI_LAKI ? "Laki-laki" : "Perempuan",
                   p->idType == KTP ? "KTP" : "Paspor");
            j++;
        }

        temp = temp->next;
    }
}

void freeListPemesanan(PemesananNode** head) {
    while (*head != NULL) {
        PemesananNode* temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

int validasiTanggal(Date d) {
    if (d.hari < 1 || d.hari > 31 || d.bulan < 1 || d.bulan > 12 || d.tahun < 2024) return 0;
    return 1;
}

int validasiStasiun(const Kereta* k, const char* asal, const char* tujuan) {
    int foundAsal = 0;
    int foundTujuan = 0;

    nodeStasiun* current = k->utama.stasiun;
    while (current != NULL) {
        if (strcmp(current->stasiun.namaStasiun, asal) == 0)
            foundAsal = 1;
        if (strcmp(current->stasiun.namaStasiun, tujuan) == 0)
            foundTujuan = 1;

        current = current->nextStasiun;
    }

    return foundAsal && foundTujuan;
}

int cekKapasitasTersedia(PemesananNode* head, Kereta* kereta, Date tanggal, int jumlahDiminta) {
    int totalTerpesan = 0;
    PemesananNode* temp = head;

    while (temp != NULL) {
        if (temp->info.kereta == kereta &&
            temp->info.hariBerangkat.hari == tanggal.hari &&
            temp->info.hariBerangkat.bulan == tanggal.bulan &&
            temp->info.hariBerangkat.tahun == tanggal.tahun
			) {
            totalTerpesan += temp->info.jumlahPenumpang;
        }
        temp = temp->next;
    }

    return (kereta->kapasitas - totalTerpesan) >= jumlahDiminta;
}

void simpanPemesananKeFile(PemesananNode* head, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) return;

    PemesananNode* temp = head;
    while (temp != NULL) {
        Pemesanan* p = &temp->info;
        fprintf(f, "%s;%s;%s;%d-%d-%d;%s;%d\n",
                p->emailPemesan, p->stasiunAwal, p->stasiunTujuan,
                p->hariBerangkat.hari, p->hariBerangkat.bulan, p->hariBerangkat.tahun,
                p->kereta->namaKereta, p->jumlahPenumpang);

        int i = 0;
        while (i < p->jumlahPenumpang) {
            fprintf(f, "%s|%s|%d|%d\n", p->daftarPenumpang[i].noIdentitas,
                    p->daftarPenumpang[i].nama,
                    (int)p->daftarPenumpang[i].gender,
                    (int)p->daftarPenumpang[i].idType);
            i++;
        }

        temp = temp->next;
    }

    fclose(f);
}

void loadPemesananDariFile(PemesananNode** head, const char* filename, Kereta daftarKereta[], int jumlahKereta) {
    FILE* f = fopen(filename, "r");
    if (!f) return;

    char baris[1024];
    while (fgets(baris, sizeof(baris), f)) {
        Pemesanan p;
        char namaKereta[MAX];
        sscanf(baris, "%[^;];%[^;];%[^;];%d-%d-%d;%[^;];%d",
               p.emailPemesan, p.stasiunAwal, p.stasiunTujuan,
               &p.hariBerangkat.hari, &p.hariBerangkat.bulan, &p.hariBerangkat.tahun,
               namaKereta, &p.jumlahPenumpang);

        p.kereta = NULL;
        int i = 0;
        while (i < jumlahKereta) {
            if (strcmp(daftarKereta[i].namaKereta, namaKereta) == 0) {
                p.kereta = &daftarKereta[i];
                break;
            }
            i++;
        }

        int j = 0;
        while (j < p.jumlahPenumpang && fgets(baris, sizeof(baris), f)) {
            sscanf(baris, "%[^|]|%[^|]|%d|%d",
                   p.daftarPenumpang[j].noIdentitas,
                   p.daftarPenumpang[j].nama,
                   (int*)&p.daftarPenumpang[j].gender,
                   (int*)&p.daftarPenumpang[j].idType);
            j++;
        }

        insertPemesanan(head, p);
    }

    fclose(f);
}

void prosesPemesananUser(const User* userLogin, PemesananNode** head, Kereta* kereta, int jumlahKereta, HistoryNode** historyTop) {
    Pemesanan pemesanan;
    strcpy(pemesanan.emailPemesan, userLogin->email);
    pemesanan.kereta = kereta;

    printf("Masukkan stasiun awal: ");
    fgets(pemesanan.stasiunAwal, MAX, stdin);
    pemesanan.stasiunAwal[strcspn(pemesanan.stasiunAwal, "\n")] = 0;

    printf("Masukkan stasiun tujuan: ");
    fgets(pemesanan.stasiunTujuan, MAX, stdin);
    pemesanan.stasiunTujuan[strcspn(pemesanan.stasiunTujuan, "\n")] = 0;

    printf("Tanggal keberangkatan (dd-mm-yyyy): ");
    scanf("%d-%d-%d", &pemesanan.hariBerangkat.hari, &pemesanan.hariBerangkat.bulan, &pemesanan.hariBerangkat.tahun);
    getchar();
    
	// Hitung jumlah kursi terisi untuk tanggal dan kereta ini
	int kursiTerisi = 0;
	PemesananNode* temp = *head;
	while (temp != NULL) {
	    if (temp->info.kereta == kereta &&
	        temp->info.hariBerangkat.hari == pemesanan.hariBerangkat.hari &&
	        temp->info.hariBerangkat.bulan == pemesanan.hariBerangkat.bulan &&
	        temp->info.hariBerangkat.tahun == pemesanan.hariBerangkat.tahun) {
	        kursiTerisi += temp->info.jumlahPenumpang;
	    }
	    temp = temp->next;
	}

	// Visualisasi kursi
	tampilkanKursi(kereta->kapasitas, kursiTerisi);

	// Hitung sisa kursi
	int sisaKursi = kereta->kapasitas - kursiTerisi;
	if (sisaKursi <= 0) {
	    printf("\nMaaf, semua kursi sudah penuh pada tanggal ini.\n");
	    printf("Tekan Enter untuk kembali ke dashboard...");
	    getchar();  // flush enter
	    return;
	}


	do {
    printf("Jumlah penumpang (maks %d): ", sisaKursi < MAX_PENUMPANG ? sisaKursi : MAX_PENUMPANG);
    fflush(stdout);
    scanf("%d", &pemesanan.jumlahPenumpang);
    getchar();

    if (pemesanan.jumlahPenumpang < 1 || 
        pemesanan.jumlahPenumpang > sisaKursi || 
        pemesanan.jumlahPenumpang > MAX_PENUMPANG) {
        printf("Jumlah tidak valid. Coba lagi.\n");
    }
	} while (pemesanan.jumlahPenumpang < 1 || 
         pemesanan.jumlahPenumpang > sisaKursi || 
         pemesanan.jumlahPenumpang > MAX_PENUMPANG);

    ATiket listTiket = NULL;
    
    for (int i = 0; i < pemesanan.jumlahPenumpang; i++) {
        printf("\n-- Data Penumpang %d --\n", i + 1);
        printf("Nama: ");
        fgets(pemesanan.daftarPenumpang[i].nama, MAX, stdin);
        pemesanan.daftarPenumpang[i].nama[strcspn(pemesanan.daftarPenumpang[i].nama, "\n")] = 0;

        printf("Tipe Identitas (0=KTP, 1=Paspor): ");
        scanf("%d", (int*)&pemesanan.daftarPenumpang[i].idType);
        getchar();

        printf("Nomor Identitas: ");
        fgets(pemesanan.daftarPenumpang[i].noIdentitas, MAX, stdin);
        pemesanan.daftarPenumpang[i].noIdentitas[strcspn(pemesanan.daftarPenumpang[i].noIdentitas, "\n")] = 0;

        printf("Jenis Kelamin (0=Laki-laki, 1=Perempuan): ");
        scanf("%d", (int*)&pemesanan.daftarPenumpang[i].gender);
        getchar();

        Tiket tiket;
        strcpy(tiket.nama, pemesanan.daftarPenumpang[i].nama);
        strcpy(tiket.stasiun_awal, pemesanan.stasiunAwal);
        strcpy(tiket.stasiun_tujuan, pemesanan.stasiunTujuan);
        tiket.waktu = kereta->utama.stasiun->stasiun.keberangkatan;
        tiket.harga = 100000.0;
        tiket.tanggal = pemesanan.hariBerangkat;

        insertTiket(&listTiket, tiket);
        pushHistory(historyTop, tiket);
    }

    saveFileTicket(listTiket, userLogin->email);

    PemesananNode* newNode = (PemesananNode*)malloc(sizeof(PemesananNode));
    newNode->info = pemesanan;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        PemesananNode* curr = *head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = newNode;
    }

    printf("\nPemesanan berhasil disimpan!");
	printf("\nTekan Enter untuk kembali ke dashboard...");
	getchar(); getchar();  // <- dua kali jika sebelumnya ada scanf

}