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
LIBIDR  =  -L../ -L../../libnfc
NAME    =  cardreadertst
ANDROID =  cardreadertst_android
all: $(NAME) $(ANDROID)

$(NAME): $(NAME).cpp
# $(CXX) -o $(NAME) $(NAME).cpp -lnetcardreader -lnfc -lpthread --static $(INC) $(LIBIDR)
	$(CXX) -o $(NAME) $(NAME).cpp -lnetcardreader -lnfc -lpthread $(INC) $(LIBIDR)

$(ANDROID): $(ANDROID).cpp
	$(CXX) -o $(ANDROID) $(ANDROID).cpp -lnetcardreader -lnfc -lpthread --static $(INC) $(LIBIDR)

clean:
	rm -f $(NAME) $(ANDROID) *.o
