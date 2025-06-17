
#include "tiket.h"

ATiket createNodeTicket(Tiket t) {
    ATiket newNode = (ATiket)malloc(sizeof(nodeTiket));
    if (newNode != NULL) {
        newNode->tiket = t;
        newNode->next = NULL;
    }
    return newNode;
}

void insertTiket(ATiket *listTicket, Tiket ticket) {
    ATiket newTiket = createNodeTicket(ticket);
    if (!newTiket) return;

    if (*listTicket == NULL) {
        *listTicket = newTiket;
    } else {
        ATiket temp = *listTicket;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newTiket;
    }
}

void readFileTicket(ATiket *listTiket, char nama[MAX]) {
    Tiket tiket;
    char filename[200];
    snprintf(filename, sizeof(filename), "Ticket%s.txt", nama);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Gagal membuka file");
        return;
    }

    char baris[512];
    while (fgets(baris, sizeof(baris), file)) {
        baris[strcspn(baris, "\n")] = '\0';

        char *token = strtok(baris, "\t");
        if (token == NULL) continue;
        strcpy(tiket.nama, token);

        token = strtok(NULL, "\t");
        if (token != NULL) strcpy(tiket.stasiun_awal, token);

        token = strtok(NULL, "\t");
        if (token != NULL) strcpy(tiket.stasiun_tujuan, token);

        token = strtok(NULL, "\t");
        if (token != NULL) tiket.harga = atof(token);

        token = strtok(NULL, "\t");
        if (token != NULL) sscanf(token, "%d-%d-%d", 
            &tiket.tanggal.hari,
            &tiket.tanggal.bulan,
            &tiket.tanggal.tahun);

        token = strtok(NULL, "\t");
        if (token != NULL) sscanf(token, "%d:%d", 
            &tiket.waktu.jam,
            &tiket.waktu.menit);

        insertTiket(listTiket, tiket);
    }

    fclose(file);
}

void saveFileTicket(ATiket listTicket, char nama[MAX]) {
    char filename[200];
    snprintf(filename, sizeof(filename), "Ticket%s.txt", nama);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Gagal membuka file");
        return;
    }

    ATiket temp = listTicket;
    while (temp != NULL) {
        fprintf(file, "%s\t%s\t%s\t%.2f\t%02d-%02d-%04d\t%02d:%02d\n",
            temp->tiket.nama,
            temp->tiket.stasiun_awal,
            temp->tiket.stasiun_tujuan,
            temp->tiket.harga,
            temp->tiket.tanggal.hari,
            temp->tiket.tanggal.bulan,
            temp->tiket.tanggal.tahun,
            temp->tiket.waktu.jam,
            temp->tiket.waktu.menit);
        temp = temp->next;
    }

    fclose(file);
}

void printListTiket(ATiket listTicket) {
    ATiket temp = listTicket;
    int count = 1;

    while (temp != NULL) {
        printf("\nTiket %d:", count);
        printf("\nNama: %s", temp->tiket.nama);
        printf("\nRute: %s ke %s", temp->tiket.stasiun_awal, temp->tiket.stasiun_tujuan);
        printf("\nTanggal: %02d-%02d-%04d", temp->tiket.tanggal.hari, temp->tiket.tanggal.bulan, temp->tiket.tanggal.tahun);
        printf("\nWaktu: %02d:%02d", temp->tiket.waktu.jam, temp->tiket.waktu.menit);
        printf("\nHarga: %.2f", temp->tiket.harga);
        printf("\n--------------------------");

        count++;
        temp = temp->next;
    }

    printf("\nMasukkan nomor tiket yang ingin dicetak: ");
    int pilihan;
    scanf("%d", &pilihan);

    if (pilihan < 1 || pilihan >= count) {
        printf("Nomor tidak valid.\n");
        return;
    }

    temp = listTicket;
    for (int i = 1; i < pilihan; i++) {
        temp = temp->next;
    }
    cetakTiket(temp->tiket);
}

void cetakTiket(Tiket tiket) {
    printf("\n--------------------------");
    printf("\nTIKET KERETA API");
    printf("\n--------------------------");
    printf("\nNama: %s", tiket.nama);
    printf("\nStasiun Awal: %s", tiket.stasiun_awal);
    printf("\nStasiun Tujuan: %s", tiket.stasiun_tujuan);
    printf("\nTanggal: %02d-%02d-%04d", tiket.tanggal.hari, tiket.tanggal.bulan, tiket.tanggal.tahun);
    printf("\nJam: %02d:%02d", tiket.waktu.jam, tiket.waktu.menit);
    printf("\nHarga: Rp%.2f", tiket.harga);
    printf("\n--------------------------\n");
}
