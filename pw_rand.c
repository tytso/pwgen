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
#include <stdio.h>
#include <stdlib.h>
#include "pwgen.h"

const char *pw_digits = "0123456789";
const char *pw_uppers = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *pw_lowers = "abcdefghijklmnopqrstuvwxyz";
const char *pw_symbols = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
const char *pw_ambiguous = "B8G6I1l0OQDS5Z2";
const char *pw_vowels = "01aeiouyAEIOUY";

static void remove_chars(char *buf, const char *remove)
{
	const char *cp;

	if (!remove)
		return;
	for (cp = remove; *cp; cp++) {
		char *r = strchr(buf, *cp);

		if (r == NULL)
			continue;
		memmove(r, r+1, strlen(r));
	}
}

static int find_chars(char *buf, const char *set)
{
	const char *cp;

	for (cp = set; *cp; cp++)
		if (strchr(buf, *cp))
			return 1;
	return 0;
}

void pw_rand(char *buf, size_t size, int pw_flags, char *remove)
{
	char		ch, *chars, *wchars;
	size_t		i, len;
	int		feature_flags;

	len = 0;
	if (pw_flags & PW_DIGITS) {
		len += strlen(pw_digits);
	}
	if (pw_flags & PW_UPPERS) {
		len += strlen(pw_uppers);
	}
	len += strlen(pw_lowers);
	if (pw_flags & PW_SYMBOLS) {
		len += strlen(pw_symbols);
	}
        chars = malloc(len+1);
        if (!chars) {
		fprintf(stderr, "Couldn't malloc pw_rand buffer.\n");
		exit(1);
	}
	wchars = chars;
	if (pw_flags & PW_DIGITS) {
		strcpy(wchars, pw_digits);
		wchars += strlen(pw_digits);
	}
	if (pw_flags & PW_UPPERS) {
		strcpy(wchars, pw_uppers);
		wchars += strlen(pw_uppers);
	}
	strcpy(wchars, pw_lowers);
	wchars += strlen(pw_lowers);
	if (pw_flags & PW_SYMBOLS) {
		strcpy(wchars, pw_symbols);
	}
	if (remove) {
		if (pw_flags & PW_AMBIGUOUS)
			remove_chars(chars, pw_ambiguous);
		if (pw_flags & PW_NO_VOWELS)
			remove_chars(chars, pw_vowels);
		remove_chars(chars, remove);
		if ((pw_flags & PW_DIGITS) &&
		    !find_chars(chars, pw_digits)) {
			fprintf(stderr,
				"Error: No digits left in the valid set\n");
			exit(1);
		}
		if ((pw_flags & PW_UPPERS) &&
		    !find_chars(chars, pw_uppers)) {
			fprintf(stderr,
				"Error: No upper case letters left in "
				"the valid set\n");
			exit(1);
		}
		if ((pw_flags & PW_SYMBOLS) &&
		    !find_chars(chars, pw_symbols)) {
			fprintf(stderr,
				"Error: No symbols left in the valid set\n");
			exit(1);
		}
		if (chars[0] == '\0') {
			fprintf(stderr,
				"Error: No characters left in the valid set\n");
			exit(1);
		}
	}
	len = strlen(chars);
try_again:
	feature_flags = (size > 2) ? pw_flags : 0;
	i = 0;
	while (i < size) {
		ch = chars[pw_number(len)];
		if ((pw_flags & PW_AMBIGUOUS) && strchr(pw_ambiguous,ch))
			continue;
		if ((pw_flags & PW_NO_VOWELS) && strchr(pw_vowels, ch))
			continue;
		buf[i++] = ch;
		if ((feature_flags & PW_DIGITS) &&
		    strchr(pw_digits, ch))
			feature_flags &= ~PW_DIGITS;
		if ((feature_flags & PW_UPPERS) &&
		    strchr(pw_uppers, ch))
			feature_flags &= ~PW_UPPERS;
		if ((feature_flags & PW_SYMBOLS) &&
		    strchr(pw_symbols, ch))
			feature_flags &= ~PW_SYMBOLS;
	}
	if (feature_flags & (PW_UPPERS | PW_DIGITS | PW_SYMBOLS))
		goto try_again;
	buf[size] = 0;
	free(chars);
	return;
}	
