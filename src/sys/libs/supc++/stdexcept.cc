#include <stdexcept>
using namespace std;

const char *exception::what() const throw()
{
	return "Unknown exception";
}
const char *logic_error::what() const throw()
{
	//return error.c_str();
	return s;
}
