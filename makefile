all:
	gcc -o clox *.c

clean:
	rm ./clox

test:
	./clox tests/test_scanner.k
