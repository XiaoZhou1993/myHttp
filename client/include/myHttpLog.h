#ifndef _HTTP_LOG_H
#define _HTTP_LOG_H

#include <cstdio>

#define HTTP_LOG_INFO(format, ...)  fprintf(stderr, "[%s @ line %d]" format "\n", __func__, __LINE__, ##__VA_ARGS__)

#endif