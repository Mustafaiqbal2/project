#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include "taskManagementSystem.h" 
#include <Windows.h>
#include "sha1.hpp"
//#include "support.h"
using namespace std;

taskManagementSystem::taskManagementSystem()
{
	numberMachines = 0;
	int no_machines;
	long double test = 0;

	do
	{

		cout << "ENTER NUMBER OF IDENTIFIER BITS: \t";
		cin >> identifier_bits;

		if (identifier_bits <= 0)
			cout << "\n\nIDENTIFIER BITS CANNOT BE 0 or less\n\n";
	} 
	while (identifier_bits <= 0);

	cout<<"ENTER ORDER OF B-TREE: \t";
	cin >>order;

	do
	{
		cout << "ENTER NUMBER OF MACHINES: \t";
		cin >> no_machines;

		test = pow(no_machines, 0.5);
		if ((test > identifier_bits))
			cout << "\n\nMACHINES CANNOT EXCEED POSIBLE FILES\n\n";
	}
	while (test > identifier_bits);

	cout << "PROCESS TO INSERT MACHINES\n\n";

	for(int i=0;i<no_machines;i++)
	{
		insertMachine();
	}
	Machine* temp = head;

	cout << "MACHINES IN RING DHT\n\n";

	while (temp->next != head)
	{
		cout << temp->name << " | " << temp->hash << " --> ";
		temp = temp->next;
	}
	cout << temp->name << " | " << temp->hash << endl;

	temp = head;

	while (temp->next != head)
	{
		temp->PrintRoutingTable();
		temp = temp->next;
	}
	temp->PrintRoutingTable();

	machID = 1;

	cout<<"PRESS ANY KEY TO CONTINUE\n\n";
	int x;
	cin >> x;

	system("cls");

	for (int i = 0; i < 10; i++)
	{
		insertData();
	}
}


string taskManagementSystem::hashingFunc(string s1)
{
	SHA1 checksum;
	checksum.update(s1);
	string s2= checksum.final();
	int bits = identifier_bits;
	bits/= 4;
	int upper = identifier_bits % 4;

	int size = s2.size();
	size--;
	if(upper == 0)
		s2 = s2.substr(size - bits + 1, size);
	else
	{
		s2 = s2.substr(size - bits, size);
		char msb = s2[0];
		int temp;
		if (msb >= 48 && msb <= 57)
		{
			temp = msb - 48;
		}
		else
		{
			switch (msb)
			{
			case 'f':
				temp = 15;
				break;
			case 'e':
				temp = 14;

				break;
			case 'd':
				temp = 13;

				break;
			case 'c':
				temp = 12;

				break;
			case 'b':
				temp = 11;

				break;
			case 'a':
				temp = 10;
				break;

			}
		}
		temp &= int(pow(2,upper))-1;
		if (temp >= 0 && temp <= 9)
		{
			msb = temp + 48;
		}
		else
		{
			switch (temp)
			{
			case 10:
				msb = 'a';
				break;
			case 11:
				msb = 'b';
				break;
			case 12:
				msb = 'c';
				break;
			case 13:
				msb = 'd';
				break;
			case 14:
				msb = 'e';
				break;
			case 15:
				msb = 'f';
				break;

			}
		}
		s2[0] = msb;
	}



	return s2;
}

