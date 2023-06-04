#pragma once
#include <iostream>
#include <fstream>

using TInfo = int;

struct NODEL
{
	TInfo info;
	NODEL* next;
	NODEL(TInfo info, NODEL* ptr = nullptr) :info(info), next(ptr) {}
	~NODEL()
	{
		next = nullptr;
	}
};

using ptrNODEL = NODEL*;

struct FLIST
{
private:
	ptrNODEL head;

public:
	FLIST(const char*file_name);
	bool empty();
	TInfo get_info(ptrNODEL ptr)
	{
		return ptr->info;
	}
	ptrNODEL get_head()
	{
		return head;
	}
	~FLIST();
	void adding_by_pointer(ptrNODEL& q, TInfo elem);
	void delete_by_pointer(ptrNODEL& q);
	void print(std::ostream& stream = std::cout);
};
