//Kody Graham & Pranaya Pudasaini
//04/05/2026
//OS Final Project - CSCI 4300
//Kody Graham: I have decided to do FIFO to really be able to compare how much better optimal is compared to the worst case scenerio 
//Pranaya Pudasaini: Implemented the OPT or Optimal Page Replacement Algorithm

//Only used simple standard imports
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

//Just to limit having to type std:: for every std library function call
using namespace std;

//Shared result data table for both of our algorithms, had to set a max size and declare as global because when it was a local var previously it was assigning to the stack and causing overflow
const int maxSize = 1000;
int outputTable[maxSize][maxSize];

//Initialize all 3 of our functions
void runFifoAlgorithm(int referenceString[], int numOfReferences, int numFrames, int outputTable[maxSize][maxSize], int& pageFaults);
void runOPTAlgorithm(int referenceString[], int numOfReferences, int numFrames, int outputTable[maxSize][maxSize], int& pageFaults);
void printResults(int referenceString[], int numOfReferences, int numFrames, int outputTable[maxSize][maxSize], int pageFaults);

int main()
{
    //Step 1: Read/parse text from file; Argument1 (first letter either F, O, or L) = replacement algorithm, Argument2 = number of memory frames (second char in file must be an int), and Argument3= the reference string(comma seperated)
    string fileName;
    string inputLine;
    ifstream inputFile;
    string repeat = "y";

    //Simple repeat loop so we dont have to relaunch our program between tests during our demo
    while (repeat == "y" || repeat == "Y")
    {

        //Simple file name input
        cout << "Enter the input file name: ";
        cin >> fileName;

        //Open the file
        inputFile.open(fileName);

        //Make sure file is open
        if (!inputFile)
        {
            cout << endl;
            cout << "ERROR: Could not open the file named:  " << fileName << endl;
            return 1;
        }

        //Read the first line of the file and save in input
        getline(inputFile, inputLine);

        //Close the file
        inputFile.close();

        //Store the 3 arguments, argumentsArray[0] = algorithm letter, argumentsArray[1] = num of frames, argumentsArray[2...1000]= page reference string
        string argumentsArray[maxSize];

        //keeps track of how many arguments we have stored so far
        int argCount = 0;

        //builds one token at a time as we scan across the input line
        string currentArg = "";

        //Split our input based on the commas
        for (int i = 0; i < inputLine.length(); i++)
        {
            //if , store last arg
            if (inputLine[i] == ',')
            {
                //Store the last saved currentArg to arguments array
                argumentsArray[argCount] = currentArg;

                //move to the next position in the arg array
                argCount++;

                //Reset currentArg so we can start fresh on the next
                currentArg = "";
            }
            else
            {
                //if its not a comma save it to currentArg so we can move it to the argumentsArray
                currentArg += inputLine[i];
            }
        }

        //Save last arg since we wont loop the last time
        if (currentArg != "")
        {
            //Store the last saved currentArg to arguments array
            argumentsArray[argCount] = currentArg;
            //move to the next position in the arg array
            argCount++;
        }

        //Make sure all 3 required args are present
        if (argCount < 3)
        {
            cout << "Error: To few arguments.\n";
            cout << "Required File Format : Algorithm letter, Number of Frames, and the Reference String\n";
            return 1;
        }

        //First arg = algorithm letter
        char algorithm = argumentsArray[0][0];

        //Second arg = num of memory frames
        int numFrames = stoi(argumentsArray[1]);

        //Everything else = reference string in order
        int referenceString[maxSize];

        //To track total # of page values that were loaded
        int numOfReferences = 0;

        //Starting at 2 because first 2 are the other arguments
        for (int i = 2; i < argCount; i++)
        {
            //Convert each page reference from a string  into an integer and then stores it in the reference string array
            referenceString[numOfReferences] = stoi(argumentsArray[i]);
            
            //Everything after the first two is going to contribute to the reference string length
            numOfReferences++;
        }

        cout << endl;

        //Basic Validation for letter chosen using this if else chain
        //If FIFO
        if (algorithm == 'F' || algorithm == 'f')
        {
            cout << "This file is marked to use the FIFO Algorithm\n";
        }
        //If OPT
        else if (algorithm == 'O' || algorithm == 'o')
        {
            cout << "This file is marked to use the OPT Algorithm\n";
        }
        //ALGORITHM CHOICE NOT AN OPTION FOR OUR PROGRAM!
        else
        {
            cout << "Error: Argument for Algorithm Name is not valid.\n";
            cout << "This program can only be used for the FIFO or Optimal algorithms!\n";
            cout << "The identified argument for algorithm was: " << algorithm << endl;

            return 1;
        }

        //Basic validation for the number of frames must be more than 0
        if (numFrames <= 0)
        {
            cout << "Error: Number of frames must be greater than 0.\n";
            return 1;
        }

        //Basic validation for the # of references in the string
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

        //Print the ref string using a simple loop
        for (int i = 0; i < numOfReferences; i++)
        {
            cout << referenceString[i] << " ";
        }
        cout << endl;

        //Split info section from real output section       
        for (int i = 0; i < numOfReferences-1; i++)
        {
            cout << "____";
        }

        cout << endl;

        //var to count the num of times a page fault occurs
        int pageFaults = 0;

        //Initialize the output table to -1's
        //Step through rows
        for (int row = 0; row < maxSize; row++)
        {
            //Step through columns per row
            for (int column = 0; column < maxSize; column++)
            {
                //Add the -1 to every slot
                outputTable[row][column] = -1;
            }
        }

        //Call my FIFO algorithm and print functions
        if (algorithm == 'F' || algorithm == 'f')
        {
            //Run my FIFO Algorithm
            runFifoAlgorithm(referenceString, numOfReferences, numFrames, outputTable, pageFaults);
            //Print Results
            printResults(referenceString, numOfReferences, numFrames, outputTable, pageFaults);

        }

        //Call Tack's OPT algorithm and the print function
        if (algorithm == 'O' || algorithm == 'o')
        {
            // RESET output table BEFORE running OPT
            for (int i = 0; i < maxSize; i++)
            {
                for (int j = 0; j < maxSize; j++)
                {
                    outputTable[i][j] = -1;
                }
            }

            // RESET page faults
            pageFaults = 0;

            // Run OPT
            runOPTAlgorithm(referenceString, numOfReferences, numFrames, outputTable, pageFaults);
            // Print results
             printResults(referenceString, numOfReferences, numFrames, outputTable, pageFaults);
        }

        cout << endl;

        //Get repeat choice from user
        cout << "Repeat, Y or N? :";
        cin >> repeat;
        cout << endl;


    }

    cout << endl;

    //Exit message
    cout << "Thank You for using our program!" << endl << "Goodbye!" << endl;

    return 0;

}

