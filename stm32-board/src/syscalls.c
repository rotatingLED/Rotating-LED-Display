/*
 * syscalls.c
 *
 *  Created on: 03.12.2009
 *      Author: Martin Thomas, 3BSD license
 */

#define SBRK_VERBOSE 1

#include <reent.h>
#include <errno.h>
#include <stdlib.h> /* abort */
#include <sys/types.h>
#include <sys/stat.h>
#include <errdisplay.h>

#include "stm32f10x.h" /* for _get_PSP() from core_cm3.h*/

#undef errno
extern int errno;

int _kill(int pid, int sig) {
	pid = pid;// avoid warnings
	sig = sig; // avoid warnings
	errno = EINVAL;
	return -1;
}

void _exit(int status) {
	status = status;// avoid warnings
	errdisplay_displayErr(ERROR_EXIT);
}

int _getpid(void) {
	return 1;
}

extern char _end; /* Defined by the linker */
static char *heap_end;

char* get_heap_end(void) {
	return (char*) heap_end;
}

char* get_stack_top(void) {
	return (char*) __get_MSP();
}

caddr_t _sbrk(int incr) {
	char *prev_heap_end;
	if (heap_end == 0) {
		heap_end = &_end;
	}
	prev_heap_end = heap_end;
	if (heap_end + incr > get_stack_top()) {
		errdisplay_displayErr(ERROR_HEAP_STACK_COLLISION);
	}
	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

int _close(int file) {
	file = file; // avoid warnings
	return -1;
}

int _fstat(int file, struct stat *st) {
	file = file; // avoid warnings
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file) {
	file = file; // avoid warnings
	return 1;
}

int _lseek(int file, int ptr, int dir) {
	file = file; // avoid warnings
	ptr = ptr; // avoid warnings
	dir = dir; // avoid warnings
	return 0;
}

int _read(int file, char *ptr, int len) {
	file = file; // avoid warnings
	ptr = ptr; // avoid warnings
	len = len; // avoid warnings
	return 0;
}

int _write(int file, char *ptr, int len) {
	file = file; // avoid warnings
	ptr = ptr; // avoid warnings
	return len;
}
