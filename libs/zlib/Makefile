include ../../Makefile.common

HEADERS=zlib.h

LIBNAME=libz

SRC= $(wildcard *.c)

OBJ=$(patsubst %.c,%.o,$(SRC))

OPT=$(CFLAGS)


all:$(LIBNAME).a

$(LIBNAME).a:$(OBJ)
	rm -f $@
	$(AR) cru $@ $^
	$(RANLIB) $@

%.o:%.c $(HEADERS)
	$(CC) -c -o $@ $< $(OPT)

clean:
	rm -f *.a *.o *~
