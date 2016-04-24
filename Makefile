CC=gcc
# TODO: wow when adding -O3 the code breaks completely! 
CFLAGS=`sdl-config --cflags`  
LFLAGS=`sdl-config --libs` -lm  
SRC=main.c md2.c primitives.c 3dmath.c 
OBJ=main.o md2.o primitives.o 3dmath.o

all: swan

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 

swan: $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS) 

debug: $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS) 

clean:
	rm -f $(OBJ) swan debug
