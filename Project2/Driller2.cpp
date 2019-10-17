#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include"DrillingRecord.h"
#include"Search.h"
#include "DrillingRecordComparator.h"
#include"Sorter.h"
#include"OULinkedList.h"
#include"OULinkedListEnumerator.h"

void drillerOutput(OULinkedList<DrillingRecord>* drillList, int* fileData); // Output data to specified location

void dataInputLoop(OULinkedList<DrillingRecord>* drillList, int* drillInfo); // Begin data inputloop
void readFileStream(OULinkedList<DrillingRecord>* drillList, std::ifstream& in, int* lineData); // opens file stream
DrillingRecord* parseRow(OULinkedList<DrillingRecord>* drillList, std::string lineOfData, int row); // Parses each row.

template <typename T>
void mergeLists(OULinkedList<T>* originalList, OULinkedList<T>* newList);

void dataManipulationLoop(OULinkedList<DrillingRecord>* drillList, int* fileData);

bool checkDateStamp(DrillingRecord* recordToAdd, OULinkedList<DrillingRecord>* drillList); // Check if date stamp matches the inital date stamp in drill array.
bool duplicateTimeStampExists(DrillingRecord* recordToAdd, OULinkedList<DrillingRecord>* drillList); // This method check if duplicate records exist. returns -1,0,1. Details for return data can be seen at function signature.
bool isInvalidFloat(double value);

int sortOption(ResizableArray<DrillingRecord>* drillArray); // Function that is executed when the user chooses sort in the data manipulation loop.
void findOption(ResizableArray<DrillingRecord>* drillArray, int sortedColumn); // Function that is executed when the user chooses find in the data manipulation loop.

using namespace std;

int main()
{
	// Create comparator to construct linked list sorted by time stamps.
	DrillingRecordComparator* drillTimeComparator = new DrillingRecordComparator(1);
	OULinkedList<DrillingRecord>* drillList = new OULinkedList<DrillingRecord>(drillTimeComparator);
	if (drillTimeComparator == NULL) throw new ExceptionMemoryNotAvailable;
	if (drillList == NULL) throw new ExceptionMemoryNotAvailable;
	
	// [0] is number of rows read in. [1] is number of valid lines.
	int* drillInfo = new int[2];
	drillInfo[0] = 0; drillInfo[1] = 0;

	dataInputLoop(drillList, drillInfo);
	dataManipulationLoop(drillList, drillInfo);

	// Only display thank you message if Driller is used.
	if (drillList->getSize() > 0)
	{
		cout << "Thanks for using Driller.\n";
	}
	return 0;
}

// Allow the user to specify what file to include.
void dataInputLoop(OULinkedList<DrillingRecord>* drillList, int* drillInfo)
{
	// Get the name of the file directly from user.
	cout << "Enter data file name: ";
	string filename = "";
	getline(cin, filename, '\n');

	// If input is empty exit loop.
	while (!filename.empty()) 
	{
		// Open input stream to the specified file
		std::ifstream in;
		in.open(filename);

		// Check if this stream is to a valid file.
		if (in.fail())
		{
			cout << "File is not available.\n";
		}
		else
		{
			readFileStream(drillList, in, drillInfo);
			// Check if there is any valid records in the list
			if (!(drillList->getSize() > 0)) 
			{
				cout << "No valid records found.";

				// Prompt user for subsequent file.
				filename = "";
				cout << "Enter data file name: ";
				getline(cin, filename, '\n');
			}
			// If there is records, exit the loop
			else 
			{
				break; // exit loop
			}
		}

		// Prompt user for subsequent file.
		filename = "";
		cout << "Enter data file name: ";
		getline(cin, filename, '\n');
	}
}

