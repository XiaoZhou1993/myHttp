#ifndef _MY_HTTP_MSG_H
#define _MY_HTTP_MSG_H

#include <string.h>

class myHttpMessage
{
	private:
		std::string m_method;
		std::string m_url;
		std::string m_path;

	public:
		myHttpMessage();
		myHttpMessage(char* httpMsg);

		~myHttpMessage();

		std::string getHttpMethod();
		bool judgeHttpReqType(const std::string &aimType);
};

#endif