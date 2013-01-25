include ../Makefile.common

#TLIBS=$(wildcard */Makefile)
#SUBDIRS=$(TLIBS:/Makefile=)


SUBDIRS= \
vnc \
freeglut \
glui \
thread \
utils


ifndef WIN32
#SUBDIRS+=matrix
else
SUBDIRS+=libjpeg
SUBDIRS+=libpng
SUBDIRS+=zlib
endif



.PHONY : subdirs $(SUBDIRS) clean all test

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
