tests: list.c tests.c
	gcc -Wall -std=c11 -g -o tests list.c tests.c
