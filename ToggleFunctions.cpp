// ToggleFunctions.cpp

#include <fstream>
#include <limits>
#include <string>
#undef max

using namespace std;

// public file variables
//---------------------------------
fstream file;
string fileloc = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\MixedRealityVRDriver\\resources\\settings\\test.txt";

int seek1 = 0, seek2 = 0, seek3 = 0; // seek location initialization
//---------------------------------

// forward function declarations
//---------------------------------
short int toggle(); // driver function

void getSeek(short int& f); // finds seek locations

void getAnswer(short int f); // writes to file
//---------------------------------

short int toggle() 
{
	file.open(fileloc, ios::in | ios::binary); // must be read in in binary mode due to how
											   // Windows treats files opened in fstream, especially in
	if (file.is_open())                        // conjunction with the getline function
	{
		short int tf = 0;
		getSeek(tf);

		if (seek3 != 0) // checks if getSeek() reached all three checkpoints
		{
			file.close();
//			getAnswer(tf);
			return tf;
		}
		else
		{
			file.close();
			return 2;
//			cout << "There was an error reading your settings file. Please check its contents and try again.";
		}
	}

	else
		return 1;
	//	cout << "ERROR: File could not be opened.";
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

void getAnswer(short int f)
{
	file.open(fileloc);

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

	else
	{
		// handle error here
	}

	file.close();
}
