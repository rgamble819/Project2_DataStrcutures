#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include"DrillingRecord.h"
#include"Search.h"
#include "DrillingRecordComparator.h"
#include"Sorter.h"

using namespace std;

void dataManipulationLoop(ResizableArray<DrillingRecord>* drillArray, int* fileData); // Enter user data manipulation loop. Ability to chose output, sort, or find. returns void.
void drillerOutput(ResizableArray<DrillingRecord>* drillArray, int* fileData); // Output data to specified location

int* getFilesFromUser(ResizableArray<DrillingRecord>* drillArray); // Reads in files specified by user. Returns integer array of size 2 - Element 0 is the number of rows read in and Element 1 is the number of valid lines.

bool checkDateStamp(DrillingRecord* recordToAdd, ResizableArray<DrillingRecord>* drillArray); // Check if date stamp matches the inital date stamp in drill array.

int checkTimeStamp(DrillingRecord* recordToAdd, ResizableArray<DrillingRecord>* drillArray, long originalSize); // This method check if duplicate records exist. returns -1,0,1. Details for return data can be seen at function signature.

int sortOption(ResizableArray<DrillingRecord>* drillArray); // Function that is executed when the user chooses sort in the data manipulation loop.
void findOption(ResizableArray<DrillingRecord>* drillArray, int sortedColumn); // Function that is executed when the user chooses find in the data manipulation loop.

int main()
{
	ResizableArray<DrillingRecord>* drillArray = new ResizableArray<DrillingRecord>(); // Create drilling array to store DrillingRecords.
	if (drillArray == NULL) throw new ExceptionMemoryNotAvailable;

	// [0] is lines read [1] is valid records. I am simply returning the values calculated from reading the files for use in output.
	int* recordData = getFilesFromUser(drillArray); // Read data from files specified by user.

	// Sort the record by default by timestamps 
	DrillingRecordComparator* comparator = new DrillingRecordComparator(1);
	if (comparator == NULL) throw new ExceptionMemoryNotAvailable;

	// TODO: Examine if i should sort while inserting or after.
	Sorter<DrillingRecord>::sort(*drillArray, *comparator);

	// Delete comparator
	//delete comparator;
	//comparator = NULL;

	// If there is DrillingRecords enter data manipulation loop.
	if (drillArray->getSize() > 0)
	{
		dataManipulationLoop(drillArray, recordData);
	}

	// Only display thank you message if Driller is used.
	if (drillArray->getSize() > 0)
	{
		cout << "Thanks for using Driller.\n";
	}
	return 0;
}

/*
// Read in selected files from the user. 
// Returns integer array of size 2. [0] is rowsRead and [1] is valid lines.
int* getFilesFromUser(ResizableArray<DrillingRecord>* drillArray)
{
	// Keep track of valid lines & lines read
	int valid = 0;
	int rowsRead = 0;
	long originalSize = drillArray->getSize();

	// Idex to remove in the event of a duplicate time stamp in subsequent files.
	int indexToRemove = -1;

	//get first filename
	cout << "Enter data file name: ";
	string filename = "";
	getline(cin, filename, '\n');

	// Track if the files input is the first file or not
	bool firstFile = true;

	// Begin filename userinput loop as long as there is an input.
	while (!filename.empty())
	{

		// Open stream to the file inputed
		ifstream in;
		in.open(filename);

		// Check if the file is valid and read data if true.
		if (in.fail())
		{
			cout << "File is not available.\n";
		}
		else
		{
			// Skip first line in file.
			string line = "";
			getline(in, line);
			line = "";

			// Track data of rows read in.
			int row = 0;

			// Begin reading the rows of the file specified.
			while (getline(in, line))
			{
				rowsRead++;
				row++; // Increment Row.

				//Create dataRecord for each DrillingRecord
				DrillingRecord* recordToAdd = new DrillingRecord;
				if (recordToAdd == NULL) throw new ExceptionMemoryNotAvailable;

				//Open String stream
				stringstream dataLine(line);
				string value = ""; // Value in column

				// This loop seperates the CSV values and check validity.
				// If the line is not valid, it will change the skipRow value to true and not add it to the Drilling Record.

				// We will be be adding data to the record as the data values are read in. We will check these records being constructed with the records in memory
				// If there is an invalid record, we will delete the record when the skipRow value switches to true.
				int column = 0;
				bool skipRow = false;
				/*
				cout << "\n";
				int ind = drillArray->getSize() - 1;
				while (ind >= 0)
				{
					cout << drillArray->get(ind) << "\n";
					ind--;
				}
				cout << "\n";//////////////////////

				while (getline(dataLine, value, ','))
				{
					// Check if the two files have matching date stamps.
					if (column == 0)
					{
						recordToAdd->addString(value);
						// Check that the date in the file matches the others
						// Also check that the first date in the subsequent files matches the first.
						if (!checkDateStamp(recordToAdd, drillArray))
						{
							// If it is the first file check that the time stamp matches the first file
							if (!firstFile)
							{
								if (drillArray->get(0).getString(0).compare(value) != 0)
								{
									// Clear the streams so that the loops terminate
									cout << "Date mismatch; file closed.\n";
									value = "";
									dataLine.str("");
									in.close();
									skipRow = true;
									break;
								}
							}

							// If it is the first file show 
							cout << "Non-matching date stamp " << value << " at line " << row << ".\n";
							skipRow = true;
							value = "";
							break;
						}
					}

					// If it is column associated with float values, check that it is valid
					if (column > 1)
					{
						double fVal = stod(value);

						// Check if valid float data
						if (fVal <= 0.00)
						{
							cout << "Invalid floating-point data at line " << row << ".\n";
							value = "";
							skipRow = true;
							break;
						}
						// Add the valid float to the record.
						recordToAdd->addNum(fVal);
					}

					if (column == 1)
					{
						recordToAdd->addString(value);
						int checkCondition = checkTimeStamp(recordToAdd, drillArray, originalSize);
						if (checkCondition == -1)
						{
							cout << "Duplicate timestamp " << value << " at line " << row << ".\n";
							skipRow = true;
							value = "";
							break;
						}
						else if (checkCondition >= 0)
						{
							//originalSize--;
							indexToRemove = checkCondition;
						}
					}

					column++; // increment column within row
					value = ""; // reset value for next value to read in
				}

				// Add or delete Record/Line
				if (skipRow != true)
				{
					if (indexToRemove >= 0) {
						drillArray->removeAt(indexToRemove);
						indexToRemove = -1;
					}
					drillArray->add(*recordToAdd);
					// Increment the amount of valid records added the the array.
					valid++;
				}
				else
				{
					// If skipRow is true delete record in memory.
					delete recordToAdd;
					recordToAdd = NULL;
				}
				// reset boolean value once row Record has been added/deleted.
				skipRow = false;
			}
			// if a file is read in this statement will be checked.
			// If the first file is true. Which means the first file just got read, set first file to false so the 
			// check for date mismatch of the subsequent files is triggered.
			if (firstFile)
			{
				firstFile = false;
			}
			// Close the input file stream once complete.
			in.close();
		}
		// Reset original size for the next file to read in.
		originalSize = drillArray->getSize();

		// Prompt user for subsequent file.
		filename = "";
		cout << "Enter data file name: ";
		getline(cin, filename, '\n');
	}

	// Create integer array that will be returned and fill in values according to return type specified in function declaration.
	int* trackedValues = new int[2];
	trackedValues[0] = rowsRead;
	trackedValues[1] = valid;
	return trackedValues;
}
*/

