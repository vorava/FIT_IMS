# IMS projekt
# Vojtech Orava (xorava02)
CC=g++
CFLAGS=-O2 -std=c++17 -Wall -Wextra
COUT=bitva

all:
	make $(COUT)

run:
	make $(COUT)
	./$(COUT) --silent -v 157 -o 582 -t 213 -p 0 -r 0 -k 0 -p 844 -r 418 -k 787

bitva: arg_parser.o main.o
	$(CC) $(CFLAGS) main.o arg_parser.o -o bitva -lsimlib -lm

arg_parser.o:
	$(CC) $(CFLAGS) arg_parser.cc -c

main.o:
	$(CC) $(CFLAGS) main.cc -c

clean:
	rm *.o *.tar.gz $(COUT)
