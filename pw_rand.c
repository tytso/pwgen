/*
 * pw_rand.c --- generate completely random (and hard to remember)
 * 	passwords
 *
 * Copyright (C) 2001 by Theodore Ts'o
 * 
 * This file may be distributed under the terms of the GNU Public
 * License.
 */

#include <string.h>
#include "pwgen.h"

#define FIRST_CHAR '!'
#define LAST_CHAR '~'

static char bad_chars[] = "'\\\"";

void pw_rand(char *buf, int size, int pw_flags)
{
	char	ch;
	int	i = 0;

	while (i < size) {
		ch = pw_random_number(LAST_CHAR - FIRST_CHAR) + FIRST_CHAR;
		if (strchr(bad_chars, ch))
			continue;
		buf[i++] = ch;
	}
	buf[size] = 0;
	return;
}	
