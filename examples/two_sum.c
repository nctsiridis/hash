#include "../map_unordered.h"
#include <stdio.h>
#include <time.h>
#include <hash.h>

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
	MapUnordered *mp = map_unordered_new(sizeof(int), sizeof(int), arr->size, 25, &byte_hash);
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

Pair hii_two_sum(IntArray *arr, int k) {
	int size = 0;
	HashIntInt *mp = hii_new();
	for (int i = 0; i < arr->size; i++) {
		int x = arr->data[i];
		int *index_match = hii_get(mp, x);
		if (index_match) {
			return (Pair) {*index_match, i};
		} else {
			int complement = k - x;
			hii_set(mp, complement, i);
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
	int size = 10000;
	int target = 0;
	int range = 10000000;
	IntArray *arr = malloc(sizeof(IntArray));
	arr->data = malloc(sizeof(int) * size);
	arr->size = size;
	for (int i = 0; i < size; i++) {
		arr->data[i] = (((int)rand()) % ((2 * range) + 1)) - range;
	}
	// arr_print(arr);
	printf("N = %d\n", size);
	clock_t start = clock();
	Pair res1 = two_sum(arr, target);
	clock_t end = clock();
	double time1 = (double)(end - start) / CLOCKS_PER_SEC;
	printf("MapUnordered Solution Runtime: %.6f\n", time1);
	if (res1.a >= 0) {
		printf("%d = %d + %d\nIndices: %d, %d\n", target, arr->data[res1.a], arr->data[res1.b], res1.a, res1.b);
	} else {
		printf("Not solution found\n");
	}

	start = clock();
	Pair res2 = n2solution(arr, target);
	end = clock();
	double time2 = (double)(end - start) / CLOCKS_PER_SEC;
	printf("N^2 Solution Runtime: %.6f\n", time2);
	if (res2.a >= 0) {
		printf("%d = %d + %d\nIndices: %d, %d\n", target, arr->data[res2.a], arr->data[res2.b], res2.a, res2.b);
	} else {
		printf("Not solution found\n");
	}

	start = clock();
	Pair res3 = hii_two_sum(arr, target);
	end = clock();
	double time3 = (double)(end - start) / CLOCKS_PER_SEC;
	printf("HashIntInt Solution Runtime: %.6f\n", time3);
	if (res3.a >= 0) {
		printf("%d = %d + %d\nIndices: %d, %d\n", target, arr->data[res3.a], arr->data[res3.b], res3.a, res3.b);
	} else {
		printf("Not solution found\n");
	}

	printf("MapUnordered Improvement %lf\n", time2 / time1);
	printf("HashIntInt Improvement %lf\n", time2 / time3);

	return 0;
}
