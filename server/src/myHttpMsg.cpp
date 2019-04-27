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

	const char delimPara1 = ' ';
	const char delimPara2 = '?';

	std::string tempSourceString(httpMsg);
	std::string tempString;
	std::string::size_type startPos,endPos;

	startPos = 0;
	endPos = tempSourceString.find(delimPara1);
	if(std::string::npos == endPos)
		exit(0);

	tempString = tempSourceString.substr(startPos, (endPos-startPos));
	m_method = tempString;
	HTTP_LOG_INFO("m_method = %s", m_method.c_str());
	tempString.clear();

	startPos = endPos + 1;
	endPos = tempSourceString.find(delimPara2);
	if(std::string::npos == endPos)
		exit(0);

	tempString = tempSourceString.substr(startPos, (endPos-startPos));
	m_url = tempString;
	HTTP_LOG_INFO("m_url = %s", m_url.c_str());
	tempString.clear();

	startPos = endPos + 1;
	endPos = tempSourceString.find(delimPara1);
	if(std::string::npos == endPos)
		exit(0);

	tempString = tempSourceString.substr(startPos, (endPos-startPos));
	m_path = tempString;
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
	HTTP_LOG_INFO("Enter the judgeHttpReqType!");
	return m_method == aimType;
}