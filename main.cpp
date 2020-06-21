#include <iostream>
#include <fstream>
#include <limits>
#include <string>

using namespace std;

// public file variables
fstream file;
string fileloc = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\MixedRealityVRDriver\\resources\\settings\\default.vrsettings";

// seek location initialization
int seek1 = 0, seek2 = 0, seek3 = 0;

// function prototypes
void getSeek();
void getAnswer();

int main()
{
	file.open(fileloc, ios::in | ios::binary); // must be read in in binary mode due to how
											   // Windows treats files opened in fstream, especially in
	if (file.is_open())                        // conjunction with the getline function
	{
		getSeek();

		if (seek3 != 0) // checks if getSeek() reached all three checkpoints
		{
			file.close();
			file.open(fileloc); // re-opens file to properly write
			getAnswer();
		}
		else
			cout << "There was an error reading your settings file. Please check its contents and try again.";

		file.close();
	}

	else
		cout << "ERROR: File could not be opened.";

	return 0;
}

void getSeek()
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

			if ((tempw != string::npos) || (tempt != string::npos))
			{
				if (tempt != string::npos) // transfers location to proper variable in case of "true,"
					tempw = tempt;

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

void getAnswer()
{
	char ans;
	bool done = false;

	while (!done)
	{
		cout << "Set thumbstick controls to true or false? (T/F): ";
		ans = getchar();

		if ((ans == 't') || (ans == 'T'))
		{
			file.seekp(seek1);
			file << "true, ";

			file.seekp(seek2);
			file << "true, ";

			file.seekp(seek3);
			file << "true, ";

			cout << "Action completed." << endl;
			done = true;
		}

		else if ((ans == 'f') || (ans == 'F'))
		{
			file.seekp(seek1);
			file << "false, ";

			file.seekp(seek2);
			file << "false, ";

			file.seekp(seek3);
			file << "false, ";

			cout << "Action completed." << endl;
			done = true;
		}

		else
		{
			cout << "Invalid answer, please try again." << endl;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}