//Step 2: Fifo Algorithm
//Handles my FIFO logic
void runFifoAlgorithm(int referenceString[], int numOfReferences, int numFrames, int outputTable[maxSize][maxSize], int& pageFaults)
{
    //frames[] stores the current pages loaded in memory
    int frames[maxSize];

    //keep track of the next page # to replace
    int nextToRemove = 0;

    //Step through and set all frames array vals to -1
    for (int i = 0; i < numFrames; i++)
    {
        frames[i] = -1;
    }

    //Step through columns to the num of references in our reference string
    for (int column = 0; column < numOfReferences; column++)
    {
        //set the current page to the corresponding value in our ref string
        int currentPage = referenceString[column];

        //Set fault to true because first entry should = page fault detected, if the page is already loaded in memory this will be come false
        bool fault = true;

        //Now step through the rows to see if page already in a frame
        for (int row = 0; row < numFrames; row++)
        {   
            //if the frame value at index row equal the currentPage
            if (frames[row] == currentPage)
            {
                //Fault not found because page already loaded
                fault = false;
                break;
            }
        }

        //If page NOT already in memory than this is a page fault
        if (fault == true)
        {
            //Incriment # 0f faults
            pageFaults++;

            //Just the empty frame check val
            int emptyFrame = -1;

            //Search for empty frame
            for (int row = 0; row < numFrames; row++)
            {
                //empty if = -1
                if (frames[row] == -1)
                {
                    emptyFrame = row;
                    break;
                }
            }

            //If an empty frame found, place the page there
            if (emptyFrame != -1)
            {
                frames[emptyFrame] = currentPage;
            }
            else
            {
                //Otherwise replace oldest page in FIFO order
                frames[nextToRemove] = currentPage;

                //move to next replacement pos
                nextToRemove++;

                //Wrap back to the beginning if the next to remove is the last frame
                if (nextToRemove == numFrames)
                {
                    nextToRemove = 0;
                }
            }

            //Save frame # for this page fault row / column pair
            for (int row = 0; row < numFrames; row++)
            {
                //Replace the -1's  with updated value
                outputTable[row][column] = frames[row];
            }
        }
    }
}

