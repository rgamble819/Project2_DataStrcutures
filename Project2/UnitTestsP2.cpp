#include <iostream>
#include "OULink.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "OULink.h"
#include "TestComparator.h";

using namespace std;


void testAppend();
void testGetFirst();
void testFind();

void success(string test);
void fail(string test);


int main()
{
	testGetFirst();
	testAppend();

	testFind();
	return -1;
}

template<typename T>
bool assert(T item1, T item2) 
{
	if (item1 == item2) 
	{
		return true;
	}

	return false;
}

void testAppend() 
{
	// Create linked list structure.
	TestComparator* comparator = new TestComparator();
	OULinkedList<int>* list = new OULinkedList<int>(comparator);

	// Add an item to the list
	if (list->append(5)) 
	{
		cout << "5 was added.\n";
	}
	else 
	{
		cout << "5 was not added.\n";
	}

	// Check if the item was added successfully.
	try 
	{
		if (assert(list->getFirst(), 5))
		{
			list->append(10);
			/*if(list->contains(10))
			{
				success("Append");
				return;
			}*/
		}
		else 
		{
			fail("Append");
		}
	}
	catch (ExceptionLinkedListAccess* exc)
	{
		delete exc;
		exc = nullptr;
		fail("Append");
	}
}

void testGetFirst() 
{
	// Create linked list structure.
	TestComparator* comparator = new TestComparator();
	OULinkedList<int>* list = new OULinkedList<int>(comparator);

	// Add an item to the list
	list->append(5);

	if (assert(list->getFirst(), 5)) 
	{
		success("getFirst");
	}
	else 
	{
		fail("getFirst");
	}
}

void success(string test)
{
	cout << test << " was successful!\n";
}

void fail(string test)
{
	cout << test << " failed!\n";
}
