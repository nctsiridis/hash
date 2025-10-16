#include "map_unordered.h"

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

int byte_hash(void* key, int key_size, int array_size) {
	unsigned char *bytes = (unsigned char*) key;
	int hash = 0;
	for (int i = 0; i < key_size; i++){
		hash = (int)bytes[i] + (hash << 6) + (hash << 16) - hash;
	}
	return abs(hash) % array_size;
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
	int (*hash)(void*, int, int)
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
		int h = mp->hash(key, mp->key_size, mp->array_size);
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
	int h = mp->hash(key, mp->key_size, mp->array_size);
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
	int h = mp->hash(key, mp->key_size, mp->array_size);
	int index = probe_key(mp, h, key);
	return (index >= 0) ? mp->entries[index].val : NULL;
}
