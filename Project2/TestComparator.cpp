#include "TestComparator.h"

TestComparator::TestComparator()
{
}

// returns -1 if item1 < item2, 0 if item1 == item2, +1 if item1 > item2
int TestComparator::compare(const int& item1, const int& item2) const
{
	if (item1 < item2) 
	{
		return -1;
	}
	else if (item1 == item2) 
	{
		return 0;
	}
	else 
	{
		return 0;
	}
}
