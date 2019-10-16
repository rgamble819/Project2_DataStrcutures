// Signature must be retained. Implementation must be added.

#ifndef SEARCH_H
#define SEARCH_H

#include "Exceptions.h"
#include "ResizableArray.h"
#include "Comparator.h"
#include "DrillingRecordComparator.h"

// returns FIRST array location matching the given item (based on the comparator)
// if not found, returns -(location to insert + 1)
// Also returns lowest index of duplicate values.
template <typename T>
long long binarySearch(const T& item, const ResizableArray<T>& array, const Comparator<T>& comparator)
{
	long high = array.getSize() - 1L;
	long low = 0L;
	long mid = 0L;

	while (high >= low)
	{
		mid = (low + high) / 2L;
		if (comparator.compare(item, array.get(mid)) > 0)
		{

			low = mid + 1L;
		}
		else if (comparator.compare(item, array.get(mid)) < 0)
		{
			high = mid - 1L;
		}
		else
		{
			// Binary search modification

			// Save index where item was found.
			long indexDown = mid;
			if (indexDown > 0)
			{
				// Loop through index below to get the lowest index of duplicate value.
				while (indexDown > 0)
				{
					indexDown--;
					if (comparator.compare(array.get(mid), array.get(indexDown)) == 0) {
						mid = indexDown;
					}
					else
					{
						break;
					}
				}
			}
			/////// End of modification. //////////

			return mid;
		}
	}
	return -1;
}

// Linear search through a resizable array.
template <typename T>
long long* linearSearch(const T& item, const ResizableArray<T>& array, const Comparator<T>& comparator)
{
	long long* recordIndexFound = new long long[array.getSize()];

	int initialize = (int)array.getSize() - 1;
	while (initialize >= 0)
	{
		recordIndexFound[initialize] = -1;
		initialize--;
	}

	unsigned long indexToLook = 0;
	long indexFoundTracker = 0;
	while (indexToLook < array.getSize())
	{
		if (comparator.compare(item, array.get(indexToLook)) == 0)
		{
			recordIndexFound[indexFoundTracker] = indexToLook;
			indexFoundTracker++;
		}
		indexToLook++;
	}
	return recordIndexFound;
}
#endif