#ifndef SEARCH_H
#define SEARCH_H

#include "OULink.h"
#include "OULinkedList.h"

// Returns the node that contains the item.
template <typename T>
OULink<T> linearSearch(OULinkedList<T> linkedList, T item) 
{
	OULink<T> currentElement = linkedList->first;

	while (currentElement != linkedList->last)
	{

	}
}
#endif