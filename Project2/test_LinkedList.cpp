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
void testRemove();
void testInsert();

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
	testRemove();
	testInsert();

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
	/*// Create linked list structure.
	TestComparator* comparator = new TestComparator();
	OULinkedList<int>* list = new OULinkedList<int>(comparator);

	/// Test hasNext
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

	if (assert(nextCounter, 25)) success("enumerator hasNext()");
	else fail("enumerator hasNext()"); 

	iterator = list->enumerator();

	// Test next
	int countSuccess = 0;
	while (iterator.hasNext()) 
	{
		if (assert(iterator.next(), list->pullFirst())) 
		{
			countSuccess++;
		}
	}

	if (countSuccess == 25) success("enumerator next()");
	else fail("enumerator next()");

	// Test peek
	list->clear();
	list->append(2);
	list->append(3);

	OULinkedListEnumerator<int> iterator2 = list->enumerator();
	
	if (iterator2.peek() == 2) success("enumerator peek()");
	else fail("enumerator peek()");*/
}

void testRemove() 
{
	// Create linked list structure.
	TestComparator* comparator = new TestComparator();
	OULinkedList<int>* list = new OULinkedList<int>(comparator);

	// Test removing an element within the middle of the list.
	int itemsToAdd = 25;
	for (int counter = 0; counter < itemsToAdd; counter++)
	{
		list->append(counter);
	}

	list->remove(5);

	// Test removing first element
	list->remove(0);

	int counter = 0;
	OULinkedListEnumerator<int> iterator = list->enumerator();
	while (iterator.hasNext())
	{
		int nex = iterator.next();
		counter++;
	}

	if (assert(list->contains(5), false)) 
	{
		if (assert(list->contains(0), false)) {
			if (assert(counter, 23)) success("remove");
			else fail("remove");
		}
		else 
		{
			fail("remove");
		}
	}
	else 
	{
		fail("remove");
	}
}

void testInsert() 
{
	// Create linked list structure.
	TestComparator* comparator = new TestComparator();
	OULinkedList<int>* list = new OULinkedList<int>(comparator);

	// Test removing an element within the middle of the list.
	int itemsToAdd = 2;
	for (int counter = 0; counter < itemsToAdd; counter++)
	{
		list->append(counter);
	}

	list->insert(5);

	int counter = 0;
	OULinkedListEnumerator<int> iterator = list->enumerator();

	while (iterator.hasNext())
	{
		int nex = iterator.next();
		counter++;
	}
	
	OULinkedList<int>* list2 = new OULinkedList<int>(comparator);
	list2->append(2);
	list2->append(6);

	list2->insert(3);

	OULinkedListEnumerator<int> iterator2 = list2->enumerator();
	int counter2 = 0;
	while (iterator2.hasNext())
	{
		int nex2 = iterator2.next();
		counter2++;
	}
	
	if (assert(list2->contains(3), true)) success("insert");
	else fail("insert");
}

void success(string test)
{
	cout << test << " was successful!\n";
}

void fail(string test)
{
	cout << test << " failed!\n";
}
