output: cli3.c ser3.c file3.c
	gcc cli3.c -o cli3.out
	gcc ser3.c -o ser3.out
	gcc file3.c -o file3.out

clear:
	rm cli3.out
	rm ser3.out
	rm file3.out
