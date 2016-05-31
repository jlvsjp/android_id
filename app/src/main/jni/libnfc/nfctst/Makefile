CROSS_COMPILE =

CROSS     = $(CROSS_COMPILE)
CC        = $(CROSS_COMPILE)gcc $(CPUFLAGS)
AS        = $(CROSS_COMPILE)as $(CPUFLAGS)
CXX       = $(CROSS_COMPILE)g++ $(CPUFLAGS)
AR        = $(CROSS_COMPILE)ar
LD        = $(CROSS_COMPILE)ld
OBJCOPY   = $(CROSS_COMPILE)objcopy
RANLIB    = $(CROSS_COMPILE)ranlib

INC     =  -I../
LIBIDR  =  -L../
NAME    =  nfctst
ANDROID =  nfctst_android

all: $(NAME) $(ANDROID)

$(NAME): $(NAME).c
	$(CC) -o $(NAME) $(NAME).c -lnfc -lpthread $(INC) $(LIBIDR)

$(ANDROID): $(ANDROID).c
	$(CC) -o $(ANDROID) $(ANDROID).c -lnfc -lpthread --static $(INC) $(LIBIDR)

clean:
	rm -f $(NAME) $(ANDROID) *.o
