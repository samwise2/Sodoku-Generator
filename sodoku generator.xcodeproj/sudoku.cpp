//
//  main.cpp
//  sodoku generator
//
//  Created by Sam Orend on 2018-06-26.
//  Copyright © 2018 Sam Orend. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

typedef vector<int> ivec;
typedef vector<ivec> imatrix;

imatrix solutions;

//to print 2-D vector
ostream & operator<<(ostream & stream, ivec &vec)
{
    for (int i = 0; i < (int) vec.size(); i++)
    {
        stream << vec[i] << " ";
    }
    
    return stream;
}

//function declarations
vector<vector<int>> insertRandomNumber (vector<vector<int>>);
bool checkRows (vector<vector<int>>);
bool checkColumns(vector<vector<int>>);
bool checkGroup(vector<vector<int>>);
bool checkAll3(vector<vector<int>>);
bool checkDone();
void backtrackSodoku();

//vectors to manipulate sudoko grid
vector<int> possibleInsertionPositions;
vector<vector<int>> grid;
vector<int> positionsOccupied;
vector<int> possibleSpots;

int selectedPos;
int numSteps;

int main() {
    int numberRepetitions;
    //asks user how many fully completed soduko grids they want (different)
    cout<<"How many reps bro"<<endl;
    cin>>numberRepetitions;
    int repCounter = 1;
    bool firstTime = true;
    //label to jump back to in order to create more grids
makeAGrid:
    bool solutionFound = false;
    vector<int> transfer;
    //creates array of all possible insertion positions
    for(int i=1; i<=81; i++)
    {
        possibleInsertionPositions.push_back(i);
    }
    //temporary array with 9 zeros to compose the grid
    for(int i=1; i<=9; i++)
    {
        transfer.push_back(0);
    }
    //creates a grid like this
    /*
     
     000000000
     000000000
     000000000
     000000000
     000000000
     000000000
     000000000
     000000000
     000000000
     
     */
    //adds temporary 000000000 rows to grid
    for(int i=1; i<=9; i++)
    {
        grid.push_back(transfer);
    }
    //if this is not the first grid then delete the first 9 rows which are from the previous grid
    if (firstTime == false)
    {
        for(int i=0; i<9; i++)
        {
            grid.erase(grid.begin()+0);
        }
    }
    firstTime = false;
    bool solutionCanBeContinued = false;
    //while a completed grid has not been found
    while(!solutionFound)
    {
        //in order to create sudo-random numbers
        srand(time(NULL));
        //inserts a random number to an available position
        insertRandomNumber(grid);
        //checks if a solution can be found given this insertion
        solutionCanBeContinued = checkAll3(grid);
        if(!solutionCanBeContinued)
        {
            //if cannot continue, backtracks the action
            backtrackSodoku();
        }
        //checks if complete grid has been found
        solutionFound = checkDone();
        numSteps++;
    }
    //prints finished grid
    for(int i=0; i<=8; i++)
    {
        cout<<grid[i]<<endl;
    }
    cout<<endl;
    repCounter++;
    //if fewer grids have been produced than requested go back to label and randomize time again
    if(repCounter<=numberRepetitions)
    {
        srand(time(NULL));
        goto makeAGrid;
    }
    return 0;
}

//checks that there are no repeated numbers in the same column row or box
bool checkAll3(vector<vector<int>>)
{
    bool canContinue1 = false;
    bool canContinue2 = false;
    bool canContinue3 = false;
    canContinue1 = checkRows(grid);
    //cout<<canContinue1<<endl;
    canContinue2 = checkColumns(grid);
    //cout<<canContinue2<<endl;
    canContinue3 = checkGroup(grid);
    //cout<<canContinue3<<endl;
    if (canContinue1 == false || canContinue2 == false || canContinue3 == false)
    {
        //grid[selectedPos/9][selectedPos%9] = 0;
        return false;
    }
    return true;
}

