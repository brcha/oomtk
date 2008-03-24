#include <c++abi.h>
#include <cstdlib>
#include <malloc.h>
#include <free.h>
using namespace std;

void *operator new (size_t size) throw()
{
	return malloc(size);
}
void *operator new[] (size_t size) throw()
{
	return malloc(size);
}
void *operator new (size_t size, void* memory) throw() {return memory;}
void *operator new[] (size_t size, void* memory) throw() {return memory;}
void operator delete (void * p)
{
	free(p);
}
void operator delete[] (void * p)
{
	free(p);
}
void operator delete (void *p, void *q)
{
	free(p);
}
void operator delete[] (void *p, void *q)
{
	free(p);
}
