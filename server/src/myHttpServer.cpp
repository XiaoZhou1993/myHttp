#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>

typedef unsigned char   	WORD;
typedef unsigned short  	WORD16;
typedef unsigned int    	WORD32;
typedef long unsigned int	WORD64;

void errorInfo(const char* errorCauseString);
int initMyHttpSocket(WORD16* port);

int main()	
{
	using namespace std;
	cout << "Running my Http server!" << endl;

	WORD16 port = 80;
	int myHttpServerSocket = -1;
	int myHttpClientSocket = -1;
	struct sockaddr_in clientAddr;
	WORD32 clientAddrLength = sizeof(clientAddr);
	const int MAXBUFNUM = 4096;
	char buff[MAXBUFNUM];
	//pthread_t newthread;

	myHttpServerSocket = initMyHttpSocket(&port);
	cout << "http running on port :" << port << endl;

	while(1)
	{
		myHttpClientSocket = accept(myHttpServerSocket, (struct sockaddr *)&clientAddr, &clientAddrLength);

		if(-1 == myHttpClientSocket)
			errorInfo("accept socket error!");

		int byteNums = read(myHttpClientSocket, buff, MAXBUFNUM);
		if(byteNums != 0)
			std::cout<<"get information:"<<buff<<std::endl;

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

	std::cout << "Excute accept request!" << std::endl;

}

int initMyHttpSocket(WORD16* port)
{
	std::cout << "Start init my http socket server!" << std::endl;
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