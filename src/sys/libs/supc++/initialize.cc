#include <c++abi.h>
#include <cstddef>
using namespace std;
using namespace __cxxabiv1;

extern "C" void halt();

void __cxxabiv1::__cxa_initialize()
{
	void (**Constructor)() = &__ctors_start__;

	size_t i = (reinterpret_cast<size_t>(&__ctors_end__) -
				reinterpret_cast<size_t>(&__ctors_start__)) / sizeof(void *);
	for (;i > 0;i--)
	{
		(*Constructor)();
		++Constructor;
	}
}
