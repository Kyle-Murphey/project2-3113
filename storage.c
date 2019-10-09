#include "storage.h"

/*
 *  Project 2
 * Kyle Murphey
 * 10/17/19
 */

/* struct to hold the file descriptor */
typedef struct 
{
  int fd;
} STORAGE;

/*
 * opens/creates a file called @name, prints error if it can't open file
 * @param:name = name of the file to ope/create
 * @return = the open file (file descriptor)
 */
STORAGE * init_storage(char * name)
{
    STORAGE * file = malloc(sizeof(STORAGE)); //allocates space for the file
    file->fd = open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR); //opens/creates the file

    //error opening/creating
    if (file->fd == -1)
    {
      fprintf(stderr, "Could not open or create file\n");
      return NULL;
    }
    return file;
}

/*
 * closes the file, prints error if can't close
 * @param:storage = file to close
 * @return = -1 if file can't close
 */
int close_storage(STORAGE *storage)
{
  int isClosed = close(storage->fd); //attempts to close file

  //file didn't close
  if (isClosed < 0)
  {
    fprintf(stderr, "error closing file\n");
  }
  return isClosed;
}

/*
 *  sets the offset to the given @location, then reads @len bytes into the @buf from @storage
 * @param:storage = file to read from
 * @param:buf = buffer to read values into
 * @param:location = offset in the file
 * @param:len = number of bytes to read starting from the offset @locarion in @storage
 * @returns = number of bytes read or -1 if error
 */
int get_bytes(STORAGE *storage, unsigned char *buf, int location, int len)
{
  int setStart = lseek(storage->fd, location, SEEK_SET); //sets the offset in the file

  //error setting offset
  if (setStart < 0)
  {
    fprintf(stderr, "error setting offset\n");
    return setStart;
  }

  int readBytes = read(storage->fd, buf, len); //reads from offset to len number of bytes

  //error reading bytes
  if (readBytes < 0) 
  {
    fprintf(stderr, "error reading file\n");
  }
  return readBytes;
}

/*
 *  sets the offset to the given @location, then writes @len bytes into the @storage from @buf
 * @param:storage = file to wrote to
 * @param:buf = buffer to read values out of
 * @param:location = offset in the file
 * @param:len = number of bytes to write starting from the offset @locarion in @storage
 * @returns = number of bytes written or -1 if error
 */
int put_bytes(STORAGE *storage, unsigned char *buf, int location, int len)
{
  int setStart = lseek(storage->fd, location, SEEK_SET); //sets the offset in the file

  //error setting offset
  if (setStart < 0)
  {
    fprintf(stderr, "error setting offset\n");
    return setStart;
  }

  int writeBytes = write(storage->fd, buf, len); //write len number of bytes into the file from the buffer

  //errors writing to file
  if (writeBytes < 0)
  {
    fprintf(stderr, "error writing to file\n");
  }
  return writeBytes;
}