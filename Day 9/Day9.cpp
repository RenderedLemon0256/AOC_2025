#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include "../Helpful_Commands.cpp"
#include <cmath>
using namespace std;

typedef long long ll;

ll calculateSize(array<int,2> position1, array<int,2> position2)
{
    int width = (max(position1[0],position2[0]) - min(position1[0],position2[0]))+1;
    int height = (max(position1[1],position2[1]) - min(position1[1],position2[1]))+1;
    return ll(width) * ll(height);
}

int part1() {
    vector<array<int,2>> positions;
    string fileLine;
    fstream file("input9.txt");
    while(getline(file,fileLine))
    {
        array<int,2> position;
        vector<int> numbers = stripNumsInt(fileLine);
        position[0] = numbers[0];
        position[1] = numbers[1];
        positions.push_back(position);
    }
    array<int,2> position1 = positions[0];
    array<int,2> position2 = positions[1];
    ll maxSize = calculateSize(position1,position2);
    bool swapped = true;
    while (swapped)
    {
        swapped = false;
        for(int i = 0; i < positions.size();i++)
        {
            array<int,2> newPos = positions[i];
            ll leftSwapSize = calculateSize(newPos,position2);
            ll rightSwapSize = calculateSize(newPos,position1);

            if(leftSwapSize <= maxSize && rightSwapSize <= maxSize) {continue;}

            if(leftSwapSize > rightSwapSize)
            {
                position1 = newPos;
                maxSize = leftSwapSize;
                swapped = true;
            }
            else
            {
                position2 = newPos;
                maxSize = rightSwapSize;
                swapped = true;
            }
        }
    }
    cout << maxSize;
    return 0;
}
bool validRectangle(array<int,2> position1, array<int,2> position2)
{
    return true;
}
int part2()
{
    vector<array<int,2>> positions;
    string fileLine;
    fstream file("input9.txt");
    array<array<char,10000>,10000> floor;
    array<char,10000> floorLine;
    floorLine.fill('.');
    floor.fill(floorLine);
    while(getline(file,fileLine))
    {
        array<int,2> position;
        vector<int> numbers = stripNumsInt(fileLine);
        position[0] = numbers[0];
        position[1] = numbers[1];
        positions.push_back(position);
    }
    array<int,2> previousPos= positions[0];
    floor[previousPos[0]][previousPos[1]] == '#';
    for(int i = 1; i < positions.size();i++)
    {
        array<int,2> pos = positions[i];
        floor[pos[0]][pos[1]] == '#';
        if(pos[0] != previousPos[0]){ //diff in x so draw line between
            int sign = -1;
            if(pos[0] > previousPos[0]) {sign = 1;}
            for(int i = previousPos[0]+sign; i )
        }
    }
    
    array<int,2> position1 = positions[0];
    array<int,2> position2 = positions[1];
    ll maxSize = calculateSize(position1,position2);
    bool swapped = true;
    while (swapped)
    {
        swapped = false;
        for(int i = 0; i < positions.size();i++)
        {
            array<int,2> newPos = positions[i];
            ll leftSwapSize = calculateSize(newPos,position2);
            if(!validRectangle(newPos,position2)) {leftSwapSize = -1;}
            ll rightSwapSize = calculateSize(newPos,position1);
            if(!validRectangle(newPos,position1)) {rightSwapSize = -1;}


            if(leftSwapSize <= maxSize && rightSwapSize <= maxSize) {continue;}

            if(leftSwapSize > rightSwapSize)
            {
                position1 = newPos;
                maxSize = leftSwapSize;
                swapped = true;
            }
            else
            {
                position2 = newPos;
                maxSize = rightSwapSize;
                swapped = true;
            }
        }
    }
    cout << maxSize;
    return 0;
}
int main(){
    return part1();
}