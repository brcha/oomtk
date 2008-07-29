#include <c++abi.h>

#include <panic.h>

using namespace __cxxabiv1;

void __cxxabiv1::__cxa_pure_virtual()
{
  panic("Pure virtual called!\n");
}
