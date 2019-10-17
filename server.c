#include "storage.h"
#include "comm.h"
#include "storage_common.h"
#include <string.h>

#define BUFSIZE 200

int main(int argc, char** argv)
{
  unsigned char buffer[BUFSIZE];
  HEADER header;
  HEADER header_out;
  int ret;
  STORAGE *storage;
  mkfifo("test_pipe", 0666);
  int fd_out;
  int fd_in;

  // Loop forever (break out with SHUTDOWN)
  while(1) {
    fprintf(stderr, "Waiting for connection with client...\n");

    // Open to_storage pipe
    if (fd_in = open("test_pipe", O_RDONLY) == -1)
    {
      fprintf(stderr, "Couldn't open pipe\n");
      exit(-1);
    }
    printf("opened read pipe\n");
    /*if (fd_out = open(PIPE_NAME_FROM_STORAGE, O_WRONLY) == -1)
    {
      fprintf(stderr, "Couldn't open pipe\n");
      exit(-1);
    }
    printf("opened write pipe\n");

    char buf[10] = "ayy lmao";

    if (write(fd_out, buf, sizeof(buf)) < 0)
    {
      printf("fail\n");
    }*/

    if (read(fd_in, &header, sizeof(HEADER)) == sizeof(HEADER))
    {
      fprintf(stdout, "actually read something lmao\n");
    }

    if (read(fd_in, &header, sizeof(HEADER)) < 0)
    {
      fprintf(stderr, "Couldn't read header\n");
      exit(-1);
    }
    printf("read pipe\n");
    





    // We broke out because of a disconnection: clean up
    fprintf(stderr, "Closing connection\n");
    close(fd_in);
    close(fd_out);
    close_storage(storage);
  }

  // Should never reach here
  return(0);
}
