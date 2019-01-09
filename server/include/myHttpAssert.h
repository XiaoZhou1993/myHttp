#ifndef _HTTP_ASSERT_H
#define _HTTP_ASSERT_H

#include <cstdio>

#define RET_NULL_POINTER -1

#define ASSERT_VAILD_POINTER(x)   \
do \
{ \
	if(!x) \
	{ \
		fprintf(stderr, "[%s @ line %d]" "The pointer is Null!" "\n", __func__, __LINE__); \
		return RET_NULL_POINTER; \
	} \
} while (0)

#define ASSERT_VAILD_POINTER_NONE_RET(x)   \
do \
{ \
	if(!x) \
	{ \
		fprintf(stderr, "[%s @ line %d]" "The pointer is Null!" "\n", __func__, __LINE__); \
		return NULL; \
	} \
} while (0)

#endif