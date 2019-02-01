#ifndef _IPC_SLDL2GL_
#define _IPC_SLDL2GL_

#define RET_OK
#define RET_FAIL

#include <string>
#include <ostream>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <cassert>

class IPC
{
public:
	enum IPC_TYPE {
		CLIENT,
		SERVICE
	};

	IPC();
	virtual ~IPC();
	void create(std::string &_filename);
	void open(std::string &_filename);
	void close();
	void read();
	void write();
	friend std::ostream & operator<<(std::ostream &os, const std::string &_data);
private:
	std::string m_filename;
};

#endif