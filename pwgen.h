/*
 * pwgen.h --- header file for password generator
 *
 * Copyright (C) 2001 by Theodore Ts'o
 * 
 * This file may be distributed under the terms of the GNU Public
 * License.
 */

struct pw_element {
	const char	*str;
	int		flags;
};

/*
 * Flags for the pw_element
 */
#define CONSONANT	0x0001
#define VOWEL		0x0002
#define DIPTHONG	0x0004
#define NOT_FIRST	0x0008

/*
 * Flags for the pwgen function
 */
#define PW_ONE_NUMBER	0x0001
#define PW_ONE_CASE	0x0002

struct pwgen_func {
	const char	*name;
	void		(*func)(char *buf, int size, int pw_flags);
	int		flags;
};

/* Function prototypes */

/* pw_phonemes.c */
extern void pw_phonemes(char *buf, int size, int pw_flags);

/* pw_rand.c */
extern void pw_rand(char *buf, int size, int pw_flags);

/* randnum.c */
extern int pw_random_number(int max_num);
