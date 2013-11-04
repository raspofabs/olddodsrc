TARGETS=test

CC=g++
#OPTIMISE=-O2
OPTIMISE=-O0
DEBUG=-ggdb3
CFLAGS= -fno-strict-aliasing -DUNIV_LINUX \
	-Werror -Wall -Wextra $(OPTIMISE) $(DEBUG)
LIBS=-Wl,-Bsymbolic-functions \
	-rdynamic -lrt -lpthread \
	-lGL -lglfw -lXrandr

RAW_COMMONOBJECTS=util.o graphics.o BadMesh.o Shader.o assetloader.o FontRender.o stbi_image.o core.o
RAW_TARGETOBJECTS=$(TARGETS:%=%.o)

COMMONOBJECTS=$(RAW_COMMONOBJECTS:%=build/%)
TARGETOBJECTS=$(RAW_TARGETOBJECTS:%=build/%)

ALLOBJECTS=$(COMMONOBJECTS) $(TARGETOBJECTS)
OBJSRC=$(COMMONOBJECTS:.o=.cpp)

recurse:
	@make -j$(shell getconf _NPROCESSORS_ONLN) all

all: $(TARGETS)

build:
	mkdir -p build

-include $(ALLOBJECTS:.o=.d)

clean:
	rm -f $(TARGETS)
	rm -f build/*

build/%.o: %.cpp | build
	$(CC) $(CFLAGS) -c $*.cpp -o build/$*.o
	$(CC) $(CFLAGS) -MM $*.cpp > build/$*.d
	sed -i '1s/^/build\//' build/$*.d

# testing
test: build/test.o $(COMMONOBJECTS)
	$(CC) build/test.o $(COMMONOBJECTS) -o test $(LIBS)
oopgame: build/oopgame.o $(COMMONOBJECTS)
	$(CC) build/oopgame.o $(COMMONOBJECTS) -o oopgame $(LIBS)
dodgame: build/dodgame.o $(COMMONOBJECTS)
	$(CC) build/dodgame.o $(COMMONOBJECTS) -o dodgame $(LIBS)

compilers: basepackages

basepackages:
	sudo apt-get update -y
	sudo apt-get upgrade -y
	sudo apt-get -y install g++ libglfw-dev xorg-dev libglu1-mesa-dev

run:
	./test
