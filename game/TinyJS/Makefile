CC=g++
#CC=clang++
CFLAGS=-c -g -Wall -rdynamic -D_DEBUG
LDFLAGS=-g -rdynamic

SOURCES=  \
TinyJS.cpp \
TinyJS_Functions.cpp \
TinyJS_MathFunctions.cpp

OBJECTS=$(SOURCES:.cpp=.o)

all: run_tests Script
lib: TinyJS.a

TinyJS.a: $(OBJECTS)
	ar rcs TinyJS.a $(OBJECTS)

run_tests: run_tests.o $(OBJECTS)
	$(CC) $(LDFLAGS) run_tests.o $(OBJECTS) -o $@

Script: Script.o $(OBJECTS)
	$(CC) $(LDFLAGS) Script.o $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f run_tests Script run_tests.o Script.o $(OBJECTS)
