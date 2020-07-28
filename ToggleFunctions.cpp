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
void	  check(string, short int(&)[4]);	    // driver function
void	  getSeek(short int(&)[4]);			    // finds seek locations
void	  getAnswer(string, bool*);			    // writes to file
void	  getFileloc(string&, short int(&)[4]); // initializes fileloc
void	  setFileloc(string);				    // makes current filepath the default
//---------------------------------

void check(string fl, short int(&f)[4])
{
	f[0] = -1;
	seek1 = seek2 = seek3 = 0;

	file.open(fl, ios::in | ios::binary);	   // must be read in in binary mode due to how
											   // Windows treats files opened in fstream, especially in
	if (file.is_open())                        // conjunction with the getline function
	{
		getSeek(f);

		if (seek3 != 0) // checks if getSeek() reached all three checkpoints
		{
			file.close();
			f[0] = 0;
			return;
		}
		else
		{
			file.close();
			f[0] = 2;
			return;
		}
	}

	else
	{
		f[0] = 1;
		return;
	}
}

void getSeek(short int(&f)[4])
{
	string line;
	int tempw, tempt, offset = 0, loc = 0; // used to find location of key terms to calculate seek location

	while (getline(file, line))
	{
		// flags lines related to thumbstickControls or thumbstickTurn
		if (seek2 == 0)
			tempw = line.find("thumbstickControls");
		else
			tempw = line.find("thumbstickTurn");

		if (tempw != string::npos) // found flagged line
		{
			// TODO: verify whitespace integrity in case of "true,//"

			// checks if flagged line is relevant
			tempw = line.find("false,");
			tempt = line.find("true,");

			if ((tempw != string::npos) || (tempt != string::npos)) // found relevant line
			{
				// offset calculation
				loc = file.tellg();				  // the file read position is currently at the end of the line + 1
				offset = loc - line.length() - 1; // due to '\n' being discarded, so length + 1 must be subtracted from read pos

				// Determines where we are in the process and
				// puts the address in the corresponding variable.
				if (seek1 == 0)
				{
					if (tempw != string::npos)
					{
						f[1] = 1;
						seek1 = tempw + offset;
					}
					else
					{
						f[1] = 2;
						seek1 = tempt + offset;
					}
				}
				else if (seek2 == 0)
				{
					if (tempw != string::npos)
					{
						f[2] = 1;
						seek2 = tempw + offset;
					}
					else
					{
						f[2] = 2;
						seek2 = tempt + offset;
					}
				}
				else
				{
					if (tempw != string::npos)
					{
						f[3] = 1;
						seek3 = tempw + offset;
						return;
					}
					else
					{
						f[3] = 2;
						seek3 = tempt + offset;
						return;
					}
				}
			}
		}
	}
}

void getAnswer(string fl, bool* ck)
{
	file.open(fl);

	file.seekp(seek1);
	if (ck[0])
		file << "true, ";
	else
		file << "false, ";

	file.seekp(seek2);
	if (ck[1])
		file << "true, ";
	else
		file << "false, ";

	file.seekp(seek3);
	if (ck[2])
		file << "true, ";
	else
		file << "false, ";

	file.close();
}

void getFileloc(string& fl, short int (&f)[4])
{
	file.open("config.txt");

	if (file.is_open())
	{
		getline(file, fl);

		string tempstr[3];

		for (int i = 0; i <= 2; i++)
		{
			if (getline(file, tempstr[i]))
				f[i + 1] = stoi(tempstr[i]);
		}

		file.close();
	}
}

void setFileloc(string fl)
{
	int i = 0; // iterator

	file.open("config.txt", ios::in);

	if (file.is_open())
	{
		string tempchk[3];
		string tempfl; 
		if (getline(file, tempfl)) // TODO - swap for method that doesn't need tempfl
		{
			for (i = 0; i <= 2; i++)
				getline(file, tempchk[i]);
		}
		file.close();

		file.open("config.txt", ios::out);

		file << fl << "\n";

		if (!tempchk[2].empty())
		{
			for (i = 0; i <= 2; i++)
				file << tempchk[i] << "\n";
		}
	}
	else
	{
		file.clear();
		file.open("config.txt", ios::out);

		file << fl << "\n";
		
		for (i = 0; i <= 2; i++)
			file << "1\n";
	}
	file.close();
}