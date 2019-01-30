#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include "myHttpMsg.h"
#include "server/include/myHttpLog.h"

myHttpMessage::myHttpMessage()
{
	m_method = "0";

	m_url = "0";

	m_path = "0";
}

myHttpMessage::myHttpMessage(char* httpMsg)
{
	HTTP_LOG_INFO("Enter myHttpMessage constructor!");

	char *tempString;

	const char* delimPara1 = " ";
	const char* delimPara2 = "?";

	tempString = strtok(httpMsg, delimPara1);
	if(!tempString)
		exit(0);
	m_method = tempString;
	HTTP_LOG_INFO("m_method = %s", m_method.c_str());

	tempString = strtok(NULL, delimPara2);
	if(!tempString)
		exit(0);
	m_url = tempString;
	HTTP_LOG_INFO("m_url = %s", m_url.c_str());

	tempString = strtok(NULL, delimPara1);
	if(!tempString)
		exit(0);
	m_path = tempString;
	HTTP_LOG_INFO("m_path = %s", m_path.c_str());
}

myHttpMessage::~myHttpMessage()
{
	HTTP_LOG_INFO("Enter the ~myHttpMessage!");
}

std::string myHttpMessage::getHttpMethod()
{
	return m_method;
}

bool myHttpMessage::judgeHttpReqType(const std::string &aimType)
{
	return m_method == aimType;
}