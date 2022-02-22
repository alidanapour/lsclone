all:
	gcc main.c print.c dirstats.c common.c -o myls -lm -Wall -g

clean:
	rm -f myls
