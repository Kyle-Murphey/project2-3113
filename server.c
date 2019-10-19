#include "storage.h"
#include "comm.h"
#include "storage_common.h"
#include <string.h>
/*
* Kyle Murphey
* Project 2
*/

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
    printf("Connection established\n");
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
      if (written != sizeof(storage))
      {
        fprintf(stderr, "Couldn't send file\n");
        exit(-1);
      }
      printf("sent file\n");
      ///

      //client loop
      while(1)
      {
        //get header to process client's request
        reader = read(fd_in, &header, sizeof(HEADER));
        if (reader != sizeof(HEADER))
        {
          fprintf(stderr, "Couldn't read header\n");
          exit(-1);
        }
        printf("Recieved HEADER\n");
        ///

        //client requesting to send data to the file
        if (header.type == WRITE_REQUEST)
        {
          //get data to write to file
          reader = read(fd_in, buffer, header.len_message);
          if (reader != header.len_message)
          {
            fprintf(stderr, "Couldn't get data to write\n");
            exit(-1);
          }
          printf("Recieved data to write\n");
          ///

          //writing bytes to the file
          written = put_bytes(storage, buffer, header.location, header.len_buffer);
          if (written != header.len_message)
          {
            fprintf(stderr, "Couldn't write to the file\n");
            exit(-1);
          }
          printf("File written to\n");
          ///

          header_out.type = ACKNOWLEDGE;
          header_out.len_message = 0;
          header_out.len_buffer = -1;
          header_out.location = -1;

          //send ACK to client
          written = write(fd_out, &header_out, sizeof(HEADER));
          if (written != sizeof(HEADER))
          {
            fprintf(stderr, "Couldn't send ACK\n");
            exit(-1);
          }
          printf("Sent ACKNOWLEDGE\n");
          ///
        }
        //client requesting data from the file
        else if (header.type == READ_REQUEST)
        {
          //reads the bytes from the file
          reader = get_bytes(storage, buffer, header.location, header.len_buffer);
          if (reader != header.len_buffer)
          {
            fprintf(stderr, "Couldn't read from file\n");
            exit(-1);
          }
          printf("data read from file\n");
          ///

          header_out.type = DATA;
          header_out.len_message = header.len_buffer;
          header_out.location = header.location;
          header_out.len_buffer = header.len_buffer;

          //sends the DATA header
          int written = write(fd_out, &header_out, sizeof(HEADER));
          if (written != sizeof(HEADER))
          {
            fprintf(stderr, "Couldn't send DATA\n");
            exit(-1);
          }
          printf("sent DATA\n");
          ///

          //send data to the client buffer
          written = write(fd_out, buffer, header_out.len_message);
          if (written != header_out.len_message)
          {
            fprintf(stderr, "Couldn't send requested data\n");
            exit(-1);
          }
          printf("sent data\n");
          ///
        }
        //client disconnects
        else if (header.type == SHUTDOWN)
        {
          header_out.type = ACKNOWLEDGE;
          header_out.len_message = 0;
          header_out.len_buffer = -1;
          header_out.location = -1;

          //send ACK to client
          int written = write(fd_out, &header_out, sizeof(HEADER));
          if (written != sizeof(HEADER))
          {
            fprintf(stderr, "Couldn't send ACK\n");
            exit(-1);
          }
          printf("ACK sent\n");
          ///

          sleep(1);
          break;
        }
        else
        {
          fprintf(stderr, "Bad header\n");
          exit(-1);
        }
      }
    }

    // We broke out because of a disconnection: clean up
    fprintf(stderr, "Closing connection\n");
    close(fd_in);
    close(fd_out);
    close_storage(storage);
  }

  free(storage);
  // Should never reach here
  return(0);
}
