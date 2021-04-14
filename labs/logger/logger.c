#include <stdio.h>
#include <stdarg.h>

#define RESET 0
#define BRIGHT 1
#define DIM 2
#define UNDERLINE 3
#define BLINK 4
#define REVERSE 7
#define HIDDEN 8
#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define	WHITE 7

void set_color(int attr, int fog, int bag){
    	char command[13];
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fog + 30, bag + 40);
	printf("%s", command);
}

void infof(const char *format, ...){
    va_list args;
    va_start(args,format);
    set_color(RESET,BLUE,BLACK);
    vprintf(format,args);
    va_end(args);
    printf("%c[0m",0x1B); 
}

void warnf(const char *format, ...){
    va_list args;
    va_start(args,format);
    set_color(RESET,YELLOW,BLACK);
    vprintf(format,args);
    va_end(args);
    printf("%c[0m",0x1B);
}

void errorf(const char *format, ...){
    va_list args;
    va_start(args,format);
    set_color(RESET,RED,BLACK);
    vprintf(format,args);
    va_end(args);
    printf("%c[0m",0x1B);
}

void panicf(const char *format, ...){
    va_list args;
    va_start(args,format);
    set_color(RESET,MAGENTA,BLACK);
    vprintf(format,args);
    va_end(args);
    printf("%c[0m",0x1B);
}

