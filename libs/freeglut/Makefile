include ../../Makefile.common

.PHONY : clean all test


PROGS= \
libglut.a


LIBSRC=\
freeglut_callbacks.c \
freeglut_cursor.c \
freeglut_display.c \
freeglut_ext.c \
freeglut_font.c \
freeglut_font_data.c \
freeglut_gamemode.c \
freeglut_geometry.c \
freeglut_glutfont_definitions.c \
freeglut_init.c \
freeglut_input_devices.c \
freeglut_joystick.c \
freeglut_main.c \
freeglut_menu.c \
freeglut_misc.c \
freeglut_overlay.c \
freeglut_spaceball.c \
freeglut_state.c \
freeglut_stroke_mono_roman.c \
freeglut_stroke_roman.c \
freeglut_structure.c \
freeglut_teapot.c \
freeglut_videoresize.c \
freeglut_window.c \
freeglut_xinput.c


HEADERS=\
./GL/freeglut.h \
./GL/glut.h \
./GL/freeglut_ext.h \
./GL/freeglut_std.h \
./internal/freeglut_teapot_data.h \
./internal/freeglut_internal.h


#CFLAGS=-O4 -DNDEBUG


ifdef WIN32
OPT  = $(CFLAGS) -I. -Iinternal  -DFREEGLUT_STATIC
else
OPT  = $(CFLAGS) -I. -Iinternal  \
-DHAVE_ERRNO_H \
-DHAVE_USBHID_H \
-DHAVE_USB_JS \
-DHAVE_VFPRINTF \
-DHAVE_LIMITS_H \
-DHAVE_GETTIMEOFDAY \
-DHAVE_SYS_TYPES_H \
-DHAVE_UNISTD_H \
-DHAVE_STDBOOL_H \
-DHAVE_FCNTL_H \
-DHAVE_SYS_IOCTL_H \
-DHAVE_SYS_PARAM_H \
-DHAVE_X11_EXTENSIONS_XF86VMODE_H \
-DHAVE_X11_EXTENSIONS_XRANDR_H \
-DFREEGLUT_STATIC
endif


OBJ   = $(patsubst %.c, %.o  , $(LIBSRC) ) 

all: $(PROGS)


libglut.a:$(OBJ)
	rm -f $@
	$(AR) cru $@ $^
	$(RANLIB) $@

%.o:%.c  $(HEADERS)
	$(CC) -c -o $@ $< $(OPT)
obj:
	mkdir obj
clean:
	-rm -f $(PROGS) *.o *.a
