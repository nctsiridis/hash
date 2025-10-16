#include "../map_unordered.h"
#include <stdio.h>

int main() {
	MapUnordered *mp = map_unordered_new(1, 1, 3, 5, &byte_hash);
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
