#include <sys/socket.h>
#include <iostream>
#include <cstring>
//#include <cygwin/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../include/myHttpLog.h"

void errorInfo(const char* errorCauseString);

int main() 
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    	errorInfo("socket error!");

    const char* serverAddr = "127.0.0.1";

    struct sockaddr_in targetServerAddr;
    memset((char*)&targetServerAddr, 0, sizeof(targetServerAddr));

    targetServerAddr.sin_family = AF_INET;
    targetServerAddr.sin_port = htons(80);
    inet_pton(AF_INET, serverAddr, &targetServerAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&targetServerAddr, sizeof(targetServerAddr)) < 0) 
    {
        errorInfo("connect failed");
        return 0;
    }

    const int MAXBUF = 4096;
    char buffer[MAXBUF] = "hello TCP";
    int nbytes = write(clientSocket, buffer, 10);

    close(clientSocket);
}

void errorInfo(const char* errorCauseString)
{
	perror(errorCauseString);
	exit(1);	
}