/*
 * sdice: a simple passphrase generator
 *
 * gcc -Wall -std=c99 -O0 -S -fverbose-asm sdice.c
 * gcc -Wall -std=c99 -O0 sdice.c -o sdice
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "simple1024.h"

/* (2^10)-1, i.e. ten 1s */
#define BITMASK (DSIZE - 1)

/* Maximum number of words */
#define MAXPHRASE 100

/* Size of the read buffer */
#define BSIZE (MAXPHRASE * 2)

int main(int argc, char **argv) {
	int fd;
	uint8_t buf[BSIZE];
	unsigned short int len = 8;

	/* be aware that your shell may store on disk the passed length */
	if (argc > 1) {
		char *endptr;
		long int n = strtol(argv[1], &endptr, 10);
		if ((!*endptr) && (n > 0) && (n <= MAXPHRASE))
			len = n;
		else
			return EXIT_FAILURE;
	}

	fd = open("/dev/urandom", O_RDONLY);
	if ((fd == -1) || (read(fd, buf, 2*len) != 2*len) || (close(fd) == -1))
		return EXIT_FAILURE;

	for (size_t i = 0; i < 2*len; i += 2)
		printf("%s ", words[*(uint16_t*)&buf[i] & BITMASK]);

	putchar(0x0A);

	/* check that your compiler don't optimize the zeroing away */
	memset(buf, 0, sizeof(buf));

	return EXIT_SUCCESS;
}

