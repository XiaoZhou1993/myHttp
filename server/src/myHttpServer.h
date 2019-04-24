#ifndef _MY_HTTP_SERVER_H
#define _MY_HTTP_SERVER_H

#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>

#include "server/include/myHttpLog.h"
#include "server/include/myHttpAssert.h"
#include "server/src/myHttpMsg.h"

typedef unsigned char   	WORD8;
typedef unsigned short  	WORD16;
typedef unsigned int    	WORD32;
typedef unsigned long long	WORD64;

int initMyHttpSocket(WORD16* port);
void errorInfo(const char* errorCauseString);

void* acceptRequest(void* arg);
void myHttpCreatePipe(void);
bool judgeInputLegal(std::string tempChoice);
bool inputServerChoice(void);

#endif