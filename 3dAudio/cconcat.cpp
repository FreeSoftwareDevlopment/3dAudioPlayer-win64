#include <memory>
#include <cstring>
#include "cconcat.h"
#pragma warning(disable : 4996)

char* concat(const char* c1, const char* c2, const char* c3)
{
#define t(c) c == nullptr ? 0 : strlen(c)
	size_t s1l = strlen(c1),
		s2l = t(c2),
		s3l = t(c3),
		ssize = s2l + s1l + s3l + 1,
		me = sizeof(char) * ssize;
#undef t
	char* cw = (char*)malloc(me);
	if (cw == nullptr)
		throw std::exception("malloc returned nullptr");
	strncpy(cw, c1, s1l);
#define elem(c, x) if(c != nullptr) strncat(cw, c, x)
	elem(c2, s2l);
	elem(c3, s3l);
#undef elem
	return cw;
}
