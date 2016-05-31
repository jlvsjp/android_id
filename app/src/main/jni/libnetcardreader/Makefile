CROSS_COMPILE =

CROSS     = $(CROSS_COMPILE)
CC        = $(CROSS_COMPILE)gcc $(CPUFLAGS)
AS        = $(CROSS_COMPILE)as $(CPUFLAGS)
CXX       = $(CROSS_COMPILE)g++ $(CPUFLAGS)
AR        = $(CROSS_COMPILE)ar
LD        = $(CROSS_COMPILE)ld
OBJCOPY   = $(CROSS_COMPILE)objcopy
RANLIB    = $(CROSS_COMPILE)ranlib

LIBNAME = libnetcardreader
PRG     = cardreadertst

INC     = -I. -I../libnfc
LIBIDR  = -L .

all:$(LIBNAME) prgtst
SOURCES = $(wildcard *.cpp) 
OBJS = $(patsubst %.cpp,%.o,$(SOURCES))


$(LIBNAME): $(OBJS)
	$(AR) -rcs $(LIBNAME).a *.o

%.o : %.cpp
	$(CC) -DHAVE_CONFIG_H -c $< -o $@ $(CFLAGS) $(INC) $(LIBIDR)

prgtst:
	$(MAKE) -C $(PRG)

clean:
	rm -f $(PRG)/$(PRG) $(PRG)/$(PRG)_android $(LIBNAME).a *.o
