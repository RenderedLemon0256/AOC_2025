#include <fstream>
#include <iostream>
#include <string>
#include "Helpful_Commands.cpp"

int passedZeros = 0;
int prevNum = 50;
bool prevZero = false;
int decodeLine(string line)
{
    char direction = line[0];
    int num = stripNums(line)[0];

    if(direction == 'R')
    {
        return num;
    }
    else
    {
        return -num;
    }
}

int normalisePlace(int place)
{
    return ((place % 100) + 100) % 100;
}

int main(){
    string fileLine;
    ifstream inputFile("input1.txt");
    int place = 50;
    int landedZeros = 0;
    while(getline (inputFile,fileLine))
    {
        int offset = decodeLine(fileLine);
        int steps = abs(offset);
        int direction = (offset > 0) ? 1 : -1;

        for (int i = 0; i < steps; i++)
        {
            place += direction;
            place = normalisePlace(place);

            if (place == 0)
                landedZeros++;
        }
    }
    cout << fileLine + "\n";
    cout << to_string(place) + '\n';
    cout << landedZeros + passedZeros;
     return 0;
}