include ../../Makefile.common

.PHONY : clean all test


PROGS= \
libbmp.a


LIBSRC=\
bmp.cpp

HEADERS=\
bmp.h


OPT  = $(CFLAGS) -I../bmp -I../libpng -I../libjpeg -I../zlib -I../freegetopt


OBJ   = $(patsubst %.cpp, %.o  , $(LIBSRC) ) 

all: $(PROGS)


libbmp.a:$(OBJ)
	rm -f $@
	$(AR) cru $@ $^
	$(RANLIB) $@

%.o:%.cpp  $(HEADERS)
	$(CXX) -c -o $@ $< $(OPT)
obj:
	mkdir obj
clean:
	-rm -f $(PROGS) *.o *.a
