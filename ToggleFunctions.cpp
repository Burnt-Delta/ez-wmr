// ToggleFunctions.cpp: Provides functions to provide file manipulation.
// Author: Burnt-Delta

#include <fstream>
#include <limits>
#include <string>
#undef max

using namespace std;

// forward function declarations
//---------------------------------
void	  check(string, short int(&)[4], int(&)[3]);	 // driver function
void	  getSeek(short int(&)[4], int(&)[3], fstream&); // finds seek locations
void	  getAnswer(string, bool*, int[3]);				 // writes to file
void	  getFileloc(string&, short int(&)[4]);			 // initializes fileloc
void	  setFileloc(string);							 // makes current filepath the default
void	  setCheckbox(bool*);							 // makes current checkbox selection the default 
//---------------------------------

void check(string fl, short int(&f)[4], int(&seek)[3])
{
	fstream file;
	f[0] = -1;

	file.open(fl, ios::in | ios::binary);	   // must be read in in binary mode due to how
											   // Windows treats files opened in fstream, especially in
	if (file.is_open())                        // conjunction with the getline function
	{
		getSeek(f, seek, file);

		if (seek[2] != 0) // checks if getSeek() reached all three checkpoints
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

void getSeek(short int(&f)[4], int(&seek)[3], fstream& file)
{
	string line;
	int tempw, tempt, offset = 0, loc = 0; // used to find location of key terms to calculate seek location

	while (getline(file, line))
	{
		// flags lines related to thumbstickControls or thumbstickTurn
		if (seek[1] == 0)
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
				if (seek[0] == 0)
				{
					if (tempw != string::npos)
					{
						f[1] = 1;
						seek[0] = tempw + offset;
					}
					else
					{
						f[1] = 2;
						seek[0] = tempt + offset;
					}
				}
				else if (seek[1] == 0)
				{
					if (tempw != string::npos)
					{
						f[2] = 1;
						seek[1] = tempw + offset;
					}
					else
					{
						f[2] = 2;
						seek[1] = tempt + offset;
					}
				}
				else
				{
					if (tempw != string::npos)
					{
						f[3] = 1;
						seek[2] = tempw + offset;
						return;
					}
					else
					{
						f[3] = 2;
						seek[2] = tempt + offset;
						return;
					}
				}
			}
		}
	}
}

void getAnswer(string fl, bool* ck, int seek[3])
{
	fstream file;
	file.open(fl);

	file.seekp(seek[0]);
	if (ck[0])
		file << "true, ";
	else
		file << "false, ";

	file.seekp(seek[1]);
	if (ck[1])
		file << "true, ";
	else
		file << "false, ";

	file.seekp(seek[2]);
	if (ck[2])
		file << "true, ";
	else
		file << "false, ";

	file.close();
}

void getFileloc(string& fl, short int (&f)[4])
{
	fstream file;
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

	fstream file;
	file.open("config.txt", ios::in);

	if (file.is_open())
	{
		string tempchk[3];
		string tempfl; 
		if (getline(file, tempfl)) // TODO: swap for method that doesn't need tempfl
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

void setCheckbox(bool* ck)
{
	fstream file;
	file.open("config.txt", ios::in);

	if (file.is_open())
	{
		string tempfl;
		getline(file, tempfl);

		file.close();

		file.open("config.txt", ios::out);
		file << tempfl << "\n";
	}
	else
	{
		file.clear();
		file.open("config.txt", ios::out);
		file << "\n";
	}

	for (int i = 0; i <= 2; i++)
	{
		if (ck[i])
			file << "2\n";
		else
			file << "1\n";
	}

	file.close();
}
