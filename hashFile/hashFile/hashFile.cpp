/****************************************************************************************************************************
* Author:		Jonathan Casper																								*
*																															*
* Program Name:	pinsAndKeys																									*
*																															*
* Description:	This program takes two input files: a hash file that contains pin/key pairs where a pin will be the value	*
*				used to calculate a hash value, and a pin file that contains a list of pins to locate in the hash file. It	*
*				will then create and output file report listing the pins with their corresponding keys, along with the		*
*				number of access it took to find the record.																*
*																															*
****************************************************************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

const int PINSIZE = 3;
const int KEYSIZE = 5;

struct HashRecord
{
	char Pin[PINSIZE];
	char Key[KEYSIZE];
	int Top;
};

void main()
{
	fstream pinsAndKeys("pinsAndKeys.dat", ios::in | ios::binary);
	fstream pins("pins.txt", ios::in);

	if (!pinsAndKeys.is_open())
	{
		cout << "Couldn't open 'pinsAndKeys.dat'... Closing program" << endl;
	}
	else if (!pins.is_open())
	{
		cout << "Couldn't open 'pins.txt'... Closing program" << endl;
	}
	else
	{
		// program
		fstream fout("hashReport.txt", ios::out);
		string pin;
		vector<HashRecord>records;
		double averageCount = 0.00;

		int recordSize, primeAddresses, nextRRN, pinCount = 0;
		
		pinsAndKeys.read((char*)&recordSize, sizeof(recordSize));
		pinsAndKeys.read((char*)&primeAddresses, sizeof(primeAddresses));
		pinsAndKeys.read((char*)&nextRRN, sizeof(nextRRN));

		fout << "PIN   " << "KEY       " << "# OF ACCESSES" << endl << endl;
		while (!pins.eof())
		{
			pins >> pin;
			pinCount++;

			string key = "*****";
			int accessCnt = 0;

			int hash = (pin[0] * pin[1] * pin[2]);
			hash %= primeAddresses;
			bool foundRecord = false;

			

			pinsAndKeys.seekg((12 + hash * recordSize));
			while (foundRecord == false)
			{

				HashRecord record;
				pinsAndKeys.read((char*)&record, sizeof(HashRecord));
				accessCnt++;
				string recordPin = record.Pin;
				recordPin = recordPin.substr(0, PINSIZE);

				if (recordPin == pin)
				{
					key = record.Key;
					foundRecord = true;
				}
				else if (record.Pin[0] == '*')
				{
					foundRecord = true;
				}
				else if (record.Top == -1)
				{
					foundRecord = true;
				}
				else
				{
					pinsAndKeys.seekg((12 + primeAddresses * recordSize + record.Top * recordSize));
				}
			}		
						
			averageCount += accessCnt;
			key = key.substr(0, KEYSIZE);
			fout << pin << "   " << key << "           " << accessCnt << endl;
		}
		fout << endl;
		averageCount = averageCount / pinCount;
		fout << "Average access to locate a pin or determine pin does not exist: " << fixed << setprecision(2) <<  averageCount;

		fout.close();
	}

	pins.close();
	pinsAndKeys.close();
}