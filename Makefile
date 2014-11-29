.PHONEY: clean

cgi-bin/Login.cgi: Login.o
	gcc -o cgi-bin/Login.cgi Login.o

Login.o: Login.c
	gcc -c Login.c

clean:
	rm -rf Login.o
