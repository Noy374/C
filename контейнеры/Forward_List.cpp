#include "Forward_List.h"

FLIST::FLIST(const char * file_name)
{
	head = new NODEL(0);
	std::ifstream file(file_name);
	TInfo x;
	ptrNODEL tail = head;
	while (file >> x)
	{
		adding_by_pointer(tail->next, x);
		tail = tail->next;
	}
	file.close();
}

bool FLIST::empty()
{
	return !head->next;
}

void FLIST::adding_by_pointer(ptrNODEL & q, TInfo elem)
{
	q = new NODEL(elem, q);
}

void FLIST::delete_by_pointer(ptrNODEL & q)
{
	ptrNODEL p = q;
	q = p->next;
	delete p;
}

FLIST::~FLIST()
{
	while (!empty())
		delete_by_pointer(head);
	delete head;
}

void FLIST::print(std::ostream& stream)
{
	if (empty())
		stream << "empty";
	else
	{
		ptrNODEL p = head->next;
		while (p)
		{
			stream << p->info << ' ';
			p = p->next;
		}
		stream << '\n';
	}
}


