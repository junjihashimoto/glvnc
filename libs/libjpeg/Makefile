include ../../Makefile.common

.PHONY : clean all test


PROGS= \
libjpeg.a 

LIBSRC=\
jaricom.c \
jcapimin.c \
jcapistd.c \
jcarith.c \
jccoefct.c \
jccolor.c \
jcdctmgr.c \
jchuff.c \
jcinit.c \
jcmainct.c \
jcmarker.c \
jcmaster.c \
jcomapi.c \
jcparam.c \
jcprepct.c \
jcsample.c \
jctrans.c \
jdapimin.c \
jdapistd.c \
jdarith.c \
jdatadst.c \
jdatasrc.c \
jdcoefct.c \
jdcolor.c \
jddctmgr.c \
jdhuff.c \
jdinput.c \
jdmainct.c \
jdmarker.c \
jdmaster.c \
jdmerge.c \
jdpostct.c \
jdsample.c \
jdtrans.c \
jerror.c \
jfdctflt.c \
jfdctfst.c \
jfdctint.c \
jidctflt.c \
jidctfst.c \
jidctint.c \
jmemmgr.c \
jmemnobs.c \
jquant1.c \
jquant2.c \
jutils.c


HEADERS=\
jconfig.h \
jerror.h \
jmorecfg.h \
jpeglib.h



OPT  = $(CFLAGS)

OBJ   = $(patsubst %.c, %.o  , $(LIBSRC) ) 

all: $(PROGS)


libjpeg.a:$(OBJ)
	rm -f $@
	$(AR) cru $@ $^
	$(RANLIB) $@

%.o:%.c  $(HEADERS)
	$(CC) -c -o $@ $< $(OPT)
obj:
	mkdir obj
clean:
	-rm -f $(PROGS) *.o *.a  *~
