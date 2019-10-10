#ifndef DRILLING_RECORD_COMPARATOR_H
#define DRILLING_RECORD_COMPARATOR_H

#include "Comparator.h"

class TestComparator : public Comparator<int> {
public:
	TestComparator();

	// returns -1 if item1 < item2, 0 if item1 == item2, +1 if item1 > item2
	int compare(const int& item1, const int& item2) const;
};

// You must create a corresponding DrillingRecordComparator.cpp file to implement the constructor and member function compare().

#endif