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

ifdef WIN32
OTHER_LIBS= ../libs/freeglut/libglut.a -lopengl32 -lglu32 -lwinmm -lgdi32 -lm
else
OTHER_LIBS=  ../libs/freeglut/libglut.a   -lGL -lGLU -lm -ldl -lXrandr -llapack  -latlas -lblas -lgfortran -lXxf86vm -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -lopencv_video
endif

HEADERS=$(shell find .. -name '*.h')

CFLAGS+=-I../libs/libbmp -I../libs/utils -I../libs/freeglut -DFREEGLUT_STATIC  -I../libs/thread
#CFLAGS+=-I../libs/libbmp -I../libs/utils -I/home/junji/odviewer/libs/freeglut.bak/include  -DFREEGLUT_STATIC

all: objs progs

objs:$(OBJS)


progs:$(PROGRAMS)

clean:
	-rm -rf obj/*
	-rm -rf $(PROGRAMS)

obj/%.o:%.cpp $(HEADERS)
	$(CXX) -c -o $@ $< $(CFLAGS)


bin/%$(EXE):obj/%.o $(LIBS) $(NPROG)
	$(CXX) -o $@ $<  $(CFLAGS) $(NPROG) $(LIBS) $(OTHER_LIBS)
#	cp ../bin/template ../$@
