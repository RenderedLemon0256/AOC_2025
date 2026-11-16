#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <assert.h>
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
int main() {
    readInput();
    cout << shapes[0].shape[0][0];
    return 0;
}