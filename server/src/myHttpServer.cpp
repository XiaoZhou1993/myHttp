#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include "../include/myHttpLog.h"

typedef unsigned char   	WORD;
typedef unsigned short  	WORD16;
typedef unsigned int    	WORD32;
typedef unsigned long long	WORD64;

int initMyHttpSocket(WORD16* port);
void errorInfo(const char* errorCauseString);

int main()	
{
	using namespace std;
	HTTP_LOG_INFO("Running my Http server!");

	WORD16 port = 80;
	int myHttpServerSocket = -1;
	int myHttpClientSocket = -1;
	struct sockaddr_in clientAddr;
	WORD32 clientAddrLength = sizeof(clientAddr);

	const int maxBufNum = 4096;
	char buff[maxBufNum];
	//pthread_t newthread;

	myHttpServerSocket = initMyHttpSocket(&port);
	HTTP_LOG_INFO("http running on port : %d", port);

	while(1)
	{
		myHttpClientSocket = accept(myHttpServerSocket, (struct sockaddr *)&clientAddr, &clientAddrLength);

		if(-1 == myHttpClientSocket)
			errorInfo("accept socket error!");

		int byteNums = read(myHttpClientSocket, buff, maxBufNum);
		if(byteNums != 0)
			HTTP_LOG_INFO("get information: %s", buff);

		close(myHttpClientSocket);
		//if(pthread_create(&newthread, NULL, acceptRequest,myHttpClientSocket) != 0)
			//errorInfo("create pthread failed!");
	}

	close(myHttpServerSocket);

	return 0;
}

void acceptRequest(int client)
{
	char buf[1024];
	int numChars;
	char method[255];
	char url[255];
	char path[512];
	WORD32 i,j;

	//struct stat st;

	HTTP_LOG_INFO("Excute accept request!");

}

int initMyHttpSocket(WORD16* port)
{
	HTTP_LOG_INFO("Start init my http socket server!");

	struct sockaddr_in serverAddr;
	int http = socket(AF_INET, SOCK_STREAM, 0);	//AF_INET:网络连接，ipv4    //SOCK_STREAM:TCP连接

	if(-1 == http)
		errorInfo("create socket error!");

	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(*port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(http, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		errorInfo("bind socket error!");

	if(listen(http, 5) < 0)
		errorInfo("listen socket error!");

	return http;
}

void errorInfo(const char* errorCauseString)
{
	perror(errorCauseString);
	exit(1);	
}