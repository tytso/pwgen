CC = gcc
RM = rm -f
CFLAGS = -g -O2
DEFS =  -DHAVE_RAND38
TAR = tar

CCOPTS = $(CFLAGS) $(DEFS)

OBJS= pwgen.o pw_phonemes.o pw_rand.o randnum.o

pwgen: $(OBJS)
	$(CC) -o pwgen $(OBJS)

clean:
	$(RM) $(OBJS) pwgen *~

