glvnc
=====

vnc client using opengl


Compile for linux
-------
>sudo apt-get install libxxf86vm-dev
>sudo apt-get install libxrandr-dev 
>sudo apt-get install libcv2-dev
>ln -s Makefile.linux Makefile.common
>make

Compile for msys(windows)
-------
>ln -s Makefile.msys Makefile.common
>make

Run
-------
>src/bin/glvnc "hostname or ip" "port_number(5900+vncport)" "password"

