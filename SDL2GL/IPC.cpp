#include "IPC.h"

IPC::IPC()
{
}

void IPC::create(std::string &_filename)
{
	
}

void IPC::open(std::string &_filename)
{

}

void IPC::close()
{

}

void IPC::read()
{

}

void IPC::write()
{

}

std::ostream & operator<<(std::ostream &os, std::string & _data)
{
	os << _data;
	return os;
}


IPC::~IPC()
{
}
