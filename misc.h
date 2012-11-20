#ifndef MISC_H_INCLUDED
#define MISC_H_INCLUDED

#include <sstream>
#include <ctime>

template<typename T>
string ToString(const T& a)
{
	stringstream ss;
	ss<<a;
	return ss.str();
}

template<typename T>
const char* ToChar(const T& a)
{
	stringstream ss;
	ss<<a;
	return ss.str().c_str();
}


template<typename T>
T CharTo(const char* a)
{
	stringstream ss(a);
	T ret;
	ss>>ret;
	return ret;
}


bool operator>=(const struct timeval& t1, const struct timeval& t2)
{
	if(t1.tv_sec>t2.tv_sec)
		return true;
	else if(t1.tv_sec<t2.tv_sec)
		return false;
	else
	{
		if(t1.tv_usec>=t2.tv_usec)
			return true;
		else
			return false;
	}
}
//===================
struct timeval operator+(const struct timeval& t1, const struct timeval& t2)
{
	struct timeval ret;
	int nMod = 0;

	ret.tv_usec = t1.tv_usec + t2.tv_usec;
	if(ret.tv_usec>=1000000)
	{
		nMod = 1;
		ret.tv_usec-=1000000;
	}
	ret.tv_sec = t1.tv_sec + t2.tv_sec + nMod;

	return ret;
}
//===================
struct timeval operator-(const struct timeval& t1, const struct timeval& t2)
{
	struct timeval ret;
	int nMod = 0;

	ret.tv_usec = t1.tv_usec - t2.tv_usec;
	if(ret.tv_usec<0)
	{
		nMod = 1;
		ret.tv_usec=1000000-ret.tv_usec;
	}
	ret.tv_sec = t1.tv_sec - t2.tv_sec - nMod;

	return ret;
}


#endif // MISC_H_INCLUDED
