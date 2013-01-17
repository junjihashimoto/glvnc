include ../Makefile.common

.PHONY : clean all test

TMPVAL='main('
TPROGRAMS=$(shell grep -l $(TMPVAL) *.cpp)
PROGRAMS=$(TPROGRAMS:%.cpp=bin/%$(EXE))

TNPROG=$(shell grep -L $(TMPVAL) *.cpp)
NPROG=$(TNPROG:%.cpp=obj/%.o)

TOBJS=$(wildcard *.cpp)
OBJS=$(TOBJS:%.cpp=obj/%.o)


#LIBS=$(shell find .. -name '*.a')
#OTHER_LIBS=-ldl -llapack -latlas -lm
#OTHER_LIBS=-ldl -L/home4/md2/WORK/PSRX3/hasimoto/GotoBLAS2 -lgoto2 -lgfortran  -lfftw3 -lm
#OTHER_LIBS=-ldl -lGL -lGLEW -lglut -lGLU -lm

HEADERS=$(shell find .. -name '*.h')

CFLAGS+=-I../libs/libbmp
CFLAGS+=-I../libs/utils
CFLAGS+=-I../libs/freeglut -DFREEGLUT_STATIC
CFLAGS+=-I../libs/thread
CFLAGS+=-I../libs/vnc

OTHER_LIBS= ../libs/freeglut/libglut.a
OTHER_LIBS+= ../libs/vnc/libvnc.a

ifdef WIN32
OTHER_LIBS+= ../libs/libjpeg/libjpeg.a
OTHER_LIBS+= ../libs/libpng/libpng.a
OTHER_LIBS+= ../libs/zlib/libz.a
OTHER_LIBS+= -lopengl32 -lglu32 -lwinmm -lgdi32 -lws2_32 -lshell32 -lm
CFLAGS+=-I../libs/libjpeg -I../libs/libpng -I../libs/zlib

else
OTHER_LIBS+= -lGL -lGLU -lm -ldl -lXrandr -lXxf86vm
OTHER_LIBS+= -llapack  -latlas -lblas -lgfortran
OTHER_LIBS+= -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -lopencv_video
endif



all: objs progs

objs:$(OBJS)


progs:$(PROGRAMS)

clean:
	-rm -rf obj/*
	-rm -rf $(PROGRAMS)

obj/%.o:%.cpp $(HEADERS)
	-if [ ! -d obj ] ; then mkdir obj ;fi
	$(CXX) -c -o $@ $< $(CFLAGS)


bin/%$(EXE):obj/%.o $(LIBS) $(NPROG)
	-if [ ! -d bin ] ; then mkdir bin ;fi
	$(CXX) -o $@ $<  $(CFLAGS) $(NPROG) $(LIBS) $(OTHER_LIBS)
#	cp ../bin/template ../$@

#check-syntax:
#	$(CXX)  $(CFLAGS)  -Wall -Wextra -pedantic -fsyntax-only $(CHK_SOURCES)
