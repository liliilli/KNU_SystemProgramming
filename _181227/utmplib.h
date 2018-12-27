/*
 * @file : utmplb.h
 * @author : 2012104208 Jongmin Yun
 * @datetime : 2018-12-27 12:10
 * @description : Header file for include utmplib.c as defining 
 * function symbol table address pointer avoiding segment fault.
 */

#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <utmp.h>

typedef int t_fd_index;

/*
 * @brief Open 
 */
t_fd_index		utmp_open(const char* iFileName);
struct utmp*	utmp_next();
int						utmp_reload();
void					utmp_close();
