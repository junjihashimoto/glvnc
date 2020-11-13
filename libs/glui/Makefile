include ../../Makefile.common

.PHONY : clean all test


PROGS= \
libglui.a


LIBSRC=\
algebra3.cpp \
arcball.cpp \
glui.cpp \
glui_add_controls.cpp \
glui_bitmap_img_data.cpp \
glui_bitmaps.cpp \
glui_button.cpp \
glui_checkbox.cpp \
glui_column.cpp \
glui_commandline.cpp \
glui_control.cpp \
glui_edittext.cpp \
glui_filebrowser.cpp \
glui_img_checkbox_0.c \
glui_img_checkbox_0_dis.c \
glui_img_checkbox_1.c \
glui_img_checkbox_1_dis.c \
glui_img_downarrow.c \
glui_img_leftarrow.c \
glui_img_listbox_down.c \
glui_img_listbox_up.c \
glui_img_listbox_up_dis.c \
glui_img_radiobutton_0.c \
glui_img_radiobutton_0_dis.c \
glui_img_radiobutton_1.c \
glui_img_radiobutton_1_dis.c \
glui_img_rightarrow.c \
glui_img_spindown_0.c \
glui_img_spindown_1.c \
glui_img_spindown_dis.c \
glui_img_spinup_0.c \
glui_img_spinup_1.c \
glui_img_spinup_dis.c \
glui_img_uparrow.c \
glui_list.cpp \
glui_listbox.cpp \
glui_mouse_iaction.cpp \
glui_node.cpp \
glui_panel.cpp \
glui_radio.cpp \
glui_rollout.cpp \
glui_rotation.cpp \
glui_scrollbar.cpp \
glui_separator.cpp \
glui_spinner.cpp \
glui_statictext.cpp \
glui_string.cpp \
glui_textbox.cpp \
glui_translation.cpp \
glui_tree.cpp \
glui_treepanel.cpp \
glui_window.cpp \
quaternion.cpp \
viewmodel.cpp


HEADERS=\
./GL/glui.h \
./internal/arcball.h \
./internal/quaternion.h \
./internal/glui_internal.h \
./internal/glui_internal_control.h \
./internal/viewmodel.h \
./internal/algebra3.h


CFLAGS=-O4 -DNDEBUG

ifdef WIN32
OPT  = $(CFLAGS) -Iinternal -I. -I../freeglut
else
OPT  = $(CFLAGS) -Iinternal -I. -DHAVE_ERRNO_H -DHAVE_USBHID_H -DHAVE_USB_JS -DHAVE_VFPRINTF -DHAVE_LIMITS_H -DHAVE_GETTIMEOFDAY  -DHAVE_SYS_TYPES_H -DHAVE_UNISTD_H   -DHAVE_STDBOOL_H -DHAVE_FCNTL_H -DHAVE_SYS_IOCTL_H -DHAVE_SYS_PARAM_H  -I../freeglut
endif


OBJ   = $(patsubst %.c, %.o  , $(LIBSRC) ) $(patsubst %.cpp, %.o  , $(LIBSRC) ) 

all: $(PROGS)


libglui.a:$(OBJ)
	rm -f $@
	$(AR) cru $@ $^
	$(RANLIB) $@

%.o:%.c  $(HEADERS)
	$(CC) -c -o $@ $< $(OPT)
%.o:%.cpp  $(HEADERS)
	$(CXX) -c -o $@ $< $(OPT)
obj:
	mkdir obj
clean:
	-rm -f $(PROGS) *.o *.a
