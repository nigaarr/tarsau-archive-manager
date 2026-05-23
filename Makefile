CC = gcc
CFLAGS = -Wall -Wextra -g
OBJ = main.o archive.o extract.o

all: tarsau

tarsau: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o tarsau

%.o: %.c tarsau.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o tarsau