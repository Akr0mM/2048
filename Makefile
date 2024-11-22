CFLAGS = -g -Wall -Wextra -std=c99
CLIBS = -Isrc/Include -Lsrc/lib -lmingw32 -lSDL2main \
		-lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2
OBJECTS = sort.o

all: test_Sort	#default goal

test_Sort: $(OBJECTS)
	gcc $(CFLAGS) -o test_Sort $(OBJECTS) $(CLIBS)

sort.o: sort.c
	gcc $(CFLAGS) -c sort.c $(CLIBS)

run:
	test_Sort

.PHONY : clean
clean:
	-del -f *.o *.exe