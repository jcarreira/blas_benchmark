all:
	gcc dot.c -o dot -Wall -ansi -Wall -lblas -std=c99 -ggdb
	gcc mmul.c -o mmul -Wall -ansi -Wall -lblas -std=c99 -ggdb
