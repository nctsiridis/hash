example_two_sum: \
		examples/two_sum.c \
		init \
		map_unordered \
		build/map_unordered.o
	cc examples/two_sum.c build/map_unordered.o -o build/two_sum
	./build/two_sum

example_struct: \
		examples/map_unordered_with_struct.c \
		init \
		map_unordered \
		build/map_unordered.o
	cc examples/map_unordered_with_struct.c build/map_unordered.o -o build/map_unordered_with_struct_example
	./build/map_unordered_with_struct_example

example_char: \
		examples/map_unordered_with_char.c \
		init map_unordered build/map_unordered.o
	cc examples/map_unordered_with_char.c build/map_unordered.o -o build/map_unordered_with_char_example
	./build/map_unordered_with_char_example

init:
	mkdir -p build

map_unordered: map_unordered.c map_unordered.h
	cc -c map_unordered.c -o build/map_unordered.o
