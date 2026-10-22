#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <array>
using namespace std;
typedef long long ll;
int part1() {
    vector<vector<array<int,2>>> graph;
    string fileLine;
    fstream file("input7.txt");
    while(getline(file,fileLine))
    {
        vector<array<int,2>> graphLine;
        for(int i =0; i < fileLine.size();i++)
        {
            array<int,2> graphCharacter;
            graphCharacter[0] = fileLine[i];
            graphCharacter[1] = 0;
            graphLine.push_back(graphCharacter);
        }
        graph.push_back(graphLine);
    }
    for(int i = 0; i < graph.size()-1;i++) //bottom line doesn't currently matter
    {
        for(int j = 0; j < graph[i].size();j++)
        {
            char character = graph[i][j][0];
            if(character == 'S' || character == '|') //beam going down
            {
                char characterBelow = graph[i+1][j][0];
                if(characterBelow == '.') {graph[i+1][j][0] = '|';}
                else if (characterBelow == '^'){
                    //no splitters on edge so we don't need to check
                    graph[i+1][j][1] = 1; //showing splitter was used
                    graph[i+1][j-1][0] = '|';
                    graph[i+1][j+1][0] = '|';
                }
            }
            else {continue;}
        }
    }
    int total = 0;
    //counting splitters used
    for(int i = 0; i < graph.size();i++)
    {
        for(int j = 0; j < graph[i].size();j++)
        {
            if(graph[i][j][1] == 1) {total++;}
        }
    }
    cout << total;
}
int part2(){
    vector<vector<array<ll,2>>> graph;
    string fileLine;
    fstream file("input7.txt");
    while(getline(file,fileLine))
    {
        vector<array<ll,2>> graphLine;
        for(int i =0; i < fileLine.size();i++)
        {
            array<ll,2> graphCharacter;
            graphCharacter[0] = fileLine[i];
            graphCharacter[1] = 0;
            graphLine.push_back(graphCharacter);
        }
        graph.push_back(graphLine);
    }
    for(int i = 0; i < graph.size()-1;i++) //bottom line doesn't currently matter
    {
        for(int j = 0; j < graph[i].size();j++)
        {
            char character = graph[i][j][0];
            ll beamMultipler = max(graph[i][j][1],ll(1));
            if(character == 'S' || character == '|') //beam going down
            {
                char characterBelow = graph[i+1][j][0];
                if(characterBelow == '.' || characterBelow == '|') {
                    graph[i+1][j][0] = '|';
                    graph[i+1][j][1] += beamMultipler;
                }
                else if (characterBelow == '^'){
                    //no splitters on edge so we don't need to check
                    graph[i+1][j-1][0] = '|';
                    graph[i+1][j-1][1] += beamMultipler;
                    graph[i+1][j+1][0] = '|';
                    graph[i+1][j+1][1] += beamMultipler;

                }
            }
            else {continue;}
        }
    }
    long long total = 0;
    //counting splitters used
    int lastLine = graph.size()-1;
    for(int i = 0; i < graph[lastLine].size();i++)
    {
        total += graph[lastLine][i][1];
    }
    cout << total;
    return 0;
}
int main(){
    return part2();
}