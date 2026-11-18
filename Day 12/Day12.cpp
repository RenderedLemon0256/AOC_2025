#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <assert.h>
#include <map>
#include <queue>
#include "../Helpful_Commands.cpp"
using namespace std;
struct shape
{
    int shapeIndex;
    vector<array<char,3>> shape;
};
struct region
{
    int height;
    int width;
    vector<int> shapeCounts;
};
struct regionState
{
    vector<vector<char>> currentState; //int so can be filled with shape index
    vector<int> currentShapeCounts;
    friend bool operator==(const regionState& lhs, const regionState& rhs)
    {
        return lhs.currentState == rhs.currentState && lhs.currentShapeCounts == lhs.currentShapeCounts;
    }
    friend bool operator<(const regionState& lhs, const regionState& rhs)
    {
        return lhs.currentState < rhs.currentState;
    }
};
vector<shape> shapes;
vector<region> regions;
enum linesPart
{
    SHAPE,
    REGION,
    NONE
};
bool lineIsShape(string line)
{
    if(line[0] == '.' || line[0] == '#') {return true;}
    return false;
}
array<char,3> shapeLine(string line)
{
    array<char,3> shapeLine;
    for(int i = 0; i < 3;i++)
    {
        shapeLine[i] = line[i];
    }
    return shapeLine;
}
region decodeRegionLine(string line)
{
    vector<int> nums = stripNumsInt(line); 
    region newRegion;
    if(nums.size() != 8)
    {
        cout << "ERROR: " << line;
        return newRegion;
    }
    newRegion.width = nums[0];
    newRegion.height = nums[1];
    vector<int> shapeCounts;
    for(int i = 2;i< nums.size();i++)
    {
        shapeCounts.push_back(nums[i]);
    }
    assert(shapeCounts.size() == 6);
    newRegion.shapeCounts = shapeCounts;
    return newRegion;
}
void decodeLines(vector<string> lines)
{
    int shapeIndex = 0;
    vector<array<char,3>> currentShape;
    linesPart currentPart = NONE;
    for(int i = 0; i < lines.size();i++)
    {
        string line = lines[i];
        if(line == "")
        {
            if(currentPart != SHAPE)
            {
                continue;
            }
            shape newShape;
            newShape.shape = currentShape;
            newShape.shapeIndex = shapeIndex;
            shapes.push_back(newShape);
            shapeIndex++;
            currentShape.clear();
            currentPart = NONE;
            continue;
        }
        if(lineIsShape(line))
        {
            currentPart = SHAPE;
            currentShape.push_back(shapeLine(line));
            continue;
        }
        if(line[1] == ':') {continue;} //shape header
        else
        {
            regions.push_back(decodeRegionLine(line));
        }
    }
}
void readInput()
{
    string fileLine;
    fstream file("input125.txt");
    vector<string> lines;
    while(getline(file,fileLine))
    {
        lines.push_back(fileLine);
    }
    decodeLines(lines);
}

vector<regionState> visitedStates;
map<regionState,regionState> predecesorMap;
queue<regionState> nextStateQueue;

bool allShapesUsed(regionState state)
{
    for(int i = 0; i < state.currentShapeCounts.size();i++)
    {
        if(state.currentShapeCounts[i] > 0) {return false;}
    }
    return true;
}

bool shapeFits(shape shapeToInsert,int x,int y,vector<vector<char>> currState)
{
    for(int i = -1; i < 2;i++)
    {
        for(int j = -1; j < 2;j++)
        {
            if(shapeToInsert.shape[i+1][j+1] == '#' && currState[x+i][y+j] == '#') {return false;}
        }
    }
    return true;
}

vector<vector<char>> addShape(shape shapeToInsert,int x,int y,vector<vector<char>> currState)
{
    for(int i = -1; i < 2;i++)
    {
        for(int j = -1; j < 2;j++)
        {
            if(currState[x+i][y+j] == '#') {continue;}
            currState[x+i][y+j] = shapeToInsert.shape[i+1][j+1];
        }
    }
    return currState;
}

bool stateVisited(regionState currState)
{
    for(int i =0;i < visitedStates.size();i++)
    {
        if(currState == visitedStates[i]) {return true;} 
    }
    return false;
}

bool shapeConnected(vector<vector<char>> currState,int x, int y) 
//checking shape is connected as adding unconnected never leads to correct solution uniquely 
{
    for(int i = -1;i < 2;i++)
    {
        for(int j = -2;j< 2;j++)
        {
            int xCorr = x+i;
            int yCorr = y+j;
            if(xCorr < 0 || yCorr < 0 || xCorr > currState.size() || yCorr > currState[0].size()){
                return true; // on edge so counts as connected
            }
            if(currState[i+x][j+y] == '#') {return true;}
        }
    }
    return false;
}

