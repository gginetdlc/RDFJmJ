CC=gcc
CFLAGS= -I. -lm -fno-stack-protector
#DEBUG
#CFLAGS= -I. -lm -fno-stack-protector -g 
DEPS = scripts/struct_support.h scripts/md5.h
OBJ = scripts/struct.o scripts/struct_support.o scripts/md5.o

all: run

run: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

struct.o: scripts/struct.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

struct_support.o: scripts/struct_support.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

md5.o: md5.c $(DEPS)
	$(CC) -c -o  $@ $< $(CFLAGS)

clean:
	rm -rf scripts/*o scripts/structmake scripts/md5 run
