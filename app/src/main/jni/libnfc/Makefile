CROSS_COMPILE =

CROSS     = $(CROSS_COMPILE)
CC        = $(CROSS_COMPILE)gcc $(CPUFLAGS)
AS        = $(CROSS_COMPILE)as $(CPUFLAGS)
CXX       = $(CROSS_COMPILE)g++ $(CPUFLAGS)
AR        = $(CROSS_COMPILE)ar
LD        = $(CROSS_COMPILE)ld
OBJCOPY   = $(CROSS_COMPILE)objcopy
RANLIB    = $(CROSS_COMPILE)ranlib

LIBNAME = libnfc
PRG     = nfctst

INC     = -I. -I./include -I../../../qt-linux/iTop4412_Kernel_3.0/include/
LIBIDR  = -L .

all:$(LIBNAME) prgtst
SOURCES = $(wildcard *.c) 
OBJS = $(patsubst %.c,%.o,$(SOURCES))


$(LIBNAME): $(OBJS)
	$(AR) -rcs $(LIBNAME).a *.o

%.o : %.c
	$(CC) -DHAVE_CONFIG_H -c $< -o $@ $(CFLAGS) $(INC) $(LIBIDR)

prgtst:
	$(MAKE) -C $(PRG)

clean:
	rm -f $(PRG)/$(PRG) $(PRG)/$(PRG)_android $(LIBNAME).a *.o
