# Usage: make FILE=main (without .cpp)
FILE ?= main

# Include and lib paths (adjust if needed)
INCLUDE = -I"C:/Users/Drew/Documents/libraries/SFML-2.6.2/include"
LIB = -L"C:/Users/Drew/Documents/libraries/SFML-2.6.2/lib"

# Flags
CFLAGS = -DSFML_STATIC -std=c++17
LIBS = -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-main -lopengl32 -lfreetype -lgdi32 -lwinmm -luuid -luser32 -lkernel32

# Object files
OBJS = $(FILE).o LoadingScreen.o Button.o

all: bin
	g++ -c $(FILE).cpp $(INCLUDE) $(CFLAGS)
	g++ -c LoadingScreen.cpp $(INCLUDE) $(CFLAGS)
	g++ -c Button.cpp $(INCLUDE) $(CFLAGS)
	g++ $(OBJS) -o $(FILE).exe $(LIB) $(LIBS)
	mv $(FILE).exe bin
	rm $(OBJS)

bin:
	if not exist bin mkdir bin

clean:
	rm /Q *.o *.exe
	rm /Q bin\*.exe

test:
	bin\$(FILE).exe
