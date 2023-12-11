#pragma once
#include <string>
using namespace std;

class Data
{
public:
	string key;
	string filepath;
	Data* next;
	Data* chain;
	Data()
	{
		next = nullptr;
		chain = nullptr;
	}
	Data(string& k1, string& c1)
	{
		key = k1;
		filepath = c1;
		next = nullptr;
		chain = nullptr;
	}
};

class DataList
{
public:
	Data* head;
	DataList()
	{
		head = nullptr;
	}
	void add(Data& node)
	{
		if (head == nullptr)
		{
			head = &node;
			return;
		}
		Data* tmp = head;
		while (tmp->next != nullptr)
		{
			tmp = tmp->next;
		}
		tmp->next = &node;
	}
	void deleteList(Data* temp)
	{
		if (temp == 0)
		{
			return;
		}
		else
		{
			deleteList(temp->next);
			temp->next = 0;
			return;
		}
	}
	~DataList()
	{
		deleteList(head);
		head = nullptr;
	}
};

