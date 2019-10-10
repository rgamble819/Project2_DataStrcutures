#include <iostream>
#include "OULink.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "OULink.h"
#include "TestComparator.h";

using namespace std;

void testFind();
void testAppend();

int main()
{
	testFind();
	testAppend();

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

	list->append(5);

	if (assert(list->contains(5), true)) 
	{
		cout << "Append test was successful!";
	}
}
