#include "Double_List.h"

DLIST::DLIST(const char* file_name)
{
	std::ifstream file(file_name);
	TInfo elem;
	file >> elem;
	first_node(elem);
	while (file >> elem)
		add_after(end, elem);
	file.close();
}

void DLIST::first_node(TInfo elem)
{
	begin = new NODEDL(elem);
	end = begin;
}

bool DLIST::empty()
{
	return begin == nullptr;
}

void DLIST::add_after(ptrNODEDL ptr, TInfo elem)
{
	ptrNODEDL p = new NODEDL(elem, ptr->next, ptr);
	if (ptr == end)
		end = p;
	else
		ptr->next->prev = p;
	ptr->next = p;
}

void DLIST::add_before(ptrNODEDL ptr, TInfo elem)
{
	ptrNODEDL p = new NODEDL(elem, ptr, ptr->prev);
	if (ptr == begin)
		begin = p;
	else
		ptr->prev->next = p;
	ptr->prev = p;
}

void DLIST::print(std::ostream& stream)
{
	ptrNODEDL ptr = begin;
	while (ptr)
	{
		stream << ptr->info << ' ';
		ptr = ptr->next;
	}
	stream << '\n';
}

DLIST::~DLIST()
{
	while (begin->next)
		delete_after(begin);
	delete begin;
	end = begin = nullptr;
}

void DLIST::delete_before(const ptrNODEDL & ptr)
{
	ptrNODEDL p = ptr->prev;
	if (p == begin)
	{
		begin = p->next;
		p->next->prev = nullptr;
	}
	else
	{
		p->next->prev = p->prev;
		p->prev->next = p->next;
	}
	delete p;
}

void DLIST::delete_after(const ptrNODEDL & ptr)
{
	ptrNODEDL p = ptr->next;
	if (p == end)
	{
		end = p->prev;
		p->prev->next = nullptr;
	}
	else
	{
		p->next->prev = p->prev;
		p->prev->next = p->next;
	}
	delete p;
}
