#include "Binary_Tree.h"

void add(ptrNODEBT &t, int elem)
{
	if (!t)
		t = new NODEBT(elem);
	else
		if (elem < t->info)
			add(t->left, elem);
		else
			if (elem > t->info)
				add(t->right, elem);
}

BTREE::BTREE(const char * file_name)
{
	std::ifstream file(file_name);
	TInfo x;
	root = nullptr;
	while (file >> x)
		add(root, x);
	file.close();
}

bool BTREE::empty()
{
	return !root;
}

void clear(ptrNODEBT &t)
{
	if (t)
	{
		clear(t->left);
		clear(t->right);
		delete t;
		t = nullptr;
	}
}

BTREE::~BTREE()
{
	clear(root);
}

void printt(ptrNODEBT t, int level, std::ostream & stream)
{
	if (t)
	{
		printt(t->right, level + 1, stream);
		for (int i = 1; i <= level; i++)
			stream << "  ";
		stream << t->info << std::endl;
		printt(t->left, level + 1, stream);
	}
}
void BTREE::print(std::ostream & stream)
{
	printt(root, 0, stream);
}
