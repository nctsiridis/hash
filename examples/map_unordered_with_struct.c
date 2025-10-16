#include "../map_unordered.h"
#include "math.h"
#include <stdio.h>

typedef struct MyStruct {
	int x, y;
	float angle;
} MyStruct;

int main() {
	MapUnordered *mp = map_unordered_new(sizeof(int), sizeof(MyStruct), 10, 10, &byte_hash);

	for (int i = 1; i <= 10; i++) {
		MyStruct s = {i + 1, i, sin((i + 1.0) / i)};
		printf("Mapping %d -> %d, %d, %f\n", i, s.x, s.y, s.angle);
		map_unordered_insert(mp, &i, &s);
	}

	for (int i = 1; i <= 10; i++) {
		void* res = map_unordered_get(mp, &i);
		if (res) {
			MyStruct s = *(MyStruct*)res;
			printf("Retreived %d -> %d, %d, %f\n", i, s.x, s.y, s.angle);
		}
	}

	for (int i = 1; i <= 10; i++) {
		if (i % 2) {
			MyStruct s = {i + 1, i, cos((i + 1.0) / i)};
			printf("Mapping %d -> %d, %d, %f\n", i, s.x, s.y, s.angle);
			map_unordered_insert(mp, &i, &s);
		} else {
			map_unordered_erase(mp, &i);
		}
	}

	for (int i = 1; i <= 10; i++) {
		void* res = map_unordered_get(mp, &i);
		if (res) {
			MyStruct s = *(MyStruct*)res;
			printf("Retreived %d -> %d, %d, %f\n", i, s.x, s.y, s.angle);
		}
	}

	return 0;
}
