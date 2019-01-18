#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include "client/include/myHttpLog.h"

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

    std::string sendBuff;
    HTTP_LOG_INFO("Please input string : ");
    while(getline(std::cin, sendBuff))
    {
        HTTP_LOG_INFO("input string : %s", sendBuff.c_str());
        int nbytes = write(clientSocket, sendBuff.c_str(), sendBuff.length());
        HTTP_LOG_INFO("sent bytes : %d", nbytes);
        sendBuff.clear();
        HTTP_LOG_INFO("Please input string : ");
    }

    close(clientSocket);
}

void errorInfo(const char* errorCauseString)
{
	perror(errorCauseString);
	exit(1);	
}