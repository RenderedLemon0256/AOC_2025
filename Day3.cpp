#include <string>
#include "Helpful_Commands.cpp"
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

long long getMaxVolt(string line)
{
    int endOfString = line.size();
    int maxPlace = 0;
    int prevPlace = -1;
    long long total = 0;

    for(int i = 0;i < 12;i++)
    {
        maxPlace = 0;
        int endOffset = 11 - i;
        for(int j = prevPlace+1; j < endOfString-endOffset;j++)
        {
            int num = line[j] - '0';
            if(num > maxPlace)
            {
                maxPlace = num;
                prevPlace = j;
            }
        }
        //cout << to_string(maxPlace) + '\n';
        long long fullNum = pow(10,endOffset) * maxPlace;
        total += fullNum;
    }
    
    
    return total;
}

int main() {
    string fileLine;
    fstream inputFile("input3.txt");
    long long total = 0;
    while(getline(inputFile,fileLine))
    {
        long long maxVolt = getMaxVolt(fileLine);
        total += maxVolt;
    }
    cout << total;
}