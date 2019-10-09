EXEC=project1
CFLAGS = -c -O3 -Wall
LIBS = storage.o
OTHERS = storage.h

all: $(EXEC)

.c.o:
	$(CC) $(CFLAGS) $< -o $@


$(EXEC): $(EXEC).o $(LIBS)
	gcc $(EXEC).o $(LIBS) -o $(EXEC)

clean:
	rm -f *.o $(EXEC) $(LIBS)

zip:
	zip project1.zip README.txt project1.c Makefile storage.c storage.h


install:
	cp project1_dist.tar ~/notepad/classes/cs3113/export/projects

tar_release:
	tar -cvf project1_release.tar Makefile *.h *.c
