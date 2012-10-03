include ../../Makefile.common

.PHONY : clean all test


PROGS= \
libutils.a


LIBSRC=\
$(wildcard *.cpp)

HEADERS=\
$(wildcard *.h)


OPT  = $(CFLAGS)

OBJ   = $(patsubst %.cpp, %.o  , $(LIBSRC) ) 

all: $(PROGS)


%.a:$(OBJ)
	rm -f $@
	$(AR) cru $@ $^
	$(RANLIB) $@

%.o:%.cpp  $(HEADERS)
	$(CXX) -c -o $@ $< $(OPT)
obj:
	mkdir obj
clean:
	-rm -f $(PROGS) *.o *.a
