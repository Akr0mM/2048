CFLAGS = -g -Wall -Wextra -std=c99
CLIBS = -Isrc/Include -Lsrc/lib -lmingw32 -lSDL2main \
		-lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2
OBJECTS = main.o

all: test_2048	#default goal

test_2048: $(OBJECTS)
	gcc $(CFLAGS) -o test_2048 $(OBJECTS) $(CLIBS)

main.o: main.c
	gcc $(CFLAGS) -c main.c $(CLIBS)

run:
	test_2048

.PHONY : clean
clean:
	-del -f *.o *.exe