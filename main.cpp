#include <iostream>
#include <fstream>
#include <limits>
#include <string>

using namespace std;

fstream file;

// seek location initialization
int seek1 = 0, seek2 = 0, seek3 = 0;

// function prototypes
void getSeek();
void getAnswer();

int main()
{

    file.open("C:\\Program Files (x86)\\Steam\\steamapps\\common\\MixedRealityVRDriver\\resources\\settings\\test.txt", ios::in | ios::out | ios::binary);

    if (file.is_open())
    {
        cout << "File is open." << endl;
        getSeek();
        cout << seek1 << " " << seek2 << " " << seek3 << endl; // TEST LINE

        if ((seek1 != 0) && (seek2 != 0) && (seek3 !=0)) // checks if getSeek() reached all three checkpoints
        {
            file.close();
            file.open("C:\\Program Files (x86)\\Steam\\steamapps\\common\\MixedRealityVRDriver\\resources\\settings\\test.txt");
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
    int tempw, tempt, offset; // tempw/tempt holds the in-line location of key terms, offset calculates seek location
    bool atFirst = true, atSec = false; // determines which of the three seek values is currently being found

    while (getline(file, line))
    {
        // searches line for a key term, places result in tempw
        if (atFirst)
            tempw = line.find("thumbstickControls");
        else
            tempw = line.find("thumbstickTurn");


        if (tempw != string::npos)  // flags lines related to thumbstickControls or thumbstickTurn
        {
            cout << "Reached thumbstick" << endl; // TEST LINE

            // checks if flagged line is relevant
            tempw = line.find("false,");
            tempt = line.find("true,");

            if ((tempw != string::npos)||(tempt != string::npos))  // if the line contains "false," or "true,"
            {
                cout << "Reached true/false" << endl; // TEST LINE

                if (tempt != string::npos) // transfers location to proper variable in case of "true,"
                    tempw = tempt;

                offset = file.tellg() - line.length() - 1; // offset calculation

                if ((atFirst)&&(!atSec))    // Determines where we are in the process and
                {                           // puts the address in the corresponding variable.
                    seek1 = tempw + offset;
                    atSec = true;
                }
                else if (atFirst)
                {
                    seek2 = tempw + offset;
                    atFirst = false;
                }
                else
                    seek3 = tempw + offset; // to do: prevent third variable from being re-altered
            }
        }
    }

}

void getAnswer()
{
    char ans;
    cout << "Set thumbstick controls to true or false? (T/F): ";
    ans = getchar();

    if ((ans=='t')||(ans=='T'))
    {
        file.seekp(seek1);
        file << "true, ";

        file.seekp(seek2);
        file << "true, ";

        file.seekp(seek3);
        file << "true, ";

        cout << "Action completed." << endl;

    }

    else if ((ans=='f')||(ans=='F'))
    {
        file.seekp(seek1);
        file << "false, ";

        file.seekp(seek2);
        file << "false, ";

        file.seekp(seek3);
        file << "false, ";

        cout << "Action completed." << endl;
    }

    else
    {
        cout << "Invalid answer, please try again." << endl;
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        getAnswer();
    }

}
