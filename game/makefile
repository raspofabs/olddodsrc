TARGETS=test game

CC=g++
#CC=clang++
#OPTIMISE=-O2
OPTIMISE=-O0
DEBUG=-ggdb3
CFLAGS= -fno-strict-aliasing -DUNIV_LINUX \
	-Werror -Wall -Wextra $(OPTIMISE) $(DEBUG)
LIBS=-Wl,-Bsymbolic-functions \
	-rdynamic -lrt -lpthread \
	-lGL -lglfw -lXrandr

RAW_COMMONOBJECTS=GameTextures.o GameMeshes.o
RAW_TARGETOBJECTS=$(TARGETS:%=%.o)
COMMONOBJECTS=$(RAW_COMMONOBJECTS:%=build/%)
TARGETOBJECTS=$(RAW_TARGETOBJECTS:%=build/%)
ALLOBJECTS=$(COMMONOBJECTS) $(TARGETOBJECTS)
OBJSRC=$(COMMONOBJECTS:.o=.cpp)
CORE=core/core.a
TINYJS=TinyJS/TinyJS.a

recurse:
	@make -j$(shell getconf _NPROCESSORS_ONLN) -C core all
	@make -j$(shell getconf _NPROCESSORS_ONLN) -C TinyJS lib
	@make -j$(shell getconf _NPROCESSORS_ONLN) all

all: $(TARGETS)

build:
	mkdir -p build

-include $(ALLOBJECTS:.o=.d)

clean:
	@make -C core clean
	@make -C TinyJS clean
	rm -f $(TARGETS)
	rm -f build/*

build/%.o: %.cpp | build
	$(CC) $(CFLAGS) -c $*.cpp -o build/$*.o
	$(CC) $(CFLAGS) -MM $*.cpp > build/$*.d
	sed -i '1s/^/build\//' build/$*.d

# testing
test: build/test.o $(COMMONOBJECTS) $(CORE) $(TINYJS)
	$(CC) build/test.o $(COMMONOBJECTS) $(CORE) $(TINYJS) -o test $(LIBS)
game: build/game.o $(COMMONOBJECTS) $(CORE) $(TINYJS)
	$(CC) build/game.o $(COMMONOBJECTS) $(CORE) $(TINYJS) -o game $(LIBS)
oopgame: build/oopgame.o $(COMMONOBJECTS) $(CORE) $(TINYJS)
	$(CC) build/oopgame.o $(COMMONOBJECTS) $(CORE) $(TINYJS) -o oopgame $(LIBS)
dodgame: build/dodgame.o $(COMMONOBJECTS) $(CORE) $(TINYJS)
	$(CC) build/dodgame.o $(COMMONOBJECTS) $(CORE) $(TINYJS) -o dodgame $(LIBS)

compilers: basepackages

basepackages:
	sudo apt-get update -y
	sudo apt-get upgrade -y
	sudo apt-get -y install g++ libglfw-dev xorg-dev libglu1-mesa-dev

run:
	#./test
	./game
