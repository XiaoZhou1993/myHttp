#include "server/src/myHttpServer.h"

void* acceptRequest(void* arg);
void myHttpCreatePipe(void);
bool judgeInputLegal(std::string tempChoice);
bool inputServerChoice(void);

int main()	
{
	HTTP_LOG_INFO("Running my Http server!");

	WORD16 port = 80;
	int myHttpServerSocket = -1;
	int myHttpClientSocket = -1;
	struct sockaddr_in clientAddr;
	WORD32 clientAddrLength = sizeof(clientAddr);

	pthread_t newthread;

	myHttpServerSocket = initMyHttpSocket(&port);
	HTTP_LOG_INFO("Http running on port : %d", port);

	myHttpClientSocket = accept(myHttpServerSocket, (struct sockaddr *)&clientAddr, &clientAddrLength);
	if(-1 == myHttpClientSocket)
		errorInfo("Accept socket error!");

	bool strRet = inputServerChoice();
	HTTP_LOG_INFO("strRet = %d", strRet);
	while(!strRet)
	{
		HTTP_LOG_INFO("Enter the infinite loop!");

		if(pthread_create(&newthread, NULL, acceptRequest, (void *)(&myHttpClientSocket)))
			errorInfo("Create thread failed!");

		if(pthread_join(newthread, NULL))
			errorInfo("Join thread failed!");

		strRet = inputServerChoice();
	}

	if(close(myHttpServerSocket))
		errorInfo("Close the socket failed!");

	HTTP_LOG_INFO("Shutdown the server successful!");
	return 0;
}

void* acceptRequest(void* arg)
{
	HTTP_LOG_INFO("Execute accept request!");

	ASSERT_VAILD_POINTER_NONE_RET(arg);

	int *client = (int *)arg;
	const int maxBufNum = 4096;
	char buff[maxBufNum];

	int byteNums = read(*client, buff, maxBufNum);
	HTTP_LOG_INFO("Received bytes : %d", byteNums);
	if(byteNums != 0)
		HTTP_LOG_INFO("Get information: %s", buff);

	myHttpMessage recvHttpMsg(buff);

	if(recvHttpMsg.judgeHttpReqType("GET"))
	{
		std::string tempString = recvHttpMsg.getHttpMethod();
		HTTP_LOG_INFO("The message type is %s!", tempString.c_str());
	}
	else
	{
		HTTP_LOG_INFO("The message type is not get!");
	}

	memset(buff, 0, strlen(buff));

	return NULL;
}

int initMyHttpSocket(WORD16* port)
{
	HTTP_LOG_INFO("Start initial my http socket server!");

	struct sockaddr_in serverAddr;

	int http = socket(AF_INET, SOCK_STREAM, 0);	//AF_INET:ipV4  //SOCK_STREAM:TCP连接

	if(-1 == http)
		errorInfo("Create socket error!");

	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(*port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	/*set the address reuse*/
	int addrReuseOn = 1;
	if(setsockopt(http , SOL_SOCKET, SO_REUSEADDR, &addrReuseOn, sizeof(addrReuseOn)) < 0)
		errorInfo("Set socket error!");

	if(bind(http, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		errorInfo("Bind socket error!");

	if(listen(http, 5) < 0)
		errorInfo("Listen socket error!");

	return http;
}

bool inputServerChoice(void)
{
	HTTP_LOG_INFO("Enter inputServerChoice!");
	HTTP_LOG_INFO("Run the server? please input yes/no!");

	WORD8 count = 10;

	std::string choiceInput;
	getline(std::cin, choiceInput);
	HTTP_LOG_INFO("The input is %s!", choiceInput.c_str());

	while(count)
	{
		if (judgeInputLegal(choiceInput))
		{
			break;
		}
		else
		{
			HTTP_LOG_INFO("Run the server? please input yes/no!");
			choiceInput.clear();
			getline(std::cin, choiceInput);
		}

		count--;
	}

	return choiceInput == "yes";
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
			errorInfo("Create child process failed!");
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