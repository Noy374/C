#pragma once

#include <iostream>
#include <fstream>

using TInfo = int;

struct NODEBT
{
	TInfo info;
	NODEBT* left=nullptr, *right=nullptr;
	NODEBT(TInfo info) :info(info) {}
	~NODEBT()
	{
		left = right = nullptr;
	}
};

using ptrNODEBT = NODEBT*;

struct BTREE
{
private:
	ptrNODEBT root;
public:
	BTREE() { root = nullptr; }
	BTREE(const char*file_name);
	bool empty();
	TInfo get_info(ptrNODEBT ptr)
	{
		return ptr->info;
	}
	ptrNODEBT get_root()
	{
		return root;
	}
	~BTREE();
	void print(std::ostream& stream = std::cout);
};