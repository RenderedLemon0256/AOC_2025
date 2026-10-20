#include <fstream>
#include <vector>
#include <iostream>
#include <string>
using namespace std;

vector<vector<char>> graph;
vector<vector<char>> graphCopy;

int getAdjacentTP(int x, int y,bool graph1Active)
{
    int total = 0;
    for(int i = 1;i >= -1; i--)
    {
        for(int j = 1; j >= -1; j--)
        {
            //skipping own place
            if (i == 0 && j == 0) {continue;}

            if(i + x > graph[y].size()-1) {continue;}
            if(i + x < 0) {continue;}

            if(j + y > graph.size()-1) {continue;}
            if(j + y < 0) { continue;}

            char character;
            if(graph1Active)
            {
                character = graph[y+j][x+i];
            }
            else
            {
                character = graphCopy[y+j][x+i];
            }
            if(character == '@') {total++;}
            
        }
    }
    return total;
}
int main() {
    string fileLine;
    fstream inputFile("input4.txt");
    while(getline(inputFile,fileLine))
    {
        vector<char> graphLine;
        for(int i = 0; i < fileLine.size();i++)
        {
            graphLine.push_back(fileLine[i]);
        }
        graph.push_back(graphLine);
        graphCopy.push_back(graphLine);
    }
    int completeTotal = 0;
    bool tpRemoved = true;
    bool graph1Active = true;
    while(tpRemoved)
    {
        int total = 0;
        tpRemoved = false;
        if(graph1Active)
        {
            for(int i = 0; i < graph.size();i++)
            {
            for(int j = 0; j < graph[i].size();j++)
            {
                if(graph[i][j] != '@') {
                    graphCopy[i][j] = '.';
                    continue;
                }
                int adjacentTP = getAdjacentTP(j,i,graph1Active);
                if(adjacentTP < 4) {
                    total++;
                    graphCopy[i][j] = '.';
                    tpRemoved = true;
                    }
                else
                {
                    graphCopy[i][j] = '@';
                }
            }
            }
            graph1Active = false;
        }
        else
        {
            for(int i = 0; i < graph.size();i++)
            {
            for(int j = 0; j < graph[i].size();j++)
            {
                if(graphCopy[i][j] != '@') {
                    graph[i][j] = '.';
                    continue;
                }
                int adjacentTP = getAdjacentTP(j,i,graph1Active);
                if(adjacentTP < 4) {
                    total++;
                    graph[i][j] = '.';
                    tpRemoved = true;
                    }
                else
                {
                    graph[i][j] = '@';
                }
            }
            }
            graph1Active = true;
        }
        completeTotal += total;
    }
    
    cout << completeTotal;
    return 0;
}