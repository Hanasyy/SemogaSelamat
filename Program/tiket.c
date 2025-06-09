#include "tiket.h"

void readFileTicket(ATiket *listTicket, char nama[MAX]){
	Tiket ticket;
	
	char filename[] = "Ticket\";
	char txt[] = ".txt";
	
	strcat(filename, nama);
	strcat(filename, txt);
	
	FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Gagal membuka file");
        return 1;
    }
    
    char baris[512];
    
    while (fgets(baris, sizeof(baris), file)) {
        baris[strcspn(baris, "\n")] = '\0'; // Hapus newline

        // Pisahkan berdasarkan tab
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
            
        insertTiket(&(*listTicket), tiket);
    }
    
    fclose(file);
}

void saveFileTicket(ATiket listTicket, char nama[MAX]){
	char filename[] = "Ticket\";
	char txt[] = ".txt";
	
	strcat(filename, nama);
	strcat(filename, txt);
	
	FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Gagal membuka file");
        return 1;
    }
    
    ATiket temp = listTicket;
    
    while(temp != Null){
    	fprintf(file, "%s\t%s\t%s\t%.2f\t%02d-%02d-%04d\t%02d:%02d\n",
	        listTicket.tiket.nama,
	        listTicket.tiket.stasiun_awal,
	        listTicket.tiket.stasiun_tujuan,
	        listTicket.tiket.harga,
	        listTicket.tiket.tanggal.hari,
	        listTicket.tiket.tanggal.bulan,
	        listTicket.tiket.tanggal.tahun,
	        listTicket.tiket.waktu.jam,
	        listTicket.tiket.waktu.menit
    	);
	}
	
	fclose(file);
}

void insertTiket(ATiket *listTicket, Tiket ticket){
	ATiket temp;
	
	ATiket newTiket = createNodeTicket(ticket);
	
	if(*listTicket == NULL){
		*listTicket = newTiket;
	}else{
		temp = *listTicket;
		while(temp->next != NULL){
			temp = temp->next;	
		}
		temp->next = newTiket;
	}
}

void printListTiket(ATiket listTicket){
	ATiket temp = listTicket;
	
	int count = 1;
	
	if(temp != NULL){
		printf("\nList Tiket %s", temp->tiket->nama);
		
		while(temp != NULL){
			printf("\n%d Dari %s Ke %s Tanggal %02d %02d %04d Pukul %02d:%02d", 
			count, 
			temp->tiket->stasiun_awal,
			temp->tiket->stasiun_tujuan,
			temp->tiket->tanggal->hari,
			temp->tiket->tanggal->bulan,
			temp->tiket->tanggal->tahun,
			temp->tiket->waktu->jam,
			temp->tiket->waktu->menit);
			
			count++;
			temp = temp->next;
		}
	}
	int i;
	printf("\nMasukkan nomor tiket yang ingin dicetak : ");
	scanf("%d", &i);
	
	if(i > count || i < 1){
		printf("\nInput tidak valid");
		printListTiket(listTicket);
	}else{
		Atiket temp2 = listTicket;
		count = 1;
		while(count != i){
			temp2 = temp2->next;
			count++;
		}
		if(count == i){
			cetakTiket(temp2.tiket);
		}
	}
}

ATiket createNodeTicket(Tiket t){
	ATiket newNode = (ATiket)malloc(sizeof(nodeTicket));
	newNode->tiket = t;
	newNode->next = NULL;
	
	return newNode;
}

void cetakTiket(Tiket tiket){
	printf("\nTiket atas nama : %s", tiket.nama);
	printf("\nStasiun Asal : %s", tiket.stasiun_awal);
	printf("\nStasiun Tujuan : %s", tiket.stasiun_tujuan);
	printf("\nHarga : %f", tiket.harga);
	printf("\nTanggal Keberangkatan : %02d %02d 04d", tiket.tanggal.hari, tiket.tanggal.bulan, tiket.tanggal.tahun);
	printf("Jam Keberangkatan : %02d:%02d", tiket.waktu.jam, tiket.waktu.menit);
	printf("\n\n");
}
