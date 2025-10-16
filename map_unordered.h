#ifndef MAP_UNORDERED
#define MAP_UNORDERED

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Pair {
	void *key, *val;
} Pair;

typedef struct MapUnordered {
	int size, array_size;
	int key_size, val_size;
	int max_probe_dist;
	Pair *entries;
	int (*hash)(void*, int, int);
} MapUnordered;

int byte_hash(void* key, int key_size, int array_size);
void debug_map_unordered_print(MapUnordered *mp);
MapUnordered* map_unordered_new(
	int key_size, 
	int val_size, 
	int init_size, 
	int max_probe_dist,
	int (*hash)(void*, int, int)
);
void map_unordered_free_entries(MapUnordered *mp);
void map_unordered_insert(MapUnordered *mp, void *key, void *val);
void map_unordered_expand(MapUnordered *mp);
void map_unordered_insert(MapUnordered *mp, void *key, void *val);
void map_unordered_erase(MapUnordered *mp, void*key);
void* map_unordered_get(MapUnordered *mp, void *key);

#endif
