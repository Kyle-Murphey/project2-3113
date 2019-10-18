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
  mkfifo("pipe_in", 0666);
  mkfifo("pipe_out", 0666);
  int fd_out;
  int fd_in;

  // Loop forever (break out with SHUTDOWN)
  while(1) {
    fprintf(stderr, "Waiting for connection with client...\n");

    // Open to_storage pipe
    fd_in = open(PIPE_NAME_TO_STORAGE, O_RDONLY);
    if (fd_in == -1)
    {
      fprintf(stderr, "Couldn't open pipe\n");
      exit(-1);
    }
    printf("opened read pipe\n");

    int reader = read(fd_in, &header, sizeof(HEADER));

    if (reader != sizeof(HEADER))
    {
      fprintf(stderr, "Couldn't read header\n");
      exit(-1);
    }
    printf("read pipe\n");

    if (header.type == INIT_CONNECTION)
    {
      while(1)
      {
        sleep(5);
      }
    }
    





    // We broke out because of a disconnection: clean up
    fprintf(stderr, "Closing connection\n");
    close(fd_in);
    close(fd_out);
    close_storage(storage);
  }

  // Should never reach here
  return(0);
}
