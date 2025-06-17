#ifndef rute_H
#define rute_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Station {
    char* name;                 
    struct Station** children;  
    int child_count;            
} Station;

Station* create_station(const char* name);
void add_child(Station* parent, Station* child);
void visualize_route(Station* node, int level, int is_last);
void free_tree(Station* node);
void tampilRute();

#endif