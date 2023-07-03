run:
	clang -o main src/main.c src/vec.c -lm && ./main && imv result.bmp
