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
struct region
{
    int height;
    int width;
    vector<int> shapeCounts;
};
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
            continue;
        }
        if(lineIsShape(line))
        {
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
    fstream file("input12.txt");
    vector<string> lines;
    while(getline(file,fileLine))
    {
        lines.push_back(fileLine);
    }
    decodeLines(lines);
}

bool isRegionFit(region currRegion)
{
    int area = currRegion.height * currRegion.width;
    int shapeArea = 0;
    for(int i = 0; i < currRegion.shapeCounts.size();i++)
    {
        shapeArea += currRegion.shapeCounts[i] * 8;
    }
    return area > shapeArea;
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