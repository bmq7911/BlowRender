#pragma once
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>

static inline void _Log( uint32_t line, const char * func,const char * type, const char* title , const char * fmt , ... ){
#define FMT_STR "%s: [line]:%05d [func]:%s [%s] "
    printf(FMT_STR, type, line, func, title);
	va_list pArgs;
	va_start(pArgs, fmt);
    // double * p = (double*)(pArgs);
    char buff[2048] = { 0 };
    vsnprintf(buff, 2048, fmt, pArgs);
	printf( buff );
    printf("\r\n");
	va_end(pArgs);
}

#ifdef DEBUG_LOG
#define LOG( type, title, fmt, ... )  _Log( __LINE__, __FUNCTION__, type, title, fmt, ##__VA_ARGS__ );
#define LOG_TRACE( title, fmt, ...)   LOG( "Trace",  title, fmt, ##__VA_ARGS__)
#define LOG_ERROR( title, fmt, ...)   LOG( "Error",  title, fmt, ##__VA_ARGS__)
#define LOG_WARNING( title, fmt, ... ) LOG( "Warning",  title, fmt, ##_VR_ARGS__)
#else
#define LOG( type, title, fmt, ... ) 
#define LOG_TRACE( title, fmt, ...) 
#define LOG_ERROR( title, fmt, ...) 
#define LOG_WARING( title, fmt, ... )
#endif 

