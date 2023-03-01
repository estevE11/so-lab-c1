output: cli1.c ser1.c
	gcc cli1.c -o cli1
	gcc ser1.c -o ser1

clear:
	rm cli1.out
	rm ser1.out