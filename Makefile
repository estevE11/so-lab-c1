output: cli1.c ser1.c
	gcc cli1.c -o cli1.out
	gcc ser1.c -o ser1.out

clear:
	rm cli1.out
	rm ser1.out