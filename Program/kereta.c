#include "kereta.h"

Kereta listKereta[MAX];
int jumlahKereta = 0;

void readFileListKereta() {
    FILE *file = fopen("listKereta.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    while (fgets(listKereta[jumlahKereta].namaKereta, MAX, file) != NULL) {
        
        listKereta[jumlahKereta].namaKereta[strcspn(listKereta[jumlahKereta].namaKereta, "\n")] = 0;
        jumlahKereta++;
    }

    fclose(file);
}

void printListKereta() {
    printf("List Kereta:\n");
    for (int i = 0; i < jumlahKereta; i++) {
        printf("%s\n", listKereta[i].namaKereta);
    }
}

void readKereta(char namaKereta[]) {
    char filename[MAX];
    snprintf(filename, sizeof(filename), "%s.txt", namaKereta);
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    Rute rUtama, rReverse;
    createListRute(&rUtama);
    createListRute(&rReverse);

    char jenisRute[MAX], namaStasiun[MAX];
    Jam jamKedatangan, jamKeberangkatan;

    while (fscanf(file, "%s\t%s\t%d:%d\t%d:%d", jenisRute, namaStasiun, 
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

    listKereta[jumlahKereta - 1].utama = rUtama;
    listKereta[jumlahKereta - 1].reverse = rReverse;
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
    newNode->nextStasiun = r->stasiun;
    r->stasiun = newNode;
}

void printRuteKereta(Rute r) {
    nodeStasiun *current = r.stasiun;
    while (current != NULL) {
        printf("Stasiun: %s, Kedatangan: %02d:%02d, Keberangkatan: %02d:%02d\n",
               current->stasiun.namaStasiun,
               current->stasiun.kedatangan.jam, current->stasiun.kedatangan.menit,
               current->stasiun.keberangkatan.jam, current->stasiun.keberangkatan.menit);
        current = current->nextStasiun;
    }
}
