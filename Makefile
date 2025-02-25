all: a4.c
	gcc -fsanitize=address -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Werror -std=c17 -Wpedantic -O0 -g a4.c -o a4

clean:
	rm -rf *.o a4 d
