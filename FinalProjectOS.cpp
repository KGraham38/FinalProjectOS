//Kody Graham
//04/05/2026
//OS Final Project - CSCI 4300
//I have decided to do FIFO to really be able to compare how much better optimal is compared to the worst case scenerio 

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

int main()
{
    //Page replacement FIFO
    
    //Step 1: Read/parse text from file; Argument1 (first letter either F, O, or L) = replacement algorithm, Argument2 = number of memory frames (second char in file must be an int), and Argument3= the reference string(comma seperated)
    string fileName;
    string input;
    ifstream inputFile;

    cout << "Enter the input file name: ";
    cin >> fileName;

    //Open the file
    inputFile.open(fileName);

    //Make sure file is open
    if (!inputFile)
    {
        cout << "ERROR: Could not open the file " << fileName << endl;
        return 1;
    }

    //Read the first line of the file and save in input
    getline(inputFile, input);

    //Close the file
    inputFile.close();

    //Store the arguments
    string argumentsArray[1000];
    int argCount = 0;
    string currentArg = "";

    //Split the input based on the commas
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == ',')
        {
            argumentsArray[argCount] = currentArg;
            argCount++;
            currentArg = "";
        }
        else
        {
            currentArg += input[i];
        }
    }
        
     //Save last arg
     if (currentArg != "")
     {
         argumentsArray[argCount] = currentArg;
         argCount++;
     }

     //Make sure all 3 required args are present
     if (argCount<3)
     {
         cout << "Error: To few arguments.\n";
         cout << "Required File Format : Algorithm letter, Number of Frames, and the Reference String\n";
         return 1;
     }

     //First arg = algorithm letter
     char algorithm = argumentsArray[0][0];
     
     //Second arg = num of memory frames
     int numFrames = stoi(argumentsArray[1]);

     //Everything else = reference string
     int referenceString[1000];
     int numOfReferences = 0;

     //Starting at 2 because first 2 are the other arguments
     for (int i = 2; i < argCount; i++)
     {
         referenceString[numOfReferences] = stoi(argumentsArray[i]);
         numOfReferences++;
     }

     //Basic Validation
     if (algorithm == 'F' || algorithm == 'f')
     {
        cout << "This file is marked to use the FIFO Algorithm\n";
     }
     else if (algorithm == 'O' || algorithm == 'o')
     {
         cout << "This file is marked to use the OPT Algorithm\n";
     }
     else
     {
         cout << "Error: Argument for Algorithm Name is not valid.\n";
         cout << "This program can only be used for the FIFO or Optimal algorithms!\n";

         return 1;
     }

     if (numFrames <= 0)
     {
         cout << "Error: Number of frames must be greater than 0.\n";
         return 1;
     }

     if (numOfReferences == 0)
     {
         cout << "Error: Reference string not found.\n";
         return 1;
     }

     //Test section to confirm input
     cout << endl;
     cout << "Algorithm: " << algorithm << endl;
     cout << "Number of Frames: " << numFrames << endl;
     cout << "Reference String: ";

     for (int i = 0; i < numOfReferences; i++)
     {
         cout << referenceString[i] << " ";
     }
     cout << endl;

     //Split info section from real output section
     int dashLength = numOfReferences * 3;
       
    for (int i = 0; i < dashLength; i++)
    {
            cout << "___";
    }

    cout << endl;
    

    //Step 2: Fifo Algorithm 

    //Step 3: Output
    //Step 3.a: Calculate num of page faults 
    
    //Step 3.b: Display like the figure in the final project instruction.
    //Basically show all of the input string values but only show the pages when a page fault occurs.
   
    //Other implementation output notes: 
    //Seperator is a number of dashes corresponding to the length of the input string, looks like about 6x dashes per num of input string values
    //Just one space to the right of each string value

    return 0;

}