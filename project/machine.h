#pragma once
#include "BTree.h"
class Machine;

class RoutingTable
{
public:
	Machine* data;
	RoutingTable* next;
	RoutingTable* prev;

	RoutingTable(Machine*);
};

class Machine
{
public:	
	string name;
	string hash;
	int identifier_bits;
	int order;
	BTree* data;
	RoutingTable* head;
	Machine* next;

	Machine(string&, string&, int);
	void PrintRoutingTable();
	void PrintBTree();
	void SearchinRoutingTable();
	void CreateRouting(int);
	void deleteTable();
	void insertData(string,string);
	void deleteData(string);
	string search(string);

};


