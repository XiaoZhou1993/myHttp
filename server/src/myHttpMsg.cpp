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
	m_method = tempString;
	if(true == m_method.empty())
		exit(0);
	HTTP_LOG_INFO("m_method = %s", m_method.c_str());

	tempString = strtok(NULL, delimPara2);
	m_url = tempString;
 	if(true == m_url.empty())
		exit(0);
	HTTP_LOG_INFO("m_url = %s", m_url.c_str());

	tempString = strtok(NULL, delimPara1);
	m_path = tempString;
	if(true == m_path.empty())
		exit(0);
	HTTP_LOG_INFO("m_path = %s", m_path.c_str());
}

myHttpMessage::~myHttpMessage()
{
	HTTP_LOG_INFO("Enter the ~myHttpMessage!");

	m_method.clear();
	m_url.clear();
	m_path.clear();
}

std::string myHttpMessage::getHttpMethod()
{
	return m_method;
}

bool myHttpMessage::judgeHttpReqType(const std::string &aimType)
{
	HTTP_LOG_INFO("Enter the judgeHttpReqType !");
	return m_method == aimType;
}