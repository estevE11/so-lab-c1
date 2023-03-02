output: cli2.c ser2.c
	gcc cli2.c -o cli2.out
	gcc ser2.c -o ser2.out

clear:
	rm cli2.out
	rm ser2.out
