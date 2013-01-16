include ../../Makefile.common
HEADERS=png.h

LIBNAME=libpng

SRC= $(wildcard *.c)

OBJ=$(patsubst %.c,%.o,$(SRC))

OPT=$(CFLAGS) -I../zlib


all:$(LIBNAME).a

$(LIBNAME).a:$(OBJ)
	rm -f $@
	$(AR) cru $@ $^
	$(RANLIB) $@

%.o:%.c $(HEADERS)
	$(CC) -c -o $@ $< $(OPT)

clean:
	rm -f *.a *.o
