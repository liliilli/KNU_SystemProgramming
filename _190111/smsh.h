#ifndef GAURD_SMSH_H
#define GAURD_SMSH_H
//
// @file : smsh.h
// @author : 2012104208 Jongmin Yun
// @datetime : 2019-01-10 22:33
// @description : Function declaration file of smsh1.c 
//

#include <stdio.h>

#define YES 1
#define NO 0

// @brief Read next command line from fp.
// @return dynamically allocated string holding command line.
// This string must be released outside world.
// If EOF or called error by malloc() in function body, just return NULL.
char*  NextCmd();

// @brief Split a line int oarray of white-space separated tokens.
// @return a NULL-terminated array of pointers to copies of the tokens.
// or NULL if line if no tokesn on the line.
char** SplitLine(char*);

// @brief Free the list returend by SplitLine() function call.
void   FreeList(char**);

void*  Emalloc(size_t);

void*  Erealloc(void*, size_t);

// @brief Get a error message and print message. exit with int value.
void   Fatal(char*, char*, int);

// @brief Run a program passing it arguments.
// @return Status returned via wait, or -1 on error on fork() or wait() error.
int    Execute(char** argv);

int IsControlCommand(char*);
int DoControlCommand(char**);
int OkToExecute();

int Process(char** iArgv);

#endif // GAURD_SMSH_H
