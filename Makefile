



all: project2 server

server.o: server.c
	gcc -c server.c

storage.o: storage.c
	gcc -c storage.c

server: server.o storage.o
	gcc server.o storage.o -o server

project2.o: project2.c
	gcc -c project2.c

storage_remote.o: storage_remote.c
	gcc -c storage_remote.c

project2: project2.o storage_remote.o
	gcc project2.o storage_remote.o -o project2

clean:
	rm -f *.o server project2 pipe_in pipe_out

pipes:
	mkfifo pipe_in pipe_out

zip:
	zip project2.zip README.txt project2.c Makefile storage.c storage.h

tar_release:
	tar -cvf project2_release.tar Makefile *.h *.c

