// ToggleFunctions.cpp: Provides functions to provide file manipulation.
// Author: Burnt-Delta

#include <fstream>
#include <limits>
#include <string>
#undef max

using namespace std;

// public file variables
//---------------------------------
fstream file;

int seek1 = 0, seek2 = 0, seek3 = 0; // seek location initialization
//---------------------------------

// forward function declarations
//---------------------------------
short int toggle(string);				// driver function
void	  getSeek(short int&);			// finds seek locations
void	  getAnswer(short int, string); // writes to file
void	  getFileloc(string&);			// initializes fileloc
void	  setFileloc(string);			// makes current filepath the default
//---------------------------------

short int toggle(string fl) 
{
	file.open(fl, ios::in | ios::binary);	   // must be read in in binary mode due to how
											   // Windows treats files opened in fstream, especially in
	if (file.is_open())                        // conjunction with the getline function
	{
		short int tf = 0;
		getSeek(tf);

		if (seek3 != 0) // checks if getSeek() reached all three checkpoints
		{
			file.close();
			return tf;
		}
		else
		{
			file.close();
			return 2;
		}
	}

	else
		return 1;
}

void getSeek(short int& f)
{
	string line;
	int tempw, tempt, offset, loc = 0; // used to find location of key terms to calculate seek location

	while (getline(file, line))
	{
		// searches line for a key term, places result in tempw
		if (seek2 == 0)
			tempw = line.find("thumbstickControls");
		else
			tempw = line.find("thumbstickTurn");

		// flags lines related to thumbstickControls or thumbstickTurn
		if (tempw != string::npos)
		{
			// TODO: verify whitespace integrity in case of "true,//"

			// checks if flagged line is relevant
			tempw = line.find("false,");
			tempt = line.find("true,");

			// In case of "true," transfers location to tempw for later use.
			// Also sets tf flag.
			if ((tempw != string::npos) || (tempt != string::npos))
			{
				if (tempt != string::npos) 
				{
					f = 3;
					tempw = tempt;
				}
				else
					f = 4;

				// offset calculation
				loc = file.tellg();				   // the file read position is currently at the end of the line + 1
				offset = loc - line.length() - 1;  // due to '\n' being discarded, so length + 1 must be subtracted from read pos

				if (seek1 == 0)				// Determines where we are in the process and
					seek1 = tempw + offset; // puts the address in the corresponding variable.
				else if (seek2 == 0)
					seek2 = tempw + offset;
				else
				{
					seek3 = tempw + offset;
					return;
				}
			}
		}
	}
}

void getAnswer(short int f, string fl)
{
	file.open(fl);

	if (f == 4)
	{
		file.seekp(seek1);
		file << "true, ";

		file.seekp(seek2);
		file << "true, ";

		file.seekp(seek3);
		file << "true, ";
	}

	else if (f == 3)
	{
		file.seekp(seek1);
		file << "false, ";

		file.seekp(seek2);
		file << "false, ";

		file.seekp(seek3);
		file << "false, ";
	}

	file.close();
}

void getFileloc(string& fl)
{
	file.open("config.txt");

	if (file.is_open())
	{
		getline(file, fl);
		file.close();
	}
}

void setFileloc(string fl)
{
	file.open("config.txt", ios::out);
	file << fl << "\n";
	file.close();
}