// Returns a drilling record populated from the CSV data. nullptr signifies an invalid record.
DrillingRecord* parseRow(OULinkedList<DrillingRecord>* drillList, string lineOfData, int row)
{
	// Create drilling record to populate
	DrillingRecord* recordToReturn = new DrillingRecord();

	// Create string stream to parse data
	stringstream dataLine(lineOfData);
	string value = ""; // Value in the column

	// This loop with parse CSV data and check the validity of the data.
	// If there is an invalid record, we will return a null value.
	int column = 0;
	bool skipRow = false;
	while (getline(dataLine, value, ',')) 
	{
		// Check date stamp validity
		if (column == 0)
		{
			recordToReturn->addString(value);
			// check that the date in the file matches the list
			if (!checkDateStamp(recordToReturn, drillList))
			{
				if (false)  // When comparing to 2nd Linked List.  //TODO:::::
				{
					if (drillList->getFirst().getString(0).compare(value) != 0)
					{
						cout << "Date mismatch; file closed.\n";
						throw Exception();
					}
				}

				// Display error message and return null ptr
				cout << "Non-matching date stamp " << value << " at line " << row << ".\n";
				delete recordToReturn;
				recordToReturn = NULL;
				return nullptr;
			}
		}

		if (column == 1) 
		{
			recordToReturn->addString(value);
			if (duplicateTimeStampExists(recordToReturn, drillList)) 
			{
				cout << "Duplicate timestamp " << value << " at line " << row << ".\n";
				delete recordToReturn;
				recordToReturn = NULL;
				return nullptr;
			}
		}

		if (column > 1) 
		{
			double floatVal = stod(value);
			
			// Returns a nullptr to be handled in super method.
			if (isInvalidFloat(floatVal)) 
			{
				cout << "Invalid floating-point data at line " << row << ".\n";
				delete recordToReturn;
				recordToReturn = NULL;
				return nullptr;
			}
			// Add value to record if valid.
			recordToReturn->addNum(floatVal);
		}

		// Reset loop variables.
		column++;
		value = "";
	}

	return recordToReturn;
}

// Takes input file stream as parameter and reads the file.
void readFileStream(OULinkedList<DrillingRecord>* drillList, ifstream& in, int* lineData)
{
	// Skip first line in the file.
	string lineOfData = "";
	getline(in, lineOfData);
	lineOfData = "";

	// Track the row of the line of data being parsed.
	int row = 0;

	// Begin reading the rows of the file specified by the user.
	while (getline(in, lineOfData)) 
	{
		row++; // Increment the row # within the file.
		lineData[0]++; // Increment the total # of rows read into the file.

		try {
			// Create drilling record and check if valid.
			DrillingRecord* recordToAdd = parseRow(drillList, lineOfData, row);
			
			if (recordToAdd == nullptr) 
			{
				continue;
			}

			// Insert item into the list based on order.
			drillList->insert(*recordToAdd);
			lineData[1]++; // Increment total valid rows.
		}
		catch (Exception* exc)
		{
			delete exc;
			exc = NULL;

			// Exception is thrown when date mismatch occurs.
			in.close();
			break;
		}
	}
}

// Returns if the date stamps match to the initial date in drill record.
bool checkDateStamp(DrillingRecord* recordToAdd, OULinkedList<DrillingRecord>* drillList)
{
	// Create date stamp comparator
	DrillingRecordComparator* comparator = new DrillingRecordComparator(0);
	if (comparator == NULL) throw new ExceptionMemoryNotAvailable;

	// Check if the date stamp is similar to first record or if it is the first record.
	if (drillList->getSize() < 1)
	{
		return true;
	}
	else if (comparator->compare(*recordToAdd, drillList->getFirst()) == 0)
	{
		return true;
	}
	return false;
}

// Checks if the linked list contains a duplicate timestamp
bool duplicateTimeStampExists(DrillingRecord* recordToAdd, OULinkedList<DrillingRecord>* drillList)
{
	if (drillList->contains(*recordToAdd)) return true;
	else return false;
}

bool isInvalidFloat(double value)
{
	if (value <= 0.00) 
	{
		return true;
	}
	return false;
}

