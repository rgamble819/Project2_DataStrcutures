#include <iostream>
#include "OULink.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "OULink.h"
#include "TestComparator.h";

using namespace std;


void testAppend();
void testGetFirst();
void testRemoveFirst();
void testPullFirst();
void testContains();
void testFind();
void testClear();
void testGetSize();
void testEnumerator();

void success(string test);
void fail(string test);


int main()
{	
	testAppend();
	testGetFirst();
	testRemoveFirst();
	testPullFirst();
	testContains();
	testFind();
	testClear();
	testGetSize();
	testEnumerator();


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
	list->append(5);

	// Check if the item was added successfully.
	try 
	{
		if (assert(list->getFirst(), 5))
		{
			list->append(10);
			if(list->contains(10) 
				&& list->getSize() == 2)
			{
				success("append");
			}
		}
		else 
		{
			fail("append");
		}
	}
	catch (ExceptionLinkedListAccess* exc)
	{
		delete exc;
		exc = nullptr;
		fail("append");
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

void testRemoveFirst() 
{
	// Create linked list structure.
	TestComparator* comparator = new TestComparator();
	OULinkedList<int>* list = new OULinkedList<int>(comparator);

	// Add an item to the list
	list->append(5);

	list->removeFirst();

	try {
		if (list->getFirst())
		{
			fail("removeFirst");
		}
	}
	catch(ExceptionLinkedListAccess* exc)
	{
		delete exc;
		exc = NULL;

		success("removeFirst");
	}
}

void testPullFirst()
{
	// Create linked list structure.
	TestComparator* comparator = new TestComparator();
	OULinkedList<int>* list = new OULinkedList<int>(comparator);

	// Add an item to the list
	list->append(5);

	if(assert(list->pullFirst(), 5))
	{
		try 
		{
			list->getFirst();
			fail("pillFirst");
		}
		catch (ExceptionLinkedListAccess* exc) 
		{
			delete exc;
			exc = NULL;
			success("pullFirst");
		}
	}
	else 
	{
		fail("pullFirst");
	}
}

void testContains()
{
	// Create linked list structure.
	TestComparator* comparator = new TestComparator();
	OULinkedList<int>* list = new OULinkedList<int>(comparator);

	list->append(5);
	list->append(10);

	if (assert(list->contains(5), true) 
		&& assert(list->contains(10), true) 
		&& assert(list->contains(7), false)) 
	{
		success("contains");
	}
}

void testFind() 
{
	// Create linked list structure.
	TestComparator* comparator = new TestComparator();
	OULinkedList<int>* list = new OULinkedList<int>(comparator);

	list->append(5);
	list->append(10);

	if (assert(list->find(10), 10))
	{
		try 
		{
			list->find(7);
			fail("find");
		}
		catch (ExceptionLinkedListAccess* exc) 
		{
			delete exc;
			exc = NULL;
			success("find");
		}
	}
	else 
	{
		fail("find");
	}
}

void testClear()
{
	// Create linked list structure.
	TestComparator* comparator = new TestComparator();
	OULinkedList<int>* list = new OULinkedList<int>(comparator);

	// Add an item to the list
	int itemsToAdd = 25;
	for (int counter = 0; counter <= itemsToAdd; counter++) 
	{
		list->append(counter);
	}

	list->clear();

	try 
	{
		if (assert(list->contains(5), true)) 
		{
			fail("clear");
			return;
		}
		assert(list->getFirst(), 0);
		fail("clear");
	}
	catch (ExceptionLinkedListAccess* exc) 
	{
		delete exc;
		exc = NULL;

		success("clear");
	}
}

void testGetSize() 
{
	// Create linked list structure.
	TestComparator* comparator = new TestComparator();
	OULinkedList<int>* list = new OULinkedList<int>(comparator);

	int itemsToAdd = 25;
	for (int counter = 0; counter < itemsToAdd; counter++)
	{
		list->append(counter);
	}

	if (assert(list->getSize(), (unsigned long)25)) 
	{
		success("getSize");
	}
	else 
	{
		fail("getSize");
	}
}

void testEnumerator() 
{
	// Create linked list structure.
	TestComparator* comparator = new TestComparator();
	OULinkedList<int>* list = new OULinkedList<int>(comparator);

	int itemsToAdd = 25;
	for (int counter = 0; counter < itemsToAdd; counter++)
	{
		list->append(counter);
	}

	OULinkedListEnumerator<int> iterator = list->enumerator();

	int nextCounter = 0;
	while (iterator.hasNext()) 
	{
		iterator.next();
		nextCounter++;
	}

	if (assert(nextCounter, 24)) success("enumerator hasNext()");
	else fail("enumerator hasNext()"); 

	iterator = list->enumerator();

	int countSuccess = 0;
	while (iterator.hasNext()) 
	{
		list->pullFirst();
		if (assert(iterator.next(), list->getFirst())) 
		{
			countSuccess++;
		}
	}

	if (countSuccess == 24) success("enumerator next()");
	else fail("enumerator next()");

	list->clear();
	list->append(2);
	list->append(3);

	OULinkedListEnumerator<int> iterator2 = list->enumerator();
	
	if (iterator2.peek() == 3) success("enumerator peek()");
	else fail("enumerator peek()");
}

void success(string test)
{
	cout << test << " was successful!\n";
}

void fail(string test)
{
	cout << test << " failed!\n";
}
