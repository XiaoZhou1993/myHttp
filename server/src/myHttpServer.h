#ifndef _MY_HTTP_SERVER_H
#define _MY_HTTP_SERVER_H

typedef unsigned char   	WORD;
typedef unsigned short  	WORD16;
typedef unsigned int    	WORD32;
typedef unsigned long long	WORD64;

int initMyHttpSocket(WORD16* port);
void errorInfo(const char* errorCauseString);

#endif