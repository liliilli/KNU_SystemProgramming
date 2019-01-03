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
 * @brief Open utmp with path to read utmp file.
 * @return File descriptor of utmp
 */
t_fd_index		utmp_open(const char* iFileName);

/*
 * @brief Get next utmp information. If failed, return null.
 */
struct utmp*	utmp_next();

/*
 * @brief Release utmp buffer and read utmp file again.
 * @reutrn utmp information count.
 */
int						utmp_reload();

/*
 * @brief Close and release utmp buffer.
 */
void					utmp_close();
