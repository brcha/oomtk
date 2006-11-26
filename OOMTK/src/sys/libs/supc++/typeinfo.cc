#include <c++abi.h>
using namespace std;
using namespace __cxxabiv1;

type_info::type_info(const type_info &rhs)
{
	_name = rhs._name;
}

type_info::~type_info(){}

__fundamental_type_info::__fundamental_type_info(const __fundamental_type_info &rhs)
	: type_info(rhs){}

__array_type_info::__array_type_info(const __array_type_info &rhs)
	: type_info(rhs){}

__function_type_info::__function_type_info(const __function_type_info &rhs)
	: type_info(rhs){}

__enum_type_info::__enum_type_info(const __enum_type_info &rhs)
	: type_info(rhs){}

__class_type_info::__class_type_info(const __class_type_info &rhs)
	: type_info(rhs){}

__si_class_type_info::__si_class_type_info(const __si_class_type_info &rhs)
	: __class_type_info(rhs){}

__vmi_class_type_info::__vmi_class_type_info(const __vmi_class_type_info &rhs)
	: __class_type_info(rhs){}

__pointer_type_info::__pointer_type_info(const __pointer_type_info &rhs)
	: __pbase_type_info(rhs){}

void *__dynamic_cast (	const void *sub,
						const __class_type_info *src,
						const __class_type_info *dst,
						std::ptrdiff_t src2dst_offset)
{
	return 0;
}
