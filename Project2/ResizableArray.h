#ifndef RESIZABLE_ARRAY_H
#define RESIZABLE_ARRAY_H

#include "Exceptions.h"

const unsigned long DEFAULT_ARRAY_CAPACITY = 10;        // capacity used in no arg constructor

template <typename T>
class ResizableArray {
private:
	unsigned long capacity = DEFAULT_ARRAY_CAPACITY;    // maximum capacity, in items
	unsigned long size = 0;                             // actual number of items currently in array
	T* data = nullptr;                                     // pointer to array of any type
	void doubleCapacity();                              // method to double array capacity
	void halveCapacity();                               // method to halve array capacity
public:
	ResizableArray();                                   // constructs array with default capacity
	ResizableArray(unsigned long capacity);             // constructs array with specified capacity
	virtual ~ResizableArray();                          // frees array space as object is deleted
	void add(T item);                                   // adds item, increments size, doubles capacity as necessary

	// all indexed member functions must throw ExceptionIndexOutOfRange for bad index values
	void addAt(T item, unsigned long index);            // adds item at index, shifts following, doubles capacity as necessary
	void replaceAt(T item, unsigned long index);        // replaces item at index, otherwise unchanged
	void removeAt(unsigned long index);                 // removes item at index, shifts following back
	T get(unsigned long index) const;                   // returns (copy of) item at index
	T operator[](unsigned long index) const;            // returns (copy of) item at index

	unsigned long getSize() const;                      // returns number of items currently in array
	unsigned long getCapacity() const;                  // returns the current capacity of the array
};

// Add your implementation below this line. Do not add or modify anything above this line.


#endif


template<typename T>
void ResizableArray<T>::doubleCapacity()
{
	capacity = capacity * 2;

	// Create new array
	T* newData = new T[capacity];
	if (newData == NULL) throw new ExceptionMemoryNotAvailable;

	// Copy vals
	int newIndex = 0;
	for (unsigned long oldIndex = 0; oldIndex < size; oldIndex++)
	{
		newData[newIndex] = data[oldIndex];
		newIndex++;
	}

	//Delete old data
	delete[] data;
	data = NULL;

	// Set new data array.
	data = newData;
}

template<typename T>
void ResizableArray<T>::halveCapacity()
{
	capacity = (capacity / 2);

	// Create new array
	T* newData = new T[capacity];
	if (newData == NULL) throw new ExceptionMemoryNotAvailable;

	// Copy vals
	int newIndex = 0;
	for (unsigned long oldIndex = 0; oldIndex < size; oldIndex)
	{
		newData[newIndex] = data[oldIndex];
		newIndex++;
	}

	// Delete old data
	delete[] data;
	data = NULL;

	// Set new data array
	data = newData;
}

template<typename T>
ResizableArray<T>::ResizableArray()
{
	data = new T[capacity];
	if (data == NULL) throw new ExceptionMemoryNotAvailable;
}

template<typename T>
ResizableArray<T>::ResizableArray(unsigned long capacity)
{
	this->capacity = capacity;
	data = new T[capacity];
	if (data == NULL) throw new ExceptionMemoryNotAvailable;
}

template<typename T>
ResizableArray<T>::~ResizableArray() {
	delete[] data;
	data = NULL;
}

template<typename T>
void ResizableArray<T>::add(T item)
{
	// Check if doubling is neccessary
	if (size == capacity)
	{
		doubleCapacity();
	}
	// Add item and increment size
	data[size++] = item;
}

template<typename T>
void ResizableArray<T>::addAt(T item, unsigned long index)
{
	// check if index is valid
	if (index >= size || index < 0)
	{
		throw new ExceptionIndexOutOfRange();
	}

	// Check if there is space in the array
	if (size == capacity)
	{
		doubleCapacity();
	}

	// Shift all Elements from specified index
	long indexToShift = size - 1L;
	while (indexToShift >= 0L)
	{
		data[indexToShift + 1L] = data[indexToShift];
		indexToShift--;
	}

	// Add data to the array
	data[index] = item;
	size++;
}

template<typename T>
void ResizableArray<T>::replaceAt(T item, unsigned long index)
{
	// Check if index is valid
	if (index >= size || index < 0)
	{
		throw new ExceptionIndexOutOfRange();
	}

	// Replace item
	data[index] = item;
}

template<typename T>
void ResizableArray<T>::removeAt(unsigned long index)
{

	// Check if valid index.
	if (index >= size || index < 0)
	{
		throw new ExceptionIndexOutOfRange();
	}

	// Check if there are elements to remove
	if (size > 0)
	{
		// Shift elements to the right
		for (unsigned long j = index; j < size - 1; j++)
		{
			data[j] = data[j + 1];
		}
	}
	// TODO: MAKES SURE TO HALF THIS CAPACITY
	size--; // Adjust size
}

template<typename T>
T ResizableArray<T>::get(unsigned long index) const
{
	// Check if index is valid.
	if (index >= size || index < 0)
	{
		throw new ExceptionIndexOutOfRange();
	}
	return data[index];
}

template<typename T>
T ResizableArray<T>::operator[](unsigned long index) const
{
	if (index < size || index < 0)
	{
		throw new ExceptionIndexOutOfRange;
	}
	return data[index];
}

template<typename T>
unsigned long ResizableArray<T>::getSize() const
{
	return size;
}

template<typename T>
unsigned long ResizableArray<T>::getCapacity() const
{
	return capacity;
}

