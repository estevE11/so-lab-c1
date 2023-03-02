output: cli1.c ser1.c cli2.c ser2.c
	gcc cli1.c -o cli1.out
	gcc ser1.c -o ser1.out
	gcc cli2.c -o cli2.out
	gcc ser2.c -o ser2.out
	gcc cli3.c -o cli3.out
	gcc ser3.c -o ser3.out
	gcc file3.c -o file3.out

clear:
	rm cli1.out
	rm ser1.out
	rm cli2.out
	rm ser2.out
	rm cli3.out
	rm ser3.out
	rm file3.out