void taskManagementSystem::insertMachine()
{
	char choice;
	string manualID;
	Machine* temp = 0;
	cout << "WOULD YOU LIKE TO ASSIGN A MACHINE ID? (y/n)\n\n";
	cin >> choice;
	if (choice == 'y')
	{
		cout << "ENTER MACHINE ID:\t";
		cin.ignore();
		getline(cin,manualID);
		string hash= hashingFunc(manualID);
		temp = new Machine(manualID, hash, identifier_bits, order);
	}
	else
	{
		cout << "GENERATING MACHINE ID....\n";
		manualID = generateID();
		string hash = hashingFunc(manualID);
		temp = new Machine(manualID, hash, identifier_bits, order);
		machID++;
	}
	if (head == NULL)
	{
		head = temp;
		temp->next = head;
	}
	else
	{
		Machine* temp2 = head;
		bool flag = 1;
		if (strcmp(head->hash, temp->hash))
		{
			while (temp2->next != head)
			{
				temp2 = temp2->next;
			}
			temp2->next = temp;
			temp->next = head;
			head = temp;
			flag = 0;
		}
		while (temp2->next != head && flag)
		{
			if (strcmp(temp->hash, temp2->hash) && strcmp(temp2->next->hash, temp->hash))
			{
				temp->next = temp2->next;
				temp2->next = temp;
				flag = 0;
				break;
			}
			temp2 = temp2->next;
		}
		if (flag)
		{
			temp->next = temp2->next;
			temp2->next = temp;
		}
	}
	numberMachines++;

	if (CreateDirectory(hash, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		std::cout << "Folder created successfully.\n";
	}
	else {
		std::cerr << "Error creating the folder.\n";
	}

	// At this point machine is stored in ring dht
	
	//Functionality for bTree Splitting and Routing table creation
	LinkedList temp2 = temp->next->createLinkedList();

	Data* temp3 = temp2.head;
	while (temp3 != 0)
	{
		if (strcmp(temp->hash, temp3->key))
		{
			path = temp3->filepath;
			path = path.substr(temp->hash.size, path.size() - 1);
			temp->next->data->DeleteNode(temp3->key);
			temp->data->insert(temp3->key, path);
		}
		temp3 = temp3->next;
	}

	Machine* temp4 = head;
	do
	{
		temp4->CreateRouting(numberMachines);
		temp4 = temp4->next;
	} 
	while (temp4->next != head);
	temp4->CreateRouting(numberMachines);
}
void taskManagementSystem::deleteMachine(string hash)
{
	string key;
	Machine* temp = 0;
	cout << "ENTER MACHINE KEY YOU WANT TO DELETE\n\n";
	getline(cin, key);
	
	if (head == NULL)
	{
		return;
	}
	else
	{
		Machine* temp2 = head;
		Machine* Prev = head;
		do
		{
			if (temp2->hash == key)
			{
				temp = temp2;
				break;
			}
			Prev = temp2;
			temp2 = temp2->next;
		} while (temp2 != head);
		prev->next = temp->next;

		LinkedList temp2 = temp->createLinkedList();

		Data* temp3 = temp2.head;

		while (temp3 != 0)
		{
			path = temp3->filepath;
			path = path.substr(temp->hash.size, path.size() - 1);
			temp->next->data->DeleteNode(temp3->key);
			temp->data->insert(temp3->key, path);
			temp3 = temp3->next;
		}

	}
	if (DeleteDirectory(temp->hash, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		std::cout << "Folder deleted successfully.\n";
	}
	else {
		std::cerr << "Error deleting the folder.\n";
	}
	numberMachines--;

	Machine* temp4 = head;
	do
	{
		temp4->CreateRouting(numberMachines);
		temp4 = temp4->next;
	} while (temp4->next != head);
	temp4->CreateRouting(numberMachines);
}

string taskManagementSystem::generateID()
{
	string mach = "Machine ";
	mach+= char(machID + 48);
	return mach;
}
void taskManagementSystem::insertData()
{
	string filename;
	string content;

	cout << "\n\nnote: cin.ignore() is used if it looks like infinite loop then press enter\n\n";

	cin.ignore();
	cout<<"ENTER FILE NAME: \t";
	getline(cin, filename);

	cout<<"ENTER FILE CONTENT: \t";
	getline(cin, content);

	string hash = hashingFunc(filename);

	head->insertData(filename, hash);
}
void taskManagementSystem::removeData()
{
	string key;
	cout << "\n\nnote: cin.ignore() is used if it looks like infinite loop then press enter\n\n";

	cout << "ENTER FILE KEY: \t";
	getline(cin, key);

	head->deleteData(key);
}
void taskManagementSystem::search()
{
	string key;

	cout << "ENTER KEY: \t";
	getline(cin, key);


	string query;
	cout << "ENTER MACHINE NAME TO START QUERY FROM: \t";
	getline(cin, query);

	Machine* temp = head;
	string path;
	do
	{
		if (temp->name == query)
		{
			path = temp->search(key);
			break;
		}
	} while (temp != head);
	//FILE HANDLING OPERATORS
}