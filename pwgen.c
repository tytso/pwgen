/*
 * pwgen.c --- generate secure passwords
 *
 * Copyright (C) 2001 by Theodore Ts'o
 * 
 * This file may be distributed under the terms of the GNU Public
 * License.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "pwgen.h"

struct pwgen_func generators[] = {
	{ "phonemes",	pw_phonemes,	0 },
	{ "rand",	pw_rand,	0 },
	{ 0,		0,		0 }
};
	


int main(int argc, char **argv)
{
	int	pw_length = 8;
	int	term_width = 80;
	int	num_pw = 1;
	int	do_columns;
	int	i, num_cols;
	char	*buf;
	void	(*pwgen)(char *buf, int size, int pw_flags);
	
	pwgen = pw_phonemes;
	do_columns = isatty(1);

	buf = malloc(pw_length+1);
	if (!buf) {
		fprintf(stderr, "Couldn't malloc password buffer.\n");
		exit(1);
	}
	if (do_columns) {
		num_cols = term_width / (pw_length+1);
		num_pw = num_cols * 20;
	}
	for (i=0; i < num_pw; i++) {
		pwgen(buf, pw_length, PW_ONE_NUMBER | PW_ONE_CASE);
		if (!do_columns || ((i % num_cols) == (num_cols-1)))
			printf("%s\n", buf);
		else
			printf("%s ", buf);
	}
	free(buf);
}
