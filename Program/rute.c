#include "rute.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Station* create_station(const char* name) {
    Station* new_station = (Station*)malloc(sizeof(Station));
    if (new_station == NULL) {
        perror("Gagal alokasi memori untuk stasiun");
        exit(EXIT_FAILURE);
    }
    
    new_station->name = strdup(name); 
    if (new_station->name == NULL) {
        perror("Gagal alokasi memori untuk nama stasiun");
        free(new_station);
        exit(EXIT_FAILURE);
    }

    new_station->children = NULL;
    new_station->child_count = 0;
    return new_station;
}

void add_child(Station* parent, Station* child) {
    if (parent == NULL || child == NULL) return;

    parent->child_count++;
    
    parent->children = (Station**)realloc(parent->children, parent->child_count * sizeof(Station*));
    if (parent->children == NULL) {
        perror("Gagal realokasi memori untuk children");
        exit(EXIT_FAILURE);
    }
    
    parent->children[parent->child_count - 1] = child;
}

void visualize_route(Station* node, int level, int is_last) {
    if (node == NULL) return;

    for (int i = 0; i < level; i++) {
        printf("    ");
    }

    if (level > 0) {
        printf(is_last ? "+-- " : "+-- ");
    }

    printf("%s\n", node->name);

    for (int i = 0; i < node->child_count; i++) {
        visualize_route(node->children[i], level + 1, (i == node->child_count - 1));
    }
}

void free_tree(Station* node) {
    if (node == NULL) return;

    for (int i = 0; i < node->child_count; i++) {
        free_tree(node->children[i]);
    }

    free(node->name);
    free(node->children);
    free(node);
}

void tampilRute(){
    printf("Visualisasi Rute Stasiun Kereta Api Pulau Jawa (Model Tree)\n");
    printf("============================================================\n\n");

    // --- Membangun Stuktur Tree Rute Kereta Api ---

    // 1. Membuat stasiun-stasiun (nodes)
    // Root
    Station* root = create_station("Stasiun Gambir (Jakarta)");

    // Percabangan Awal
    Station* cikampek = create_station("Stasiun Cikampek");

    // Jalur Utara
    Station* cirebon = create_station("Stasiun Cirebon");
    Station* tegal = create_station("Stasiun Tegal");
    Station* pekalongan = create_station("Stasiun Pekalongan");
    Station* semarang = create_station("Stasiun Semarang Tawang");
    Station* surabaya_utara = create_station("Stasiun Surabaya Pasarturi");

    // Jalur Selatan
    Station* bandung = create_station("Stasiun Bandung");
    Station* tasikmalaya = create_station("Stasiun Tasikmalaya");
    Station* kroya = create_station("Stasiun Kroya");
    Station* yogyakarta = create_station("Stasiun Yogyakarta (Tugu)");
    Station* solo = create_station("Stasiun Solo Balapan");
    Station* madiun = create_station("Stasiun Madiun");
    Station* surabaya_selatan = create_station("Stasiun Surabaya Gubeng");


    // 2. Menghubungkan stasiun-stasiun (membangun tree)
    
    // Root -> Cikampek (sebagai hub percabangan)
    add_child(root, cikampek);

    // Cabang dari Cikampek
    add_child(cikampek, cirebon); // Jalur Utara
    add_child(cikampek, bandung);  // Jalur Selatan

    // Membangun Jalur Utara
    add_child(cirebon, tegal);
    add_child(tegal, pekalongan);
    add_child(pekalongan, semarang);
    add_child(semarang, surabaya_utara);

    // Membangun Jalur Selatan
    add_child(bandung, tasikmalaya);
    add_child(tasikmalaya, kroya);
    add_child(kroya, yogyakarta);
    add_child(yogyakarta, solo);
    add_child(solo, madiun);
    add_child(madiun, surabaya_selatan);

    // --- Menampilkan Tree ---
    visualize_route(root, 0, 0);
}