// Executes a sort on resizable array based on user input.
int sortOption(ResizableArray<DrillingRecord>* drillArray)
{
	// Prompt user for input
	cout << "Enter sort field (0-17): ";
	string userinput;
	getline(cin, userinput, '\n');

	// As long as the user input has something in it continue loop
	int sortColumn = stoi(userinput);
	if (!(sortColumn < 0) && !(sortColumn > 17))
	{
		// Create comparator and search with this data.
		DrillingRecordComparator* comparator = new DrillingRecordComparator(sortColumn);
		if (comparator == NULL) throw new ExceptionMemoryNotAvailable;

		// Call sort method.
		Sorter<DrillingRecord>::sort(*drillArray, *comparator);
	}
	return sortColumn;
}

// Executes user input if user wants to search from the records read into the resizable array strcuture.
void findOption(ResizableArray<DrillingRecord>* drillArray, int sortedColumn)
{
	//Prompt user input.
	cout << "Enter search field (0-17): ";
	string userinput;
	getline(cin, userinput, '\n');

	// begin user find operation.
	int findColumn = stoi(userinput);
	if (!(findColumn < 0) && !(findColumn > 17))
	{
		// Create comparator based on the search row specified by user.
		DrillingRecordComparator* comparator = new DrillingRecordComparator(findColumn);
		if (comparator == NULL) throw new ExceptionMemoryNotAvailable;

		// Prompt user for input.
		if (findColumn > 1)
		{
			cout << "Enter positive field value: ";
		}
		else
		{
			cout << "Enter exact text on which to search: ";
		}

		// Get user input
		string searchInput;
		getline(cin, searchInput, '\n');

		if (!searchInput.empty())
		{
			// Create record with same data to find.
			DrillingRecord* toFind = new DrillingRecord;
			if (toFind == NULL) throw new ExceptionMemoryNotAvailable;

			// Construct Dummy record to search for sharing the same column information.
			if (findColumn <= 1)
			{
				toFind->setString(searchInput, findColumn);
			}
			else
			{
				toFind->setNum(stod(searchInput), findColumn - 2);
			}

			// Create record to store the records found.
			ResizableArray<DrillingRecord>* recordsFound = new ResizableArray<DrillingRecord>();

			// Find index of the first element in cluster of duplicates using either binary search or linear search depending on if the data is sorted on the column specified.
			long long* index = new long long[1];
			if (findColumn == sortedColumn)
			{
				index[0] = binarySearch(*toFind, *drillArray, *comparator);


				// Check if record found.
				if (index[0] >= 0)
				{
					// Add initial record found.
					recordsFound->add(drillArray->get(index[0]));
					if (index[0] < (long)(drillArray->getSize() - 1L))
					{
						long nextIndex = index[0];
						// loop though records above the one found.
						while (!(nextIndex > (long)drillArray->getSize()))
						{
							try
							{
								if (comparator->compare(drillArray->get(index[0]), drillArray->get(nextIndex + 1L)) == 0)
								{
									recordsFound->add(drillArray->get(nextIndex + 1L));
								}
							}
							catch (ExceptionIndexOutOfRange* ex)
							{
								delete ex;
								ex = NULL;
							}
							nextIndex++;
						}
					}
				}
			}
			else
			{
				// Loop through records found and add them
				index = linearSearch(*toFind, *drillArray, *comparator);

				int sizeOfFound = sizeof(index);
				while (sizeOfFound >= 0)
				{
					if (index[sizeOfFound] >= 0) recordsFound->add(drillArray->get(sizeOfFound));
					sizeOfFound--;
				}
			}

			// Display string of the records found.
			long printThisIndex = 0;
			while (printThisIndex < (long)recordsFound->getSize())
			{
				cout << recordsFound->get(printThisIndex) << "\n";
				printThisIndex++;

			}

			// Display results of the search
			cout << "Drilling records found: " << recordsFound->getSize() << ".\n";

			//Delete temporary records found array
			delete recordsFound;
			recordsFound = NULL;

			// Delete drill record place holder for search.
			delete toFind;
			toFind = NULL;
		}
	}
}

