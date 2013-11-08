TARGETS=core.a

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

RAW_COMMONOBJECTS=util.o graphics.o BadMesh.o Shader.o assetloader.o FontRender.o stbi_image.o core.o geom.o
COMMONOBJECTS=$(RAW_COMMONOBJECTS:%=build/%)
OBJSRC=$(COMMONOBJECTS:.o=.cpp)

all: build $(TARGETS)

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

core.a: $(COMMONOBJECTS)
	ar rcs core.a $(COMMONOBJECTS)

