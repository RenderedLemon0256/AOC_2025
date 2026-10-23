#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include "../Helpful_Commands.cpp"
#include <cmath>
#include <algorithm>
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
vector<array<int,2>> positions;
bool positionHasPont(array<int,2> position)
{
    for(int k = 0; k < positions.size();k++){
        if(position[0] == positions[k][0] && position[1] == positions[k][1]){
            return true;
        }
    }
    return false;
}
int getPositionIndex(array<int,2> position)
{
    for(int k = 0; k < positions.size();k++){
        if(position[0] == positions[k][0] && position[1] == positions[k][1]){
            return k;
        }
    }
    return -1;
}
bool sortTopEdge(array<int,2> a, array<int,2> b)
{
    return a[0] < b[0]; 
}
bool sortRightEdge(array<int,2> a, array<int,2> b)
{
    return a[1] < b[1];
}
bool sortBottomEdge(array<int,2> a, array<int,2> b)
{
    return a[0] > b[0];
}
bool sortLeftEdge(array<int,2> a, array<int,2> b)
{
    return a[1] > b[1];
}
int getSign(int input)
{
    if(input > 0) {return 1;}
    if(input == 0) {return 0;}
    return -1;
}
bool validRectangle(array<int,2> position1, array<int,2> position2)
{
    int maxY = max(position1[1],position2[1]);
    int minY = min(position1[1],position2[1]);
    int maxX = max(position1[0],position2[0]);
    int minX = min(position1[0],position2[0]);
    array<int,2> bottomLeft = {minX,maxY};
    array<int,2> bottomRight = {maxX,maxY};
    array<int,2> topLeft = {minX,minY};
    array<int,2> topRight = {maxX,minY};
    vector<array<int,2>> corners = {topLeft,topRight,bottomRight,bottomLeft};
    
    

    //if all four corners are points, guarenteed to be valid
    bool bottomLeftPoint = positionHasPont(bottomLeft);
    bool bottomRightPoint = positionHasPont(bottomRight);
    bool topLeftPoint = positionHasPont(topLeft);
    bool topRightPoint = positionHasPont(topRight);

    for(int i = 0; i < positions.size();i++){
        array<int,2> pos = positions[i];
        int x = pos[0];
        int y = pos[1];
        //checking if in inner rectnagle
        if(minX < x && x < maxX && minY < y && y < maxY)
        {
            return false;
        }
    }

    int prevX = positions[0][0];
    int prevY = positions[0][1];
    for(int i = 1; i < positions.size();i++){
        array<int,2> pos = positions[i];
        int x = pos[0];
        int y = pos[1];
        if(y == prevY && minY < y && y < maxY && abs(getSign(prevX - minX) - getSign(x-minX)) == 2) {return false;} //crossed left edge
        if(y == prevY && minY < y && y < maxY && abs(getSign(prevX - maxX) - getSign(x-maxX)) == 2) {return false;} //crossed right edge
        if(x == prevX && minX < x && x < maxX && abs(getSign(prevY - minY) - getSign(y-minY)) == 2) {return false;} //crossed top edge
        if(x == prevX && minX < x && x < maxX && abs(getSign(prevY - maxY) - getSign(y-maxY)) == 2) {return false;} //crossed left edge

        prevX = x;
        prevY = y;
    }

    if(bottomLeftPoint && bottomRightPoint && topLeftPoint && topRightPoint) {return true;}


    //need to get all other point on edges
    //2 corners and middle point enough for biggest rectangle so need to check
    //checking for point on any of edges
    vector<array<int,2>> topPoints;
    vector<array<int,2>> leftPoints;
    vector<array<int,2>> rightPoints;
    vector<array<int,2>> bottomPoints;
    for(int i = 0;i < positions.size();i++)
    {
        array<int,2> pos = positions[i];
        int x = pos[0];
        int y = pos[1];
        //on top edge
        if(minX < x && x < maxX && y == minY)
        {
            topPoints.push_back(pos);
        }
        //right edge
        if(minY < y && y < maxY && x == maxX)
        {
            rightPoints.push_back(pos);
        }
        //bottom edge
        if(minX < x && x < maxX && y == maxY)
        {
            bottomPoints.push_back(pos);
        }
        //left edge
        if( minY < y && y < maxY && x == minX)
        {
            leftPoints.push_back(pos);
        }
    }

    sort(topPoints.begin(), topPoints.end(), sortTopEdge);
    sort(rightPoints.begin(), rightPoints.end(), sortRightEdge);
    sort(bottomPoints.begin(), bottomPoints.end(), sortBottomEdge);
    sort(leftPoints.begin(), leftPoints.end(), sortLeftEdge);

    vector<array<int,2>> testOrder;

    if(topLeftPoint) {testOrder.push_back(topLeft);}
    for(int i = 0; i < topPoints.size();i++)
    {
        testOrder.push_back(topPoints[i]);    
    }
    if(topRightPoint) {testOrder.push_back(topRight);}
    for(int i = 0; i < rightPoints.size();i++)
    {
        testOrder.push_back(rightPoints[i]);
    }
    if(bottomRightPoint) {testOrder.push_back(bottomRight);}
    for(int i = 0; i < bottomPoints.size();i++)
    {
        testOrder.push_back(bottomPoints[i]);
    }
    if(bottomLeftPoint) {testOrder.push_back(bottomLeft);}
    for(int i = 0; i < leftPoints.size();i++)
    {
        testOrder.push_back(leftPoints[i]);
    }
    
    if(testOrder.size() < 3) {return false;} //hard to check and never optimal

    int minIndex = getPositionIndex(testOrder[0]);
    int minPlace = 0;
    for(int i = 1; i < testOrder.size(); i++)
    {
        int index = getPositionIndex(testOrder[i]);
        if(index < minIndex || minIndex == -1) 
        {
            minIndex = index;
            minPlace = i;
        }
    }

    int prevIndex = -1;
    for(int i = minPlace; i < testOrder.size();i++)
    {
        int index = getPositionIndex(testOrder[i]);
        if(index <= prevIndex && prevIndex != -1) {return false;}
        prevIndex = index;
    }
    for(int i = 0; i < minPlace;i++)
    {
        int index = getPositionIndex(testOrder[i]);
        if(index <= prevIndex && prevIndex != -1) {return false;}
        prevIndex = index;
    }
    return true;
}
int part2() {
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
    ll maxSize = 0;
    array<array<int,2>,2> maxPositions = {positions[0],positions[1]};
    for(int i = 0; i < positions.size();i++)
    {
        array<int,2> pos1 = positions[i];
        for(int j = i+1; j <positions.size();j++)
        {
            array<int,2> pos2 = positions[j];
            if(!validRectangle(pos1,pos2)) {continue;} 
            ll size= calculateSize(pos1,pos2);
            if(size > maxSize)
            {
                maxSize = size;
                maxPositions[0] = pos1;
                maxPositions[1] = pos2;
            }
        }
    }
    
    cout << maxSize << '\n'; 
    cout << maxPositions[0][0] << ',' << maxPositions[0][1] << '\n';
    cout << maxPositions[1][0] << ',' << maxPositions[1][1] << '\n';
    return 0;
}
int main(){
    return part2();
}