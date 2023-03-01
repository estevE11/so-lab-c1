output: cli1.c ser1.c cli2.c ser2.c
	gcc cli1.c -o cli1.out
	gcc ser1.c -o ser1.out
	gcc cli2.c -o cli2.out
	gcc ser2.c -o ser2.out

clear:
	rm cli1.out
	rm ser1.out
	rm cli2.out
	rm ser2.out
