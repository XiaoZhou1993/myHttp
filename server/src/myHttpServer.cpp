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
#include "server/src/myHttpServer.h"

void* acceptRequest(void* arg);
void myHttpCreatePipe(void);
bool judgeInputLegal(std::string tempChoice);
bool inputServerChoice(void);

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

	myHttpClientSocket = accept(myHttpServerSocket, (struct sockaddr *)&clientAddr, &clientAddrLength);
	if(-1 == myHttpClientSocket)
		errorInfo("accept socket error!");

	bool strRet = inputServerChoice();
	HTTP_LOG_INFO("strRet = %d", strRet);
	while(!strRet)
	{
		HTTP_LOG_INFO("Enter the infinite loop !");

		if(pthread_create(&newthread, NULL, acceptRequest, (void *)(&myHttpClientSocket)))
			errorInfo("create thread failed!");

		if(pthread_join(newthread, NULL))
			errorInfo("join thread failed!");

		strRet = inputServerChoice();
	}

	if(close(myHttpServerSocket))
		errorInfo("Close the socket failed !");

	HTTP_LOG_INFO("Shutdown the server successful !");
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
	HTTP_LOG_INFO("received bytes : %d", byteNums);
	if(byteNums != 0)
		HTTP_LOG_INFO("get information: %s", buff);

	memset(buff, 0, strlen(buff));

	return NULL;
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

	/*设置地址重复使用*/
	int addrReuseOn = 1; //on为1表示开启
	if(setsockopt(http , SOL_SOCKET, SO_REUSEADDR, &addrReuseOn, sizeof(addrReuseOn))<0)
		errorInfo("setsockopt error");

	if(bind(http, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		errorInfo("bind socket error!");

	if(listen(http, 5) < 0)
		errorInfo("listen socket error!");

	return http;
}

bool inputServerChoice(void)
{
	using namespace std;

	HTTP_LOG_INFO("Enter inputServerChoice !");
	HTTP_LOG_INFO("Run the server? please input yes/no !");

	WORD count = 10;

	string choiceInput;
	getline(cin, choiceInput);
	HTTP_LOG_INFO("The input is %s !", choiceInput.c_str());

	while(count)
	{
		if (judgeInputLegal(choiceInput))
		{
			break;
		}
		else
		{
			HTTP_LOG_INFO("Run the server? please input yes/no !");
			choiceInput.clear();
			getline(cin, choiceInput);
		}

		count--;
	}

	return strcmp(choiceInput.c_str(), "yes");
}

bool judgeInputLegal(std::string tempChoice)
{
	if(tempChoice == "yes" || tempChoice == "no")
		return true;

	return false;
}

void myHttpCreatePipe(void)
{
	int pipeFd[2];
	std::string sendBuff;
	std::string recvBuff;
	WORD32 bytesCount = 0;

	if(0 == pipe(pipeFd))
	{
		pid_t fPid = fork();

		if(fPid < 0)
		{
			errorInfo("Create chid process failed!");
		}
		else if(0 == fPid)
		{
			HTTP_LOG_INFO("This is Child Process! The PID is %d, PPID is %d", getpid(), getppid());
			close(pipeFd[1]);
			char tempRecvBuff[128] = {0};
			bytesCount = read(pipeFd[0], tempRecvBuff, 128);
			recvBuff = std::string(tempRecvBuff);
			HTTP_LOG_INFO("This is Child Process! Received %d bytes, the string is %s", bytesCount, recvBuff.c_str());
			sleep(10);
			HTTP_LOG_INFO("Exit Child Process!");
		}
		else
		{
			HTTP_LOG_INFO("This is Parent Process! The PID is %d, CPID is %d", getpid(), fPid);
			close(pipeFd[0]);
			getline(std::cin, sendBuff);
			bytesCount = write(pipeFd[1], sendBuff.c_str(), sendBuff.length());
			HTTP_LOG_INFO("This is Parent Process! Sent %d bytes, the string is %s", bytesCount, sendBuff.c_str());
			sleep(15);
			HTTP_LOG_INFO("Exit Parent Process!");
		}
	}
}

void errorInfo(const char* errorCauseString)
{
	perror(errorCauseString);
	exit(1);	
}