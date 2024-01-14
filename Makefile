CC = gcc
CFLAGS = -Wall -Wextra

all: pagingdemand

pagingdemand: pagingdemand.c
	$(CC) $(CFLAGS) -o pagingdemand pagingdemand.c

clean:
	rm -f pagingdemand