//backtracks to remove inserted number if it does not work, and goes back two steps if no number can be inserted in that position
void backtrackSodoku()
{
    int chosenNumericalPosition2, column2, row2;
    bool saveable = false;
    for(int i=1; i<=9; i++)
    {
        chosenNumericalPosition2 = possibleSpots[selectedPos];
        column2 = chosenNumericalPosition2/9;
        row2 = chosenNumericalPosition2 - (column2*9);
        grid[column2][row2] = i;
        saveable = checkAll3(grid);
        if(saveable)
        {
            return;
            break;
        }
    }
    if (saveable == false)
    {
        chosenNumericalPosition2 = possibleSpots[selectedPos];
        column2 = chosenNumericalPosition2/9;
        row2 = chosenNumericalPosition2 - (column2*9);
        grid[column2][row2] = 0;
        positionsOccupied.pop_back();
        int previousPos = positionsOccupied[positionsOccupied.size()-1];
        chosenNumericalPosition2 = possibleSpots[previousPos];
        column2 = chosenNumericalPosition2/9;
        row2 = chosenNumericalPosition2 - (column2*9);
        grid[column2][row2] = 0;
        positionsOccupied.pop_back();
    }
}

//checks if sodoku is fully completed
bool checkDone()
{
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            if(grid[i][j] == 0)
            {
                return false;
            }
        }
    }
    return true;
}

//part of check all 3 function checks if there are repeated values in any of the groups
bool checkGroup(vector<vector<int>>)
{
    vector<int> numbers;
    int currentRow = 0;
    int currentCoulmn = 0;
     int doubleCount = 0;
    for(int x = 1; x<= 9; x++)
    {
        for(int i=currentRow; i < currentRow+3; i++)
        {
            for(int j=currentCoulmn; j < currentCoulmn+3; j++)
            {
                numbers.push_back(grid[i][j]);
            }
        }
        for(int i=0; i<numbers.size(); i++)
        {
            for(int j=0; j < numbers.size(); j++)
            {
                if (numbers[i] && numbers[j] != 0)
                {
                    if (i != j)
                    {
                        if(numbers[i] == numbers[j])
                        {
                            doubleCount++;
                        }
                        if(doubleCount >= 2)
                        {
                            return false;
                        }
                    }
                }
            }
        }
        doubleCount = 0;
        numbers.clear();
        if (x%3 == 0)
        {
            currentRow = 0;
            currentCoulmn +=3;
        }
        else if (x%3 != 0)
        {
            currentRow += 3;
        }
    }
    return true;
}
//part of checkAll3 function checks if any duplicate numbers in any of the rows
bool checkRows (vector<vector<int>>)
{
    for(int k=0; k<=8; k++)
    {
        for(int i=0; i<=8; i++)
        {
            for(int j=i+1; j<=8; j++)
            {
                if(grid[k][i]==grid[k][j])
                {
                    if(grid[k][i] != 0 && grid[k][j] != 0)
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
//part of checkAll3 function checks if any duplicate numbers in any of the columns
bool checkColumns(vector<vector<int>>)
{
    for(int k=0; k<=8; k++)
    {
        for(int i=0; i<=8; i++)
        {
            for(int j=i+1; j<=8; j++)
            {
                if(grid[i][k]==grid[j][k])
                {
                    if (grid[i][k] != 0 && grid[j][k] != 0)
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

//inserts random number into a position
vector<vector<int>> insertRandomNumber (vector<vector<int>>)
{
    int counter = 0;
    for(int i=0; i<=8; i++)
    {
        for(int j=0; j<=8; j++)
        {
            if(grid[i][j] == 0)
            {
                possibleSpots.push_back(counter);
            }
            counter++;
        }
    }
    int numPossibleInsertionPositions = possibleSpots.size();
    selectedPos = rand() % numPossibleInsertionPositions;
    int valueToInsert = rand() % 9 + 1;
    int chosenNumericalPosition = possibleSpots[selectedPos];
    int column = chosenNumericalPosition/9;
    int row = chosenNumericalPosition - (column*9);
    grid[column][row] = valueToInsert;
    positionsOccupied.push_back(possibleSpots[selectedPos]);
    return grid;
}

