#pragma once
#include "Machine.h"

class Dictionary
{
public:
	string key;
	string value;
	Dictionary* next;
	Dictionary(string key, string value)
	{
		this->key = key;
		this->value = value;
		next = NULL;
	}
};

class taskManagementSystem
{
public:
	Machine* head;
	Dictionary* dictionary;
	int identifier_bits;
	int machID;

	string hashingFunc(string s1);
	taskManagementSystem();
	void insertMachine(string,int);
	void deleteMachine(string);
	void dividerange();
	void reMapBtree(Machine*,Machine*);
	string generateID();
	void insertData(string);
	void removeData();
	string& search(string);
};

