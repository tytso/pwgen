/*
 * randnum.c -- generate (good) randum numbers.
 *
 * Copyright (C) 2001,2002 by Theodore Ts'o
 * 
 * This file may be distributed under the terms of the GNU Public
 * License.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "pwgen.h"

#ifdef HAVE_DRAND48
extern double drand48(void);
#endif

static int get_random_fd(void);

/* Borrowed/adapted from e2fsprogs's UUID generation code */
static int get_random_fd()
{
	struct timeval	tv;
	static int	fd = -2;

	if (fd == -2) {
		gettimeofday(&tv, 0);
		fd = open("/dev/urandom", O_RDONLY);
		if (fd == -1)
			fd = open("/dev/random", O_RDONLY | O_NONBLOCK);
	}
	return fd;
}

/*
 * Generate a random number n, where 0 <= n < max_num, using
 * /dev/urandom if possible.
 */
int pw_random_number(max_num)
	int max_num;
{
	unsigned int rand_num;
	int i, fd = get_random_fd();
	int lose_counter = 0, nbytes = sizeof(rand_num);
	char *cp = (char *) &rand_num;

	if (fd >= 0) {
		while (nbytes > 0) {
			i = read(fd, cp, nbytes);
			if ((i < 0) &&
			    ((errno == EINTR) || (errno == EAGAIN)))
				continue;
			if (i <= 0) {
				if (lose_counter++ == 8)
					break;
				continue;
			}
			nbytes -= i;
			cp += i;
			lose_counter = 0;
		}
	}
	if (nbytes == 0)
		return (rand_num % max_num);

	/* We weren't able to use /dev/random, fail hard */

	fprintf(stderr, "No entropy available!\n");
	exit(1);
}
