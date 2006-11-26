#include <c++abi.h>
using namespace __cxxabiv1;

unsigned int iObject = 0;
finalizeObject object[32];
dso_handle __dso_handle = (void *)&__ctors_start__;

int __cxxabiv1::__cxa_atexit(	void (*f)(void *),
								void *p,
								dso_handle d)
{
	if (iObject >= 32)return -1;
	object[iObject].function = f;
	object[iObject].param = p;
	object[iObject].dso = d;
	++iObject;
	return 0;
}
void __cxxabiv1::__cxa_finalize(dso_handle d)
{
	unsigned int i = iObject;
	for (;i > 0;i--)
	{
		--iObject;
		object[iObject].function(object[iObject].param);
	}
}
