/*
 * @file : cp1.c
 * @author : Jongmin Yun
 * @datetime : 2018-12-27 23:16 Updated.
 * @description : program for copy source file as destination file.
 * This program is imitiation of -nix command prgoram, 'cp'.
 * 
 * To use this program, first argument should be source file,
 * and second argument is destination file path to copy source file.
 * 
 * './cp1 source_file_path destination_file_path'
 */

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096
/* copied file access mode is 644 (User rw_, Group r__, Other r__) */
#define COPYMODE    0644 

enum EErrorCode
{
  /* Use this at everywhere which general error code is -1. */
  E_Failed = -1
  /* Use this code when use 'read' function in fcntl. */
  E_EOF    = 0
};

/*
 * @brief Check the number of argument on execution is same to
 * iExpected. If not, print error message and abort. 
 */ 
void CheckEnvArgCount(
    const int iArgc, 
    const unsigned iExpected,
    const char* iErrorMsg);

/*
 * @brief Print or process error message, `iMsg` and `iFilePath`, 
 * and abort program. 
 */
void ProcessError(char* iMsg, const char* iFilePath);

int main(int argc, char* argv[])
{
  int fdIn, fdOut, numChars;
  char buffer[BUFFER_SIZE];

  /* Check argc is 3 (program source_file destination_file) */
  CheckEnvArgCount(argc, 3, "usage : ./cp1 source destination\n");

  /* Read `source file` as read only. 
   * fdIn will have file descriptor index of argv[1] if successful,
   * otherwise abort. */
  if ((fdIn = open(argv[1], O_RDONLY)) == E_Failed)
  {
    ProcessError("Failed to open.\n", argv[1]);
  }

  /*
   * Create `destionation file` file as chmod 644 (rw_r__r__).
   * fdOut will have file descriptor index of argv[2] if successful,
   * otherwise abort program for example not being able to create file.
   */ 
  if ((fdOut = creat(argv[2], COPYMODE)) == E_Failed)
  {
    ProcessError("Failed to open.\n", argv[2]);
  }

  /*
   * Read file buffer with BUFFER_SIZE (4096) until file stream cursor
   * is end to file end. copied buffer will be copied to buffer.
   * If function call is successful, numChar will have read byte size.
   * or, 0(E_EOF) when it already reach eof.
   */
  while ((numChars = read(fdIn, buffer, BUFFER_SIZE)) > E_EOF)
  { /*
     * write to file descriptor of output (destination) buffer.
     * If must be successful but might return -1(E_Failed) if failed.
     */
    if (write(fdOut, buffer, numChars) != numChars)
    {
      ProcessError("Write error to ", argv[2]);
    }

    /* Error checking */
    if (numChars == E_Failed) { ProcessError("Read error from ", argv[1]); }
  }

  /*
   * close `fdIn` and `fdOut` file descriptors manually, and must be succeeded.
   */
  if (close(fdIn) == E_Failed || close(fdOut) == E_Failed)
  {
    ProcessError("Error closing files", "");
  }

  return 0;
}

void CheckEnvArgCount(
    const int iArgc, 
    const unsigned iExpected,
    const char* iErrorMsg)
{
  if (iArgc != iExpected)
  {
    fprintf(stderr, iErrorMsg);
    exit(1);
  }
}

void ProcessError(char* iMsg, const char* iFilePath)
{
  fprintf(stderr, "[Error] : %s", iMsg);
  perror(iFilePath);
  exit(1);
}

