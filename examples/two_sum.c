#include "../map_unordered.h"
#include <stdio.h>
#include <time.h>

typedef struct IntArray {
	int* data;
	int size;
} IntArray;

typedef struct Pair {
	int a, b;
} Pair;

Pair n2solution(IntArray *arr, int k) {
	for (int i = 0; i < arr->size; i++) {
		for (int j = i + 1; j < arr->size; j++) {
			if (arr->data[i] + arr->data[j] == k) {
				return (Pair) {i, j};
			}
		}
	}
	return (Pair) {-1, -1};
}

Pair two_sum(IntArray *arr, int k) {
	int size = 0;
	MapUnordered *mp = map_unordered_new(sizeof(int), sizeof(int), 100, 5, &byte_hash);
	for (int i = 0; i < arr->size; i++) {
		int x = arr->data[i];
		void *index_ref = map_unordered_get(mp, &x);
		if (index_ref) {
			int index_match = *(int*)index_ref;
			return (Pair) {index_match, i};
		} else {
			int complement = k - x;
			map_unordered_insert(mp, &complement, &i);
			size++; // DEBUG
		}
	}
	return (Pair) {-1, -1};
}

void arr_print(IntArray *arr) {
	for (int i = 0; i < arr->size; i++) {
		printf("%d, ", arr->data[i]);
	}
	printf("\n");
}

int main() {
	int size = 1000000;
	int target = 800;
	int range = 1200;
	IntArray *arr = malloc(sizeof(IntArray));
	arr->data = malloc(sizeof(int) * size);
	arr->size = size;
	for (int i = 0; i < size; i++) {
		arr->data[i] = (((int)rand()) % ((2 * range) + 1)) - range;
	}
	// arr_print(arr);

	clock_t start = clock();
	printf("Starting comparison\n");
	Pair res1 = two_sum(arr, target);
	clock_t end = clock();
	printf("Hashmap Solution Runtime: %.6f\n", (double)(end - start) / CLOCKS_PER_SEC);
	if (res1.a >= 0) {
		printf("%d = %d + %d\nIndices: %d, %d\n", target, arr->data[res1.a], arr->data[res1.b], res1.a, res1.b);
	} else {
		printf("Not solution found\n");
	}

	start = clock();
	Pair res2 = n2solution(arr, target);
	end = clock();
	printf("N^2 Solution Runtime: %.6f\n", (double)(end - start) / CLOCKS_PER_SEC);
	if (res2.a >= 0) {
		printf("%d = %d + %d\nIndices: %d, %d\n", target, arr->data[res2.a], arr->data[res2.b], res2.a, res2.b);
	} else {
		printf("Not solution found\n");
	}

	return 0;
}
