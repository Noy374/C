#pragma once

#include <iostream>
#include <fstream>
#include <string>

struct NODETT
{
	NODETT* ptrs[26];
	bool eow = false;
	NODETT()
	{
		for (short j = 0; j < 26; j++)
			ptrs[j] = nullptr;
	};
};

using ptrNODETT = NODETT*;

struct TTREE
{
private:
	ptrNODETT root;

public:
	TTREE() { root = nullptr; }
	TTREE(const char*file_name);
	bool empty();
	bool all_prts_empty(ptrNODETT t);
	ptrNODETT get_root()
	{
		return root;
	}
	void set_root(ptrNODETT t)
	{
		root = t;
	}
	~TTREE();
	void print(bool words, std::ostream& stream = std::cout);
};