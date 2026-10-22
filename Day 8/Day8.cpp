#include <vector>
#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include "../Helpful_Commands.cpp"
#include <cmath>
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct junction
{
    int x;
    int y;
    int z;
};

struct length
{
    ll dist;
    int junct1Index;
    int junct2Index;
};

ll calculateDistance(junction junct1, junction junct2)
{
    ll x = pow(max(junct1.x,junct2.x)-min(junct1.x,junct2.x),2);
    ll y = pow(max(junct1.y,junct2.y)-min(junct1.y,junct2.y),2);
    ll z = pow(max(junct1.z,junct2.z)-min(junct1.z,junct2.z),2);
    return sqrt( x+y+z );
}

int part1() {
    vector<junction> junctions;
    string fileLine;
    fstream file("input8.txt");
    while(getline(file,fileLine))
    {
        vector<int> position = stripNumsInt(fileLine);
        junction junct;
        junct.x = position[0];
        junct.y = position[1];
        junct.z = position[2];
        junctions.push_back(junct);
    }
    vector<vector<int>> circuits;
    vector<length> distances;
    for(int i = 0;i < junctions.size();i++)
    {
        vector<int> junctIndex = {i};
        circuits.push_back(junctIndex);
        for(int j = i + 1; j < junctions.size();j++)
        {
            length pairing;
            pairing.junct1Index = i;
            pairing.junct2Index = j;
            pairing.dist = calculateDistance(junctions[i],junctions[j]);
            distances.push_back(pairing);
        }
    }
    int connections = 0;
    while(connections < 1000)
    {
        ll minDist = distances[0].dist;
        int minDistPlace = 0;
        //finding current next minimum distance
        for(int i =1; i < distances.size();i++)
        {
            ll dist = distances[i].dist;
            if(dist < minDist)
            {
                minDist = dist;
                minDistPlace = i;
            }
        }
        length minDistance = distances[minDistPlace];
        distances.erase(distances.begin()+minDistPlace); //we don't want to find this again so we delete it from the list immediately
        //we have found the minimum distance
        //now we need to check the 2 junctions of the distance are in different cirucit
        int junct1Index = minDistance.junct1Index;
        int junct2Index = minDistance.junct2Index;
        int junct1Circuit = -1;
        int junct2Circuit = -1;
        //getting each junction's circuit
        for(int i = 0; i < circuits.size();i++)
        {
            vector<int> circuit = circuits[i];
            if(count(circuit.begin(),circuit.end(),junct1Index) > 0) {junct1Circuit = i;}
            if(count(circuit.begin(),circuit.end(),junct2Index) > 0) {junct2Circuit = i;}
        }
        if(junct1Circuit == junct2Circuit) {connections++;
            continue;} //same circuit so we move on
        if(junct1Circuit == -1 || junct2Circuit == -1) {cout << "error"; break;}

        //different circuit so now we need to connect these two circuits
        for(int i = 0; i<circuits[junct2Circuit].size();i++)
        {
            circuits[junct1Circuit].push_back(circuits[junct2Circuit][i]);
        }
        circuits.erase(circuits.begin()+junct2Circuit);
        connections++;
    }
    int biggest = 0;
    int bigIndex = -1;
    for(int i = 0; i < circuits.size();i++)
    {
        if(circuits[i].size() > biggest) 
        {
            biggest = circuits[i].size();
            bigIndex = i;
        }
    }
    int secondBig = 0;
    int secBigIndex = -1;
    for(int i = 0; i < circuits.size();i++)
    {
        if(i == bigIndex) {continue;}
        if(circuits[i].size() > secondBig) 
        {
            secondBig = circuits[i].size();
            secBigIndex = i;
        }
    }
    int thirdBig = 0;
    int thrBigIndex = -1;
    for(int i = 0; i < circuits.size();i++)
    {
        if(i == bigIndex) {continue;}
        if(i == secBigIndex){continue;}
        if(circuits[i].size() > thirdBig) 
        {
            thirdBig = circuits[i].size();
            thrBigIndex = i;
        }
    }
    cout << biggest * secondBig * thirdBig;
    return 0;
}
int part2(){
    vector<junction> junctions;
    string fileLine;
    fstream file("input8.txt");
    while(getline(file,fileLine))
    {
        vector<int> position = stripNumsInt(fileLine);
        junction junct;
        junct.x = position[0];
        junct.y = position[1];
        junct.z = position[2];
        junctions.push_back(junct);
    }
    vector<vector<int>> circuits;
    vector<length> distances;
    int lastJunct1 = -1;
    int lastJunct2 = -1;
    for(int i = 0;i < junctions.size();i++)
    {
        vector<int> junctIndex = {i};
        circuits.push_back(junctIndex);
        for(int j = i + 1; j < junctions.size();j++)
        {
            length pairing;
            pairing.junct1Index = i;
            pairing.junct2Index = j;
            pairing.dist = calculateDistance(junctions[i],junctions[j]);
            distances.push_back(pairing);
        }
    }
    while(true)
    {
        if(circuits.size() == 1){
            cout << junctions[lastJunct1].x * junctions[lastJunct2].x;
            break;
        }
        ll minDist = distances[0].dist;
        int minDistPlace = 0;
        //finding current next minimum distance
        for(int i =1; i < distances.size();i++)
        {
            ll dist = distances[i].dist;
            if(dist < minDist)
            {
                minDist = dist;
                minDistPlace = i;
            }
        }
        length minDistance = distances[minDistPlace];
        distances.erase(distances.begin()+minDistPlace); //we don't want to find this again so we delete it from the list immediately
        //we have found the minimum distance
        //now we need to check the 2 junctions of the distance are in different cirucit
        int junct1Index = minDistance.junct1Index;
        int junct2Index = minDistance.junct2Index;
        int junct1Circuit = -1;
        int junct2Circuit = -1;
        //getting each junction's circuit
        for(int i = 0; i < circuits.size();i++)
        {
            vector<int> circuit = circuits[i];
            if(count(circuit.begin(),circuit.end(),junct1Index) > 0) {junct1Circuit = i;}
            if(count(circuit.begin(),circuit.end(),junct2Index) > 0) {junct2Circuit = i;}
        }
        if(junct1Circuit == junct2Circuit) {continue;} //same circuit so we move on
        if(junct1Circuit == -1 || junct2Circuit == -1) {cout << "error"; break;}

        //different circuit so now we need to connect these two circuits
        for(int i = 0; i<circuits[junct2Circuit].size();i++)
        {
            circuits[junct1Circuit].push_back(circuits[junct2Circuit][i]);
        }
        circuits.erase(circuits.begin()+junct2Circuit);
        lastJunct1 = junct1Index;
        lastJunct2 = junct2Index;
    }
    return 0;
}
int main(){
    return part2();
}