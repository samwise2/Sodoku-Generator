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

ostream & operator<<(ostream & stream, ivec &vec)
{
    for (int i = 0; i < (int) vec.size(); i++)
    {
        stream << vec[i] << " ";
    }
    
    return stream;
}

vector<vector<int>> insertRandomNumber (vector<vector<int>>);
bool checkRows (vector<vector<int>>);
bool checkColumns(vector<vector<int>>);
bool checkGroup(vector<vector<int>>);
bool checkAll3(vector<vector<int>>);
bool checkDone();
void backtrackSodoku();
void insertNumber();
//bool sodokuCompleted (vector<vector<int>>);

vector<int> possibleInsertionPositions;
vector<vector<int>> grid;
vector<int> positionsOccupied;
vector<int> possibleSpots;

int selectedPos;
int numSteps;

int main() {
    // insert code here...
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
    for(int i=1; i<=9; i++)
    {
        grid.push_back(transfer);
    }
    for(int i=0; i<=8; i++)
    {
        cout<<grid[i]<<endl;
    }
    //loops until a solution is found which is definetely not the right way to do backtracking
    bool solutionCanBeContinued = false;
    while(!solutionFound)
    {
        cout<<endl;
        srand(time(NULL));
        insertRandomNumber(grid);
        for(int i=0; i<=8; i++)
        {
            cout<<grid[i]<<endl;
        }
        solutionCanBeContinued = checkAll3(grid);
        if(!solutionCanBeContinued)
        {
            backtrackSodoku();
        }
        solutionFound = checkDone();
        numSteps++;
        cout<<numSteps<<endl;
        //solutionFound = sodokuCompleted(grid);
    }
    for(int i=0; i<=8; i++)
    {
        cout<<grid[i]<<endl;
    }
    return 0;
}

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

void insertNumber(int valueToInsert)
{
    int chosenNumericalPosition = possibleSpots[selectedPos];
    int column = chosenNumericalPosition/9;
    int row = chosenNumericalPosition - (column*9);
    grid[column][row] = valueToInsert;
}

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

//checks if there is a viable solution which could potentially be created
//effectively checks for duplicates within each row?
//doesnt check groups of 3
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
    /*for(int i=0; i<=80; i++)
    {
        for(int x=0; x < possibleSpots.size(); x++)
        {
            bool shouldAdd = true;
            if(possibleSpots[x] == i)
            {
                shouldAdd = false;
            }
        }
        
    }*/
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

