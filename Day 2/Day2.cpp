#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <array>
#include <chrono>
#include <cmath>
#include "../Helpful_Commands.cpp"
using namespace std;

typedef long long ll;

array<array<array<ll,2>,2>,20> lengthFactors;

ll getSmallestIncrement(int length, int offset)
{
   
    int i = offset-1;
    ll total =0;
    while(i < length)
    {
        total += pow(10,length - i-1);
        i += offset;
    }
    return total;
}

vector<array<ll, 2>> decodeInput(string line)
{
    vector<ll> numbers = stripNumsLong(line);
    vector<array<ll,2>> ranges;
    for(int i = 0; i + 1 < numbers.size();i+=2)
    {
        ll firstNum = numbers[i];
        ll secondNum = numbers[i+1];
        array<ll,2> range{{firstNum,secondNum}};
        ranges.push_back(range);
    }
    return ranges;
}

bool hasRepeatedNum(string number,int offset)
{
    if(number.size() % offset != 0) {return false;}
    int repeatedNum = number.size() / offset;
    for(int i = 0; i < offset;i++)
    {
        for(int j = 1;j < repeatedNum;j++)
        {
            if(number[i] != number[i+(j*offset)])
            {
                return false;
            }
        }
    }
    return true;
}

array<array<ll,2>,2> getFactors(int length)
{
    array<array<ll,2>,2> factors;
    factors[0][0] = 0;
    factors[0][1] = 0;
    factors[1][0] = 0;
    factors[1][1] = 0;
    int factorIndex = 0;
    for(int i = length / 2;i > 0;i--)
    {
        if(length % i != 0) {continue;} //not a factor

        bool isFactor = false;
        for(int j = 0; j < factors.size();j++) 
        {
            //we don't want any factor that is a factor of another factor
            //because all of it's ids will be within the other factor
            if(factors[j][0] % i == 0 && factors[j][0] != 0 && factors[j][0] < 10) {isFactor = true;}
        }
        if(isFactor) {continue;}
        factors[factorIndex][0] = i;
        factors[factorIndex][1] = getSmallestIncrement(length,i);
        factorIndex++;
    }
    return factors;
}

vector<array<ll,2>> splitRanges(vector<array<ll,2>> ranges)
{
    int rangesOriginalLength = ranges.size();
    for(int i = 0; i < rangesOriginalLength;i++)
    {
        ll startRange = ranges[i][0];
        ll endRange = ranges[i][1];
        if(to_string(startRange).size() != to_string(endRange).size()) {
            //our input only ever has one digit diff so we'll be lazy and only consider that
            array<ll,2> newRange = {0,endRange}; //0 will be replaced
            string smallEndRangeString = "";
            for(int j = 0; j < to_string(startRange).size();j++)
            {
                smallEndRangeString += '9';
            }
            ll smallEndRange = atoi(smallEndRangeString.c_str());
            ll bigEndRange = smallEndRange + 1;
            ranges[i][1] = smallEndRange; //replacing endRange;
            newRange[0] = bigEndRange;
            ranges.push_back(newRange);
        }
    }
    return ranges;
}



int main(){
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
    auto t4 = high_resolution_clock::now();
    auto t3 = high_resolution_clock::now();

    string fileLine;
    ifstream inputFile("input2.txt");
    getline(inputFile,fileLine);
    vector<array<ll,2>> ranges = decodeInput(fileLine);
    ranges = splitRanges(ranges);
    

    ll total = 0;
    set<ll> ids;
    for(int i = 0; i < ranges.size();i++)
    {
        
        ll startRange = ranges[i][0];
        ll endRange = ranges[i][1];

        int rangeLength = to_string(startRange).size();
        if(lengthFactors[rangeLength][0][0] == 0) //not in hashamp
            {
                lengthFactors[rangeLength] = getFactors(rangeLength);
            }
        array<array<ll,2>,2> factors = lengthFactors[rangeLength];
       
        

        for(int k = 0; k < 2;k++)
        {
            int factor = factors[k][0];
            if(factor == 0) {continue;}
            ll j = 0;
            bool firstFound = false;
            ll factorOffset = factors[k][1];

            while(j <= endRange)
            {
                if(j >= startRange)
                {
                    ids.insert(j);
                }
                j+= factorOffset;
            }
        }
    }
    for(ll id :ids)
    {
        total += id;
    }
    cout << total << '\n';
    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    duration<double, std::milli> ms_double = t2 - t1;

    

    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";
    return 0;
}