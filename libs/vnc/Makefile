include ../../Makefile.common
HEADERS=$(wildcard *.h)

LIBNAME=libvnc

SRC= $(wildcard *.cpp)

OBJ=$(patsubst %.cpp,%.o,$(SRC))

#OPT=$(CFLAGS)
#OPT=-O4 -DNDEBUG
OPT=-O4 -I../libbmp -I../thread


all:$(LIBNAME).a

$(LIBNAME).a:$(OBJ)
	rm -f $@
	$(AR) cru $@ $^
	$(RANLIB) $@

%.o:%.cpp $(HEADERS)
	$(CXX) -c -o $@ $< $(OPT)

clean:
	rm -f *.a *.o
