include ../../Makefile.common
HEADERS=thread.h

LIBNAME=libthread

SRC= $(wildcard *.cpp)

OBJ=$(patsubst %.cpp,%.o,$(SRC))

#OPT=$(CFLAGS)
OPT=-O4 -DNDEBUG 


all:$(LIBNAME).a

$(LIBNAME).a:$(OBJ)
	rm -f $@
	$(AR) cru $@ $^
	$(RANLIB) $@

%.o:%.cpp $(HEADERS)
	$(CXX) -c -o $@ $< $(OPT)

clean:
	rm -f *.a *.o
