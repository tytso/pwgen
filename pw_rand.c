/*
 * pw_rand.c --- generate completely random (and hard to remember)
 * 	passwords
 *
 * Copyright (C) 2001,2002 by Theodore Ts'o
 * 
 * This file may be distributed under the terms of the GNU Public
 * License.
 */

#include <string.h>
#include "pwgen.h"

#define PW_CHARS "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

void pw_rand(char *buf, int size, int pw_flags)
{
	char	ch, *chars = PW_CHARS;
	int	i = 0, len;

	len = strlen(chars);
	while (i < size) {
		ch = chars[pw_random_number(len)];
		buf[i++] = ch;
	}
	buf[size] = 0;
	return;
}	
