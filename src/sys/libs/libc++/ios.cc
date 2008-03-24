#include <ios>
using namespace std;

ios_base::fmtflags ios_base::flags(fmtflags fmtfl)
{
	ios_base::fmtflags tmp = _fmtflags;
	_fmtflags = fmtfl;
	return tmp;
}
ios_base::fmtflags ios_base::setf(ios_base::fmtflags fmtfl)
{
	ios_base::fmtflags tmp = _fmtflags;
	_fmtflags = fmtfl | _fmtflags;
	return tmp;
}
ios_base::fmtflags ios_base::setf(ios_base::fmtflags fmtfl, ios_base::fmtflags mask)
{
	ios_base::fmtflags tmp = _fmtflags;
	_fmtflags = fmtfl | (_fmtflags & (~mask));
	return tmp;
}
