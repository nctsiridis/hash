#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// what tf we wanna do

// struct -> struct hash

typedef struct Pair {
	void *key, *val;
} Pair;

typedef struct MapUnordered {
	int size, array_size;
	int key_size, val_size;
	int max_probe_dist;
	Pair *entries;
	int (*hash)(void*, int);
} MapUnordered;

int byte_hash(void* key, int array_size) {
	// TODO, generic hash function for all structs (not efficient)
	// learn about hash functions before we do this
	return 0;
}

void debug_map_unordered_print(MapUnordered *mp) {
	for (int i = 0; i < mp->array_size; i++) {
		if (mp->entries[i].key) {
			printf("(%c %c), ", *(char*)mp->entries[i].key, *(char*)mp->entries[i].val);
		} else {
			printf("NULL, ");
		}
	}
	printf("\n");
}

MapUnordered* map_unordered_new(
	int key_size, 
	int val_size, 
	int init_size, 
	int max_probe_dist,
	int (*hash)(void*, int)
) {
	MapUnordered *res = malloc(sizeof(MapUnordered));
	res->array_size = init_size;
	res->key_size = key_size;
	res->val_size = val_size;
	res->max_probe_dist = max_probe_dist;
	res->size = 0;
	res->entries = malloc(sizeof(Pair) * init_size);
	for (int i = 0; i < init_size; i++) {
		res->entries[i].key = NULL;
		res->entries[i].val = NULL;
	}
	res->hash = hash;
	return res;
}

// NOTE: use realloc in future to speed this up
void map_unordered_free_entries(MapUnordered *mp) {
	for (int i = 0; i < mp->array_size; i++) {
		free(mp->entries[i].key);
		free(mp->entries[i].val);
		mp->entries[i].key = NULL;
		mp->entries[i].val = NULL;
	}
	free(mp->entries);
	mp->entries = NULL;
}

int probe_key(MapUnordered *mp, int h, void* key) {
	for (int i = 0; i < mp->max_probe_dist; i++) {
		if (!mp->entries[h].key) continue;
		if (memcmp(mp->entries[h].key, key, mp->key_size) == 0) return h;
		h = (h + 1) % mp->array_size;
	}
	return -1;
}

int probe_key_or_empty(MapUnordered *mp, int h, void* key) {
	for (int i = 0; i < mp->max_probe_dist; i++) {
		if (
			!mp->entries[h].key || 
			memcmp(mp->entries[h].key, key, mp->key_size) == 0
		) return h;
		h = (h + 1) % mp->array_size;
	}
	return -1;
}

void map_unordered_insert(MapUnordered *mp, void *key, void *val); // TODO remove
void map_unordered_expand(MapUnordered *mp) {
	Pair *temp_entries = malloc(sizeof(Pair) * mp->size);
	int index = 0, temp_index = 0;
	int size = mp->size;
	while (size--) {
		while (mp->entries[index].val == NULL) index++;
		temp_entries[temp_index].key = malloc(mp->key_size);
		temp_entries[temp_index].val = malloc(mp->val_size);
		memcpy(temp_entries[temp_index].key, mp->entries[index].key, mp->key_size);
		memcpy(temp_entries[temp_index].val, mp->entries[index].val, mp->val_size);
		temp_index++;
		index++;
	}
	map_unordered_free_entries(mp);
	mp->array_size *= 2;
	mp->entries = malloc(sizeof(Pair) * mp->array_size);
	for (int i = 0; i < mp->array_size; i++) {
		mp->entries[i].key = NULL;
		mp->entries[i].val = NULL;
	}
	for (int i = 0; i < mp->size; i++) {
		map_unordered_insert(mp, temp_entries[i].key, temp_entries[i].val);
		mp->size--; // Kinda hacky
	}
}

void map_unordered_insert(MapUnordered *mp, void *key, void *val) {
	int tries = 10;
	while (tries--) {
		int h = mp->hash(key, mp->array_size);
		int index = probe_key_or_empty(mp, h, key);
		if (index < 0) {
			map_unordered_expand(mp);
			continue;
		} else {
			mp->size++;
		}
		if(!mp->entries[index].key) mp->entries[index].key = malloc(mp->key_size);
		if(!mp->entries[index].val) mp->entries[index].val = malloc(mp->val_size);
		memcpy(mp->entries[index].key, key, mp->key_size);
		memcpy(mp->entries[index].val, val, mp->val_size);
		break;
	}
}

void map_unordered_erase(MapUnordered *mp, void*key) {
	int h = mp->hash(key, mp->array_size);
	int index = probe_key(mp, h, key);
	if (index >= 0) {
		free(mp->entries[index].key);
		free(mp->entries[index].val);
		mp->entries[index].key = NULL;
		mp->entries[index].val = NULL;
		mp->size--;
	}
}

void* map_unordered_get(MapUnordered *mp, void *key) {
	int h = mp->hash(key, mp->array_size);
	int index = probe_key(mp, h, key);
	return (index >= 0) ? mp->entries[index].val : NULL;
}

int example_hash(void* key_ref, int array_size) {
	// cast key
	char *key = (char*)key_ref;
	return abs(*key << 1) % array_size;
}

int main() {
	MapUnordered *mp = map_unordered_new(1, 1, 3, 5, &example_hash);
	for (char key = 'a'; key <= 'i'; key++) {
		char val = key + 1;
		printf("Inserting %c->%c\n", key, val);
		map_unordered_insert(mp, &key, &val);
	}
	for (char key = 'a'; key <= 'i'; key++) {
		char* val_ref = map_unordered_get(mp, &key);
		printf("Got %c->%c\n", key, *val_ref);
	}
	for (char key = 'a'; key <= 'i'; key += 2) {
		printf("Erasing %c\n", key);
		map_unordered_erase(mp, &key);
	}
	for (char key = 'a'; key <= 'i'; key++) {
		char* val_ref = map_unordered_get(mp, &key);
		if (val_ref) printf("Got %c->%c\n", key, *val_ref);
		else printf("Got %c->NULL\n", key);
	}
	return 0;
}
