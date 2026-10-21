#include <iostream>
#include <fstream>
#include <string>
#include "../Helpful_Commands.cpp"
#include <vector>
#include <array>
#include <cmath>
using namespace std;
typedef long long ll;
int part1() {
    vector<vector<ll>> numbers;
    vector<char> operators;
    string fileLine;
    fstream file("input6.txt");
    int i = 0;
    while(getline(file,fileLine))
    {
        if(i < 4)
        {
            vector<ll> lineNumbers = stripNumsLong(fileLine);
            numbers.push_back(lineNumbers);
        }
        else // operator line
        {
            operators = stripChars(fileLine);
        }
        i++;
    }
    ll total = 0;
    for(int j = 0;j < numbers[0].size();j++)
    {
        ll lineTotal = 0;
        ll number1 = numbers[0][j];
        ll number2 = numbers[1][j];
        ll number3 = numbers[2][j];
        ll number4 = numbers[3][j];
        if(operators[j] == '*')
        {
            lineTotal = number1 * number2 * number3 * number4;
        }
        else
        {
            lineTotal = number1 + number2 + number3 + number4;
        }
        total += lineTotal;
    }
    cout << total;
    return 0;
}
vector<vector<int>> stripDigits(vector<string> digitLines)
{
    vector<vector<int>> strippedDigits;
    vector<int> nextDigits;
    for(int i = 0; i < digitLines[0].size();i++)
    {
        string numberString = "";
        for(int j = 0;j < digitLines.size();j++)
        {
            char digit = digitLines[j][i];
            if(digit != ' ') {numberString += digit;}
        }
        int number = atoi(numberString.c_str());

        if(number == 0) //all whitespace
        {
            strippedDigits.push_back(nextDigits);
            nextDigits.clear();
            continue;
        }
        nextDigits.push_back(number);
    }
    //last digits don't have whitespace after them
    strippedDigits.push_back(nextDigits);
    nextDigits.clear();

    return strippedDigits;
}
int part2()
{
    vector<vector<int>> digits;
    vector<char> operators;
    string fileLine;
    fstream file("input6.txt");
    int i = 0;
    vector<string> digitLines;
    bool onNums = true;
    while(getline(file,fileLine))
    {
        if(fileLine[0] == '*')
        {
            onNums = false;
        }
        if(onNums)
        {
            digitLines.push_back(fileLine);
        }
        else // operator line
        {
            operators = stripChars(fileLine);
        }
        i++;
    }
    digits = stripDigits(digitLines);

    ll total = 0;
    for(int j = 0;j < digits.size();j++)
    {
        ll lineTotal = 0;
        vector<int> sumDigits = digits[j];
        
        if(operators[j] == '*')
        {
            for(int k = 0; k < sumDigits.size();k++)
            {
                if(lineTotal != 0)
                {
                    lineTotal *= sumDigits[k];
                }
                else{
                    lineTotal = sumDigits[k];
                }
            }
        }
        else
        {
            for(int k = 0; k < sumDigits.size();k++)
            {
                lineTotal += sumDigits[k];
            }
        }
        total += lineTotal;
    }
    cout << total;
    return 0;
}
int main()
{
    return part2();
}