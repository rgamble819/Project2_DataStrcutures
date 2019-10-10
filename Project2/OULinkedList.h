#ifndef OU_LINKED_LIST
#define OU_LINKED_LIST

#include "OULink.h"
#include "Comparator.h"
#include "OULinkedListEnumerator.h"

// OULinkedList stands for Ordered, Unique Linked List. It is a linked list that is always maintained in
// order (based on the comparator provided to it when the list is created) and that only contains unique
// items (that is, duplicates are not allowed)
template <typename T>
class OULinkedList {
	//template <typename T>
	//friend class OULinkedListEnumerator;
private:
	Comparator<T>* comparator = NULL;               // used to determine list order and item equality
	unsigned long size = 0;                         // actual number of items currently in list
	OULink<T>* first = NULL;                        // pointer to first link in list
	OULink<T>* last = NULL;                         // pointer to last link in list
public:
	OULinkedList(Comparator<T>* comparator);        // creates empty linked list with comparator
	virtual ~OULinkedList();                        // deletes all links and their data items

	// if an equivalent item is not already present, insert item in order and return true
	// if an equivalent item is already present, leave list unchanged and return false
	bool insert(T item);

	// if item is greater than item at last, append item at end and return true
	// if item is less than or equal to item at last, leave list unchanged and return false
	bool append(T item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave list unchanged and return false
	bool replace(T item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave list unchanged and return false
	bool remove(T item);

	// if any items are present, return a copy of the first item
	// if no items are present, throw new ExceptionLinkedListAccess
	T getFirst() const;

	// if any items are present, return a copy of the first item, remove it from list
	// if no items are present, throw new ExceptionLinkedListAccess
	T pullFirst();

	// if any items are present, remove the first item and return true
	// if no items are present, leave list unchanged and return false
	bool removeFirst();

	// if an equivalent item is present, return true
	// if an equivalent item is not present, false
	bool contains(T item) const;

	// if an equivalent item is present, return a copy of that item
	// if an equivalent item is not present, throw a new ExceptionLinkedListAccess
	T find(T item) const;

	void clear();                                     // deletes all links in the list, resets size to 0

	unsigned long getSize() const;                    // returns the current number of items in the list

	OULinkedListEnumerator<T> enumerator() const;     // create an enumerator for this linked list
};

// Add your implementation below this line. Do not add or modify anything above this line.


template<typename T>
OULinkedList<T>::OULinkedList(Comparator<T>* comparator)
{
	// Creates new LinkedList object and sets the comparator to order the list.
	this->comparator = comparator;
}

template<typename T>
OULinkedList<T>::~OULinkedList()
{
	delete comparator;
	comparator = nullptr;

	delete first;
	first = nullptr;

	delete last;
	last = nullptr;
}


// if item is greater than item at last, append item at end and return true
// if item is less than or equal to item at last, leave list unchanged and return false
template<typename T>
bool OULinkedList<T>::append(T item)
{
	if (comparator->compare(item, last->data) == 1)
	{
		OULink<T>* nextNode = new OULink<T>(item);
		last->next = nextNode;
		return true;
	}
	return false;
}


template<typename T>
bool OULinkedList<T>::removeFirst()
{
	OULink<T> firstItem = first;
	OULink<T> tempPtr = first->next;

	delete firstItem;
	firstItem = nullptr;

	first = tempPtr;
}

template<typename T>
bool OULinkedList<T>::contains(T item) const
{
	// Track next item
	T* itemToCheck = first.data;

	// Count up until size is reached
	long checkCount = 0;
	while (checkCount < size)
	{
		// Compare the itemToCheck with item it it equals, return the item found.
		if (comparator->compare(itemToCheck, item) == 0)
		{
			return true;
		}
		itemToCheck = first->next.data;
		checkCount++;
	}
	throw new ExceptionLinkedListAccess();
}

template<typename T>
T OULinkedList<T>::find(T item) const
{
	// Track next item
	T* itemToCheck = first->data;

	// Count up until size is reached
	long checkCount = 0;
	while (checkCount < size)
	{
		// Compare the itemToCheck with item it it equals, return the item found.
		if (comparator->compare(itemToCheck, item) == 0)
		{
			return itemToCheck;
		}
		itemToCheck = first->next->data;
		checkCount++;
	}
	throw new ExceptionLinkedListAccess();
}

// Deletes all links in the list and resets size to 0
template<typename T>
void OULinkedList<T>::clear()
{
	// Track first element
	T* temp = first->next;
	while (size > 0)
	{
		delete first;
		first = nullptr;

		first = temp;

		// Set tracked to subsequent element.
		temp = first->next;
		size--;
	}
}

// returns the current number of items in the list
template<typename T>
unsigned long OULinkedList<T>::getSize() const
{
	return size;
}

// Creates and returns an enumerator for this linked list
template<typename T>
OULinkedListEnumerator<T> OULinkedList<T>::enumerator() const
{
	return OULinkedListEnumerator<T>(first);
}


#endif // !OU_LINKED_LIST
