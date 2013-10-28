TARGETS=game

CC=g++
#OPTIMISE=-O2
OPTIMISE=-O0
DEBUG=-ggdb3
C11FLAGS= -fno-strict-aliasing -DUNIV_LINUX \
	-std=c++0x -std=gnu++0x \
	-Werror -Wall -Wextra $(OPTIMISE) $(DEBUG)
LIBS=-Wl,-Bsymbolic-functions \
	-rdynamic -lrt -lpthread -lsqlite3 -lcurl \
	-lGL -lglfw -lXrandr

RAW_COMMONOBJECTS=util.o graphics.o BadMesh.o Shader.o Thing.o assetloader.o FontRender.o stbi_image.o core.o
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
	$(CC) $(C11FLAGS) -c $*.cpp -o build/$*.o
	$(CC) $(C11FLAGS) -MM $*.cpp > build/$*.d
	sed -i '1s/^/build\//' build/$*.d

# testing
game: build/game.o $(COMMONOBJECTS)
	$(CC) build/game.o $(COMMONOBJECTS) -o game $(LIBS)

compilers: basepackages cpp11

basepackages:
	sudo apt-get -y install python-software-properties
	sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
	sudo apt-get update -y
	sudo apt-get upgrade -y
	sudo apt-get -y install g++

cpp11:
	sudo apt-get install -y gcc-4.8 g++-4.8
	sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 50
	sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 50
	#sudo apt-get -y install libgtk-3-dev

run:
	./game
