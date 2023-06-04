#pragma once

#include <iostream>
#include <fstream>

using TInfo = int;

struct NODEDL
{
	TInfo info;
	NODEDL* next, *prev;
	NODEDL(TInfo info, NODEDL* next = nullptr, NODEDL* prev = nullptr) :info(info), next(next), prev(prev) {}
	~NODEDL()
	{
		next = prev = nullptr;
	}
};

using ptrNODEDL = NODEDL *;


struct DLIST
{
private:
	ptrNODEDL begin, end;
public:
	DLIST() { begin = nullptr; end = nullptr; }
	DLIST(const char* file_name);
	void first_node(TInfo elem);
	bool empty();
	void add_after(ptrNODEDL ptr, TInfo elem);
	void add_before(ptrNODEDL ptr, TInfo elem);
	void print(std::ostream& stream = std::cout);
	~DLIST();
	void delete_before(const ptrNODEDL &ptr);
	void delete_after(const ptrNODEDL &ptr);
	ptrNODEDL get_begin()
	{
		return begin;
	}
	ptrNODEDL get_end()
	{
		return end;
	}
};
