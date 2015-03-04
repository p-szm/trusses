CC = g++
CCSW = -O3 -Wno-deprecated-declarations
INC = -I../Headers -I../External
LIBS = -lGL -lGLU -lglut

all:
	$(CC) -o trusses bar.cpp button.cpp graphics.cpp interface.cpp interpreter.cpp main.cpp math.cpp particle.cpp physics.cpp save.cpp temporary_label.cpp wall.cpp ${CCSW} $(LIBS);
