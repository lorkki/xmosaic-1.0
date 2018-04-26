# ----------------------------------------------------------------------------
# For normal machines with normal compilers:
# CC = cc
# For Sun's and other non-at-least-pseudo-ANSI-C platforms:
CC = gcc
# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# For modern GCC:
CFLAGS = -g -ansi -DMOTIF -DMOTIF1_2 -DXMOSAIC
# For SGI's:
# CFLAGS = -g -cckr -DMOTIF -DXMOSAIC
# For HP's:
# CFLAGS = -g -DMOTIF -DXMOSAIC -I/usr/include/Motif1.1 -I/usr/include/X11R4
# For SCO:
# CFLAGS = -g -DMOTIF -DMOTIF1_2 -DXMOSAIC
# For NeXT:
# CFLAGS = -g -DNEXT -I/usr/include/X11
# For everyone else:
# CFLAGS = -g -DMOTIF -DXMOSAIC
# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# For SGI's:
RANLIB = /bin/true
# For everyone else:
# RANLIB = ranlib
# ----------------------------------------------------------------------------

# You shouldn't need to edit below here.

LIBTARGET = libhtmlw.a

all: $(LIBTARGET)

CFILES = HTML.c HTMLlists.c HTMLformat.c HTMLparse.c HTMLimages.c

OBJS = $(CFILES:.c=.o)

$(LIBTARGET): $(OBJS)
	-rm -f $(LIBTARGET)
	ar rv $(LIBTARGET) $(OBJS)
	$(RANLIB) $(LIBTARGET)

clean:
	-rm $(LIBTARGET) *.o
tags:
	etags -t *.[ch]
