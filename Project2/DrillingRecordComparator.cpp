#include "DrillingRecordComparator.h"
#include <string>
using namespace std;

DrillingRecordComparator::DrillingRecordComparator(unsigned int column)
{
	this->column = column;
}

int DrillingRecordComparator::compare(const DrillingRecord& item1, const DrillingRecord& item2) const
{
	// Comparing strings
	if (column < 2)
	{
		string first = item1.getString(column);
		string second = item2.getString(column);

		return first.compare(second);
	}
	// Comparing doubles
	else
	{
		double first = item1.getNum(column - 2);
		double second = item2.getNum(column - 2);
		if (first < second)
		{
			return -1;
		}
		else if (first == second)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}