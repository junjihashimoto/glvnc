include ../Makefile.common

#TLIBS=$(wildcard */Makefile)
#SUBDIRS=$(TLIBS:/Makefile=)


SUBDIRS= \
vnc \
freeglut \
thread \
utils


SUBDIRS+=libjpeg
SUBDIRS+=libpng
SUBDIRS+=zlib



.PHONY : subdirs $(SUBDIRS) clean all test

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