// Returns if the date stamps match to the initial date in drill record.
bool checkDateStamp(DrillingRecord* recordToAdd, ResizableArray<DrillingRecord>* drillArray)
{
	// Create date stamp comparator
	DrillingRecordComparator* comparator = new DrillingRecordComparator(0);
	if (comparator == NULL) throw new ExceptionMemoryNotAvailable;

	// Check if the date stamp is similar to first record or if it is the first record.
	if (drillArray->getSize() < 1)
	{
		return true;
	}
	else if (comparator->compare(*recordToAdd, drillArray->get(0)) == 0)
	{
		return true;
	}
	return false;
}

// This method check if duplicate records exist. returns -1,0,1. 
// '-1' means that a duplicate was found in the file being read in. 
// 'greter than 0' means a duplicate was found in the old data that needs to be replaced.
// '-2' means that no duplicate was found and record can be added.
int checkTimeStamp(DrillingRecord* recordToAdd, ResizableArray<DrillingRecord>* drillArray, long originalSize)
{
	// Create time stamp comparator
	DrillingRecordComparator* comparator = new DrillingRecordComparator(1);
	if (comparator == NULL) throw new ExceptionIndexOutOfRange;

	long long* indexFound = linearSearch(*recordToAdd, *drillArray, *comparator);
	// This means no duplicate was found at all.
	if (indexFound[0] < 0)
	{
		return -2;
	}
	// This means a duplicate was found in the old data AKA replace the old data
	else if ((long)indexFound[0] <= originalSize - 1L)
	{
		return (int)indexFound[0];
	}
	// This means a duplicate was found in the new data AKA toss out record
	else
	{
		return -1;
	}
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
void dataManipulationLoop(ResizableArray<DrillingRecord>* drillArray, int* fileData)
{
	int sortedColumn = -1;

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
			drillerOutput(drillArray, fileData);

			// Prompt and get input for next data manipulation loop
			cout << "Enter (o)utput, (s)ort, (f)ind, or (q)uit: \n";
			userinput = "";
			getline(cin, userinput, '\n');
			break;
		case 's':
			sortedColumn = sortOption(drillArray);
			// Prompt and get input for next data manipulation loop
			cout << "Enter (o)utput, (s)ort, (f)ind, or (q)uit: \n";
			userinput = "";
			getline(cin, userinput, '\n');
			break;
		case 'f':
			findOption(drillArray, sortedColumn);
			// Prompt and get input for next data manipulation loop
			cout << "Enter (o)utput, (s)ort, (f)ind, or (q)uit: \n";
			userinput = "";
			getline(cin, userinput, '\n');
			break;
		}
	}
}

//Ouput to location specified by user.
void drillerOutput(ResizableArray<DrillingRecord>* drillArray, int* fileData)
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

			//Out put records in memory
			for (unsigned long index = 0; index < drillArray->getSize(); index++)
			{
				cout << drillArray->get(index) << "\n";
			}

			// Output record statistics from getFilesFromUser.
			cout << "Data lines read: " << fileData[0] << "; ";
			cout << "Valid drilling records read: " << fileData[1] << "; ";
			cout << "Drilling records in memory: " << drillArray->getSize() << "\n";
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

				for (unsigned long index = 0; index < drillArray->getSize(); index++)
				{
					myFile << drillArray->get(index) << "\n";
				}

				// Display record statistics from file read.
				myFile << "Data lines read: " << fileData[0] << "; ";
				myFile << "Valid drilling records read: " << fileData[1] << "; ";
				myFile << "Drilling records in memory: " << drillArray->getSize() << "\n";

				// Closing resource.
				myFile.close();
			}
		}
	}
}

// TODO: 
// 1) binary search returns first in cluster. --- completed
// 2) linear search returns first in cluster. 
// 3) Track if column is sorted in search/find.
// 4) if sorted by correct column, search with binary else search with linear.
// 5) track down duplicate time stamp error.