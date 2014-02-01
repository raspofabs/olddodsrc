TARGETS=lua.a

CC=gcc
OPTIMISE=-O0
DEBUG=-ggdb3
CFLAGS= -fno-strict-aliasing -DUNIV_LINUX \
	-Werror -Wall -Wextra $(OPTIMISE) $(DEBUG)
LIBS=-Wl,-Bsymbolic-functions \
	-rdynamic -lrt -lpthread \
	-lGL -lglfw -lXrandr

OBJECTS=lapi.o lauxlib.o lbaselib.o lbitlib.o lcode.o lcorolib.o \
	lctype.o ldblib.o ldebug.o ldo.o ldump.o lfunc.o lgc.o linit.o \
	liolib.o llex.o lmathlib.o lmem.o loadlib.o lobject.o lopcodes.o \
	loslib.o lparser.o lstate.o lstring.o lstrlib.o ltable.o ltablib.o \
	ltm.o lua.o luac.o lundump.o lvm.o lzio.o

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
	rm -f *.o

%.o: %.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $*.o

lua.a: $(OBJECTS)
	ar rcs lua.a $(OBJECTS)

