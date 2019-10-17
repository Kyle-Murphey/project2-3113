EXEC=project2
CFLAGS = -g -Wall
LIBS = storage.o server.o storage_remote.o 
OTHERS = storage.h storage_common.h storage_remote.h

all: $(EXEC)

.c.o:
	$(CC) $(CFLAGS) $< -o $@


$(EXEC): $(EXEC).o $(LIBS)
	gcc $(EXEC).o $(LIBS) -o $(EXEC)

clean:
	rm -f *.o $(EXEC) $(LIBS)

pipes:
	mkfifo pipe_in pipe_out

zip:
	zip project2.zip README.txt project2.c Makefile storage.c storage.h

tar_release:
	tar -cvf project2_release.tar Makefile *.h *.c