// Step 3: Optimum Page Replacement Algorithm
void runOPTAlgorithm(int referenceString[], int numOfReferences, int numFrames,
                     int outputTable[maxSize][maxSize], int& pageFaults)
{
    int frames[maxSize]; // Array to store current pages in frames

    // Initialize all frames as empty (-1 means empty)
    for (int i = 0; i < numFrames; i++)
    {
        frames[i] = -1;
    }

    // Loop through each page reference
    for (int column = 0; column < numOfReferences; column++)
    {
        int currentPage = referenceString[column]; // Current page being accessed
        bool fault = true; // Assume it is a page fault

        // Check if current page is already in frames (HIT)
        for (int row = 0; row < numFrames; row++)
        {
            if (frames[row] == currentPage)
            {
                fault = false; // Page found -> no fault
                break;
            }
        }

        // If page is NOT in frames -> PAGE FAULT
        if (fault)
        {
            pageFaults++; // Increment page fault count

            int emptyFrame = -1;

            // Check if there is an empty frame available
            for (int row = 0; row < numFrames; row++)
            {
                if (frames[row] == -1)
                {
                    emptyFrame = row;
                    break;
                }
            }

            // If empty frame exists -> place page there
            if (emptyFrame != -1)
            {
                frames[emptyFrame] = currentPage;
            }
            else
            {
                // No empty frame -> apply OPTIMAL replacement
                int replaceIndex = 0;
                int farthestUse = -1;

                // Find the page that will be used farthest in the future
                for (int row = 0; row < numFrames; row++)
                {
                    int nextUse = -1;

                    // Look ahead in reference string
                    for (int k = column + 1; k < numOfReferences; k++)
                    {
                        if (frames[row] == referenceString[k])
                        {
                            nextUse = k; // Found next use
                            break;
                        }
                    }

                    // If page is NEVER used again -> replace it immediately
                    if (nextUse == -1)
                    {
                        replaceIndex = row;
                        break;
                    }

                    // Otherwise, track the page used farthest in the future
                    if (nextUse > farthestUse)
                    {
                        farthestUse = nextUse;
                        replaceIndex = row;
                    }
                }

                // Replace the chosen page
                frames[replaceIndex] = currentPage;
            }

            // Store current frame state in output table
            for (int row = 0; row < numFrames; row++)
            {
                outputTable[row][column] = frames[row];
            }
        }
    }
}


//Step 4: Output
//Display like the figure in our final project instruction.
//Basically show all of the input string values but only show the pages when a page fault occurs.

//Other implementation output notes: 
//Seperator is a number of dashes corresponding to the length of the input string, looks like about 6x dashes per num of input string values
//Just one space to the right of each string value

//Handles final output format
void printResults(int referenceString[], int numOfReferences, int numFrames, int outputTable[maxSize][maxSize], int pageFaults)
{
    cout << endl;

    //Print the reference string across the top
    for (int i = 0; i < numOfReferences; i++)
    {
        //Found width of 3 to be ideal, print each reference string val
        cout << setw(3) << referenceString[i];

    }
    cout << endl;

    //Separator line
    for (int i = 0; i < numOfReferences; i++)
    {
        cout << "___";
    }
    cout << endl;

    //Print our resultant frame table, fairly simple just stepping through the 2d array and only printing if there was a page fault in the given column
    for (int row = 0; row < numFrames; row++)
    {
        //Step through columns
        for (int column = 0; column < numOfReferences; column++)
        {
            //if value in table is still -1 just put a space
            if (outputTable[row][column] == -1)
            {
                cout << setw(3) << " ";
            }
            //otherwise print the value in the table
            else
            {
                cout << setw(3) << outputTable[row][column];
            }
        }
        cout << endl;
    }

    //Print number of page faults
    cout << endl;
    cout << "Number of Page Faults: " << pageFaults;
    cout << endl;


}
