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
    ///

    //open write pipe
    fd_out = open(PIPE_NAME_FROM_STORAGE, O_WRONLY);
    if (fd_out == -1)
    {
      fprintf(stderr, "Couldn't open pipe\n");
      exit(-1);
    }
    printf("opened write pipe\n");
    ///

    //read ACK
    int reader = read(fd_in, &header, sizeof(HEADER));
    if (reader != sizeof(HEADER))
    {
      fprintf(stderr, "Couldn't read header\n");
      exit(-1);
    }
    printf("read pipe\n");
    ///

    //init connection
    if (header.type == INIT_CONNECTION)
    {
      /* create array to hold file name */
      char name[header.len_message + 1];
      memset(name, 0, sizeof(name));

      //get file name
      reader = read(fd_in, name, header.len_message);
      if (reader != header.len_message)
      {
        fprintf(stderr, "Couldn't get file name\n");
        exit(-1);
      }
      printf("got file name\n");
      ///

      //create/find file
      storage = malloc(sizeof(STORAGE));
      storage = init_storage(name);

      header_out.len_message = sizeof(storage);
      header_out.type = ACKNOWLEDGE;
      header_out.len_buffer = -1;
      header_out.location = -1;
      sleep(1);

      //send ack
      int written = write(fd_out, &header_out, header_out.len_message);
      if (written != header_out.len_message)
      {
        fprintf(stderr, "Couldn't send ACK\n");
        exit(-1);
      }
      printf("ACKNOWLEDGE sent\n");
      ///

      //send file to client
      written = write(fd_out, storage, sizeof(storage));
      printf("size of written: %d\n", written);
      printf("STORAGE: %ld\n", sizeof(STORAGE));
      printf("storage: %ld\n", sizeof(storage));
      if (written != sizeof(storage))
      {
        fprintf(stderr, "Couldn't send file\n");
        exit(-1);
      }
      printf("sent file\n");
      ///

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