//Method that allows the user to either output data to a specified location, sort by different keys, or find a records.
void dataManipulationLoop(OULinkedList<DrillingRecord>* drillList, int* fileData)
{
	int sortedColumn = -1;

	ResizableArray<DrillingRecord>* drillArray = new ResizableArray<DrillingRecord>;
	if (drillArray == NULL) throw new ExceptionMemoryNotAvailable();

	//TODO:: Copy list into array

	// Prompt user and get initial input
	cout << "Enter (o)utput, (s)ort, (f)ind, or (q)uit: \n";
	string userinput;
	getline(cin, userinput, '\n');

	// Check what character the user inputs
	while (userinput.at(0) != 'q')
	{
		// Execute branch case that corresponds to the input.
		switch (userinput.at(0))
		{
		case 'o':
			// Execute drillerOuput and display data then terminate loop.
			drillerOutput(drillList, fileData);
			break;
		case 's':
			sortedColumn = sortOption(drillArray);
			break;
		case 'f':
			findOption(drillArray, sortedColumn);
			break;
		case 'm':
			// Create new list that contains the records to be read in.
			DrillingRecordComparator* timeStampComparator = new DrillingRecordComparator(1);
			if (timeStampComparator == NULL) throw new ExceptionMemoryNotAvailable;
			OULinkedList<DrillingRecord>* drillListToMerge = new OULinkedList<DrillingRecord>(timeStampComparator);
			if (timeStampComparator == NULL) throw new ExceptionMemoryNotAvailable;

			// Get file input from the user.
			dataInputLoop(drillListToMerge, fileData);

			// Merges the two lists. Make sure original list is first argument.
			mergeLists(drillList, drillListToMerge);
			break;
		case 'p':
			break;
		case 'r':
			break;
		}
		// Prompt and get input for next data manipulation loop
		cout << "Enter (o)utput, (s)ort, (f)ind, (m)erge, (p)urge, (r)ecords, or (q)uit: \n";
		userinput = "";
		getline(cin, userinput, '\n');
	}
}

//Ouput to location specified by user.
void drillerOutput(OULinkedList<DrillingRecord>* drillList, int* fileData)
{
	// Get initial user input about where to output data.
	cout << "Enter output file name: ";
	string userinput;
	getline(cin, userinput, '\n');

	// Decalre stream
	ofstream myFile;

	// File input loop.
	bool continueInput = true;
	while (continueInput)
	{
		// If input is empty continue to std output
		if (userinput.empty())
		{
			continueInput = false;

			//TODO:::::::
			//Out put records in memory
		/*	for (unsigned long index = 0; index < drillList->getSize(); index++)
			{
				cout << drillArray->get(index) << "\n";
			}*/

			// Output record statistics from getFilesFromUser.
			cout << "Data lines read: " << fileData[0] << "; ";
			cout << "Valid drilling records read: " << fileData[1] << "; ";
			cout << "Drilling records in memory: " << drillList->getSize() << "\n";
		}
		// If input is a file name check that it exists and contiue with output
		else
		{
			// Check if file exists.
			myFile.open(userinput);
			if (myFile.fail())
			{
				// Display fail message
				cout << "File is not available.\n";
				cout << "Enter output file name: ";
				getline(cin, userinput, '\n');
			}
			else
			{
				// File exists so continue output to file.
				continueInput = false;

			/*	// TODO:::::
				for (unsigned long index = 0; index < drillList->getSize(); index++)
				{
					myFile << drillList->get(index) << "\n";
				}*/

				// Display record statistics from file read.
				myFile << "Data lines read: " << fileData[0] << "; ";
				myFile << "Valid drilling records read: " << fileData[1] << "; ";
				myFile << "Drilling records in memory: " << drillList->getSize() << "\n";

				// Closing resource.
				myFile.close();
			}
		}
	}
}

/*
1. drillerOutput still uses resizable array.
2. figure out where to use linked list
3. finish merge
4. finish purge
5. finish records
*/
template <typename T>
void mergeLists(OULinkedList<T>* originalList, OULinkedList<T>* newList)
{
	// Create enumerator to loop through.
	OULinkedListEnumerator<T> iterator = newList->enumerator();
	while (iterator.hasNext()) 
	{
		T newRecord = iterator.next();

		// Attempt to insert new record. If insert returns false, replace.
		if (!originalList->insert(newRecord)) 
		{
			originalList->replace(newRecord);
		}
	}
}
