#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include "server/include/myHttpLog.h"
#include "server/include/myHttpAssert.h"
#include "server/src/myHttpServer.h"

void* acceptRequest(void* arg);

int main()	
{
	using namespace std;
	HTTP_LOG_INFO("Running my Http server!");

	WORD16 port = 80;
	int myHttpServerSocket = -1;
	int myHttpClientSocket = -1;
	struct sockaddr_in clientAddr;
	WORD32 clientAddrLength = sizeof(clientAddr);

	pthread_t newthread;

	myHttpServerSocket = initMyHttpSocket(&port);
	HTTP_LOG_INFO("http running on port : %d", port);

	while(1)
	{
		myHttpClientSocket = accept(myHttpServerSocket, (struct sockaddr *)&clientAddr, &clientAddrLength);

		if(-1 == myHttpClientSocket)
			errorInfo("accept socket error!");

		if(pthread_create(&newthread, NULL, acceptRequest, (void *)(&myHttpClientSocket))!= 0)
			errorInfo("create pthread failed!");

		pthread_join(newthread, NULL);
	}

	close(myHttpServerSocket);

	return 0;
}

void* acceptRequest(void* arg)
{
	HTTP_LOG_INFO("Excute accept request!");

	ASSERT_VAILD_POINTER_NONE_RET(arg);

	int *client = (int *)arg;
	const int maxBufNum = 4096;
	char buff[maxBufNum];

	int byteNums = read(*client, buff, maxBufNum);
	if(byteNums != 0)
		HTTP_LOG_INFO("get information: %s", buff);

	close(*client);
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