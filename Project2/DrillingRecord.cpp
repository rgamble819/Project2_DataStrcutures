#include <string>
#include <sstream>
#include <iomanip>
#include "DrillingRecord.h"

using namespace std;

// Format output for this type
ostream& operator << (ostream& os, const DrillingRecord& record)
{
	for (int i = 0; i < 18; i++)
	{
		if (i < 2)
		{
			os << record.strings[i] << ";";
		}
		else if (i < 17)
		{
			os << fixed << setprecision(2) << record.nums[i - 2] << ";";
		}
		else if (i == 17)
		{
			os << fixed << setprecision(2) << record.nums[i - 2];
		}
	}

	return os;
}

DrillingRecord::DrillingRecord()
{
	for (int i = 0; i < 18; i++)
	{
		if (i < 2)
		{
			this->strings[i] = "";
		}
		else if (i >= 2)
		{
			this->nums[i - 2] = 0.0;
		}
	}
}

// Add a number to record data
void DrillingRecord::addNum(double num)
{
	this->nums[numCtr] = num;
	this->numCtr++;
}

// Add String to record data
void DrillingRecord::addString(string str)
{
	this->strings[strCtr] = str;
	this->strCtr++;
}

// Access a copy of number in record
double DrillingRecord::getNum(unsigned int index) const
{
	return (double)this->nums[index];
}

// Access a copy of string in record
string DrillingRecord::getString(unsigned int index) const
{
	return this->strings[index];
}

void DrillingRecord::setNum(double num, unsigned int index)
{
	this->nums[index] = num;
}

void DrillingRecord::setString(std::string string, unsigned int index)
{
	this->strings[index] = string;
}
