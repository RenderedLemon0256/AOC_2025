#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <set>
#include "../Helpful_Commands.cpp"
using namespace std;
typedef long long ll;
int part1() {
    vector<array<ll,2>> ranges;
    vector<ll> ids;
    string fileLine;
    fstream file("input5.txt");
    bool inRanges = true;
    while(getline(file,fileLine))
    {
        if(inRanges)
        {
            if(fileLine == "") {
                inRanges = false;
                continue;
            }
            vector<ll> numbers = stripNumsLong(fileLine);
            array<ll,2> range = {numbers[0],numbers[1]};
            ranges.push_back(range);
        }
        else
        {
            vector<ll> numbers = stripNumsLong(fileLine);
            ids.push_back(numbers[0]);
        }
    }
    int total = 0;
    for(int i = 0; i < ids.size();i++)
    {
        ll id = ids[i];
        for(int j = 0; j < ranges.size();j++)
        {
            ll startRange = ranges[j][0];
            ll endRange = ranges[j][1];
            if(startRange <= id && id <= endRange) { 
                total++;
                break;}
        }
    }
   
    cout << total;
    return 0;
}

vector<array<ll,2>> splitRanges(vector<array<ll,2>> ranges)
{
    for(int i = 0; i < ranges.size();i++)
    {
        ll startRange1 = ranges[i][0];
        ll endRange1 = ranges[i][1];
        for(int j = 0; j < ranges.size();j++)
        {
            if(i == j) {continue;}
            ll startRange2 = ranges[j][0];
            ll endRange2 = ranges[j][1];

            if(endRange1 < startRange2) //no overlap
            {
                continue;
            }

            if(startRange1 > endRange2) //no overlap
            {
                continue;
            }

            ranges.erase(ranges.begin()+j); //removing range 1 because not needed
            ll newStartRange = min(startRange1,startRange2);
            ll newEndRange = max(endRange1,endRange2);

            ranges[i][0] = newStartRange;
            ranges[i][1] = newEndRange;
            i--;
            break;
        }
    }
    return ranges;
}

int part2() {
    vector<array<ll,2>> ranges;
    string fileLine;
    fstream file("input5.txt");
    while(getline(file,fileLine))
    {
        if(fileLine == "") {
            break;
        }
        vector<ll> numbers = stripNumsLong(fileLine);
        array<ll,2> range = {numbers[0],numbers[1]};
        ranges.push_back(range);
    }
    ranges = splitRanges(ranges);
    ll freshIds = 0;
    for(int i = 0;i < ranges.size();i++)
    {
        ll startRange = ranges[i][0];
        ll endRange = ranges[i][1];
        freshIds += endRange - startRange+1;
    }
    cout << freshIds;
    return 0;
}
int main() {
    return part2();
}