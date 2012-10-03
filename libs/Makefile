include ../Makefile.common

#TLIBS=$(wildcard */Makefile)
#SUBDIRS=$(TLIBS:/Makefile=)


SUBDIRS= \
freeglut \
thread \
utils


# ifndef WIN32
# SUBDIRS+=matrix
# else
# SUBDIRS+=blas
# SUBDIRS+=lapack
# endif



.PHONY : subdirs $(SUBDIRS) clean all test

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
