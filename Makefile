include ./Makefile.common

SUBDIRS= \
libs \
src



.PHONY : subdirs $(SUBDIRS) clean all test


all:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

$(SUBDIRS):
	$(MAKE) -s -C $@

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
