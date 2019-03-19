tests:
	gcc -o tests tests.c ip.c -lcunit

main:
	gcc -o main main.c ip.c files.c

clean:
	rm main tests