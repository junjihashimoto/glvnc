include ./Makefile.common

SUBDIRS= \
libs \
mousemove \
cliprun \
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