shape mirrorShape(shape startShape)
{
    vector<array<char,3>> newShape = startShape.shape;
    vector<array<char,3>> placeholdShape = vector<array<char,3>>();
    for(int j = 0; j < 3;j++)
    {
        placeholdShape.push_back({'.','.','.'});
    }
    for(int x = 0; x < 3; x++)
    {
        for(int y = 0; y < 3; y++)
        {
            placeholdShape[x][y] = newShape[2-x][y];
        }
    }
    newShape.clear();
    for(int j = 0; j < 3;j++)
    {
        newShape.push_back(placeholdShape[j]);
    }
    shape returnShape;
    returnShape.shape = newShape;
    returnShape.shapeIndex = startShape.shapeIndex;
    return returnShape;
}

shape rotateShape(shape startShape,int rots)
{
    vector<array<char,3>> newShape = startShape.shape;
    vector<array<char,3>> placeholdShape;
    for(int i = 0; i < rots;i++)
    {
        placeholdShape.clear();
        for(int j = 0; j < 3;j++)
        {
            placeholdShape.push_back({'.','.','.'});
        }

        for(int x = 0; x < 3; x++)
        {
            for(int y = 0; y < 3; y++)
            {
                placeholdShape[x][y] = newShape[2-y][x];
            }
        }
        newShape.clear();
        for(int j = 0; j < 3;j++)
        {
            newShape.push_back(placeholdShape[j]);
        }
    }
    shape returnShape;
    returnShape.shape = newShape;
    returnShape.shapeIndex = startShape.shapeIndex;
    return returnShape;
}

void processState(regionState currState)
{
    bool availableShapes = true;
    int currentShape = 0;
    while (availableShapes) //trying each shape
    {
        shape shapeToInsert;
        bool foundShape = false;
        for(int i = currentShape; i < currState.currentShapeCounts.size();i++)
        {
            if(currState.currentShapeCounts[i] != 0)
            {
                shapeToInsert = shapes[i];
                currentShape = i+1;
                foundShape = true;
                break;
            }
        }
        if(!foundShape) {break;}

        vector<vector<char>> stateData = currState.currentState;
        for(int orr = 0; orr < 8;orr++) //trying each combination of rotation and mirroring
        {
            for(int i = 1; i < stateData.size()-1;i++)
            {
                for(int j = 1; j < stateData[0].size()-1;j++)
                {
                    int rotCount = orr % 3;
                    bool mirrored = orr > 3;

                    shapeToInsert = rotateShape(shapeToInsert,rotCount);
                    if(mirrored){
                        shapeToInsert = mirrorShape(shapeToInsert);
                    }

                    if(!shapeFits(shapeToInsert,i,j,stateData)) {continue;}
                    if(!shapeConnected(stateData,i,j)) {continue;}
                    regionState newState;
                    newState.currentState = addShape(shapeToInsert,i,j,stateData);
                    newState.currentShapeCounts = currState.currentShapeCounts;
                    newState.currentShapeCounts[shapeToInsert.shapeIndex]--;
                    if(stateVisited(newState)) {
                        continue;}
                    nextStateQueue.push(newState);
                    predecesorMap[newState] = currState;
                    visitedStates.push_back(newState);
                    return;
                }
            }
        }
    }
    if (predecesorMap.find(currState) == predecesorMap.end()) {return;}
    nextStateQueue.push(predecesorMap[currState]);
}



bool isRegionFit(region currRegion)
{
    //going to do dfs on shape positions
    regionState blankState;
    vector<vector<char>> blankStateData(currRegion.height,vector<char>(currRegion.width,'.')); //initalising with 0
    blankState.currentState = blankStateData;
    blankState.currentShapeCounts = currRegion.shapeCounts;
    while(!nextStateQueue.empty()) {nextStateQueue.pop();}
    visitedStates.clear();
    predecesorMap.clear();

    nextStateQueue.push(blankState);

    while (nextStateQueue.size() != 0)
    {
        regionState nextState = nextStateQueue.front();
        nextStateQueue.pop();
        if(allShapesUsed(nextState))
        {
            return true;
        }
        processState(nextState);
    }
    return false;
}
int getNumFitRegions()
{
    int total = 0;
    for(int i = 0; i < regions.size();i++)
    {
        region currRegion = regions[i];
        bool fitRegion = isRegionFit(currRegion);
        if(fitRegion) {total++;}
        cout << fitRegion << endl;
    }
    return total;
}
int main() {
    readInput();
    int numRegions = getNumFitRegions();
    cout << numRegions;
    return 0;
}