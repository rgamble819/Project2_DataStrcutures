// Signature must be retained. Implementation must be added.

#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"
#include "Comparator.h"

template <typename T>
class Sorter {
private:
	// additional member functions (methods) and variables (fields) may be added
	static void merge(ResizableArray<T>& array, long begining, long split, long end, const Comparator<T>& comparator);
	static void mergeSort(ResizableArray<T>& array, long begining, long end, const Comparator<T>& comparator);
	static void quickSort(ResizableArray<T>& array, long begining, long end, const Comparator<T>& comparator);
	static long partition(ResizableArray<T>& array, long begining, long end, const Comparator<T>& comparator);

public:
	static void sort(ResizableArray<T>& array, const Comparator<T>& comparator);
};

#endif

// Sort based on quicksort
template<typename T>
void Sorter<T>::sort(ResizableArray<T>& array, const Comparator<T>& comparator)
{
	//mergeSort(array, 0, array.getSize() - 1L, comparator);
	quickSort(array, 0, (array.getSize() - 1L), comparator);
}

template<typename T>
void Sorter<T>::merge(ResizableArray<T>& array, long begining, long split, long end, const Comparator<T>& comparator)
{
	// Size of the items that must be merged
	long mergedSize = end - begining + 1;

	// Keep track of the begining of the left and begining of the right array
	long left = 0;
	long right = 0;

	// Create a new array to order the items
	ResizableArray<T>* mergedRecords = new ResizableArray<T>[mergedSize];

	// Set the left index to the begining of the left array 
	// and the right to the end of the left + 1
	left = begining;
	right = split + 1;

	// Check that we don't exceed the left and right parition index
	// While they are within the bounds add whichever element is less to the next index in the merged array
	while (left <= split && right <= end)
	{
		if (comparator.compare(array.get(left), array.get(right)) <= 0)
		{
			mergedRecords->add(array.get(left));
			++left;
		}
		else
		{
			mergedRecords->add(array.get(right));
			++right;
		}
	}

	// One of the parition has completed therefore we must add the rest of the other partition
	while (left <= split)
	{
		mergedRecords->add(array.get(left));
		++left;
	}

	while (right <= end)
	{
		mergedRecords->add(array.get(right));
		++right;
	}

	// Add the sorted elements to the original array.
	for (int index = 0; index < mergedSize; ++index)
	{
		array.replaceAt(mergedRecords->get(index), begining + index);
	}

	// Deallocating memory
	delete[] mergedRecords;
	mergedRecords = NULL;
}

template<typename T>
void Sorter<T>::mergeSort(ResizableArray<T>& array, long begining, long end, const Comparator<T>& comparator)
{
	long split = 0L;

	if (begining < end)
	{
		split = (begining + end) / 2L;

		// sort partitions
		mergeSort(array, begining, split, comparator);
		mergeSort(array, split + 1L, end, comparator);

		// Merge partitions
		merge(array, begining, split, end, comparator);
	}
}

// This is the method that initiates quick sort and needs to be called recursivley to each partition.
template<typename T>
void Sorter<T>::quickSort(ResizableArray<T>& array, long begining, long end, const Comparator<T>& comparator)
{
	long partitionLoc = 0;

	// Base case, doesn't let the algorithm progress if the array is 0 or 1 elements. AKA the list is sorted.
	if (begining >= end)
	{
		return;
	}

	// Get parition and swap over lowest and highest elements
	partitionLoc = partition(array, begining, end, comparator);

	// Recursivly call method on partitions remaining.
	quickSort(array, begining, partitionLoc, comparator);
	quickSort(array, partitionLoc + 1, end, comparator);
}

// Partition based on how many elements are greate or less than the pivot
template<typename T>
long Sorter<T>::partition(ResizableArray<T>& array, long begining, long end, const Comparator<T>& comparator)
{
	// Set up all indicies and select mid pivot
	long left = 0;
	long right = 0;
	long mid = (begining + (end)) / 2;
	T pivot = array.get(mid);
	left = begining;
	right = end;

	// Loop control.
	bool complete = false;
	while (!complete)
	{
		// Increment left while the record's value is less than pivot.
		while (comparator.compare(array.get(left), pivot) < 0)
		{
			left++;
		}
		// Decrement right while the record's value is greater than pivot.
		while (comparator.compare(array.get(right), pivot) > 0)
		{
			right--;
		}

		// Check if elements remain to be checked
		if (left >= right)
		{
			complete = true;
		}
		else
		{
			T temp = array.get(left);

			// Swap elements
			array.replaceAt(array.get(right), left);
			array.replaceAt(temp, right);

			left++;
			right--;
		}
	}
	// Return partition
	return right;
}
