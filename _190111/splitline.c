//
// @file : splitline.c
// @author : 2012104208 Jongmin Yun
// @datetime : 2019-01-10 23:47
// @description : Function definition file of Split args from string chunk. 
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smsh.h"

// @brief Construct for strings.
// @return String, this is never NULL.
char* NewStr(char* iStart, int iLen);

// @brief Check input `x` value is ` ` space or `\t` tab.
// @return If checked, return 1 or 0.
int IsDelim(const char x);

char* NextCmd(char* iPrompt, FILE* iFp)
{
  char* buf;        // The buffer 
  int bufSpace = 0; // Total size
  int pos = 0;      // Current Position
  int c;            // Input char

  printf("%s", iPrompt);
  while((c = getc(iFp)) != EOF)
  {
    // If we need more space, let it allocate more heap space.
    if (pos + 1 >= bufSpace)
    {
      if (bufSpace == 0) 
      { buf = Emalloc(BUFSIZ); }
      else 
      { buf = Erealloc(buf, bufSpace + BUFSIZ); }

      // And.. move pointer to end of updated given buffer.
      bufSpace += BUFSIZ;
    }

    // End of command.
    if (c == '\n') { break; }
    else
    {
      buf[pos++] = c;
    }
  }

  if (c == EOF && pos == 0) { return NULL; }

  // Replace last character to null.
  buf[pos] = '\0';
  return buf;
}

int IsDelim(const char x)
{
  return ((x) == ' ' || (x) == '\t') ? 1 : 0;
}

char** SplitLine(char* iLine)
{
  char** args;
  int spots = 0;    // Spots in table.
  int bufspace = 0; // Bytes in table.
  int argnum = 0;   // Used slots
  char* cp = iLine; // Position in string
  char* start;
  int len;

  // If command line is empty, just return nothing but null.
  if (iLine == NULL) { return NULL; }

  args = Emalloc(BUFSIZ);
  bufspace = BUFSIZ;
  spots = BUFSIZ / sizeof(char*);

  while (*cp != '\0')
  {
    // If *cp is space or tab, just move pointer to next.
    while (IsDelim(*cp) == 1) { ++cp; }
    // If *cp is '\0' so end of string, break loop.
    if (*cp == '\0') { break; }

    if (argnum + 1 >= spots)
    {
      args = Erealloc(args, bufspace + BUFSIZ);
      bufspace += BUFSIZ;
      spots += (BUFSIZ / sizeof(char*));
    }
    
    // Make start, then find end of word.
    start = cp;
    len   = 1;
    while (*++cp != '\0' && IsDelim(*cp) == 0)
    {
      len += 1;
    }
    args[argnum] = NewStr(start, len);
    argnum += 1;
  }
  
  // Fill last args as NULL (specification)
  args[argnum] = NULL;
  return args;
}

char* NewStr(char* iStart, int iLen)
{
  char* rv = Emalloc(iLen + 1);
  rv[1] = '\0';

  strncpy(rv, iStart, iLen);
  return rv;
}

void FreeList(char** iList)
{
  char** cp = iList;
  while (*cp != NULL) 
  { 
    free(*cp); 
    cp += 1;
  }
  free(iList);
}

void* Emalloc(size_t iByteSize)
{
  void* rv = NULL;
  if ((rv = malloc(iByteSize)) == NULL)
  {
    Fatal("Out of memory!", "", 1);
  }
  return rv;
}

void* Erealloc(void* iPtrType, size_t iByteSize)
{
  void* rv = NULL;
  if ((rv = realloc(iPtrType, iByteSize)) == NULL)
  {
    Fatal("realloc() failed.", "", 1);
  }
  return rv;
}

void Fatal(char* iS1, char* iS2, int n)
{
  fprintf(stderr, "Error : %s, %s\n", iS1, iS2);
  exit(n);
}
