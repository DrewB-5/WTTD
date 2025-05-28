FILE ?= main

INCLUDE = -I"C:/Users/Drew/Documents/libraries/SFML-2.6.2/include"
LIB = -L"C:/Users/Drew/Documents/libraries/SFML-2.6.2/lib"

CFLAGS = -DSFML_STATIC -std=c++17
LIBS = -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-main -lopengl32 -lfreetype -lgdi32 -lwinmm -luuid -luser32 -lkernel32

OBJS = $(FILE).o LoadingScreen.o Button.o Level.o

all: bin
	g++ -c $(FILE).cpp $(INCLUDE) $(CFLAGS)
	g++ -c src/LoadingScreen.cpp $(INCLUDE) $(CFLAGS)
	g++ -c src/Button.cpp $(INCLUDE) $(CFLAGS)
	g++ -c src/Level.cpp $(INCLUDE) $(CFLAGS)
	g++ $(OBJS) -o $(FILE).exe $(LIB) $(LIBS)
	rm $(OBJS)
	mv $(FILE).exe bin

bin:
	if not exist bin mkdir bin

clean:
	del /Q *.o *.exe 2>nul
	del /Q bin\*.exe 2>nul

test:
	bin\$(FILE).exe