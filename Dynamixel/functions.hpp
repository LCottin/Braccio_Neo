#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#if defined(__linux__) || defined(__APPLE__)
	#include <fcntl.h>
	#include <termios.h>
	#define STDIN_FILENO 0
#elif defined(_WIN32) || defined(_WIN64)
	#include <conio.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#define ESC_ASCII_VALUE 0x1b

int getch();
int kbhit(void);

#endif