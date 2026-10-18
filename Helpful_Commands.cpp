#include <vector>
#include <cctype>
#include <string>
#include <iostream>
using namespace std;

vector<float> stripNumsFloat(string inputString)
{
    vector<float> strippedNums;
    string nextNumString;
    bool onNumber = false;

    for(int i = 0; i < inputString.size();i++)
    {
        char currentChar = inputString[i];
        if(isdigit(currentChar) || currentChar == '-')
        {
            nextNumString +=currentChar;
            onNumber = true;
            if(i < inputString.size() - 1)
            {
                continue;
            }
        }
        if(onNumber) //end of number
        {
            strippedNums.push_back(stof(nextNumString));
            nextNumString = "";
            onNumber = false;
        }
    }
    return strippedNums;
}

vector<int> stripNumsInt(string inputString)
{
    vector<int> strippedNums;
    string nextNumString;
    bool onNumber = false;

    for(int i = 0; i < inputString.size();i++)
    {
        char currentChar = inputString[i];
        if(isdigit(currentChar))
        {
            nextNumString +=currentChar;
            onNumber = true;
            if(i < inputString.size() - 1)
            {
                continue;
            }
        }
        if(onNumber) //end of number
        {
            strippedNums.push_back(atoi(nextNumString.c_str()));
            nextNumString = "";
            onNumber = false;
        }
    }
    return strippedNums;
}

vector<long long> stripNumsLong(string inputString)
{
    vector<long long> strippedNums;
    string nextNumString;
    bool onNumber = false;

    for(int i = 0; i < inputString.size();i++)
    {
        char currentChar = inputString[i];
        if(isdigit(currentChar))
        {
            nextNumString +=currentChar;
            onNumber = true;
            if(i < inputString.size() - 1)
            {
                continue;
            }
        }
        if(onNumber) //end of number
        {
            strippedNums.push_back(stoll(nextNumString));
            nextNumString = "";
            onNumber = false;
        }
    }
    return strippedNums;
}

// int main(){
//     string stringWithNums;
//     getline(cin,stringWithNums);
//     cout << stringWithNums;
//     vector<float> nums = stripNums(stringWithNums)[0];
//     return 0;
// }