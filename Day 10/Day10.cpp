#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "../Helpful_Commands.cpp"
#include <queue>
#include <array>
#include <algorithm>
using namespace std;
//need to go down tree using bfs
//find shortest path (first path that solves)
//can stop any paths that repeat
enum inputSection
{
    NONE,
    TARGET,
    BUTTON,
    JOLTAGE
};
struct joltState
{
    vector<int> currentState;
    friend bool operator==(const joltState& lhs, const joltState& rhs)
    {
        return lhs.currentState == rhs.currentState;
    }
};
struct state
{
    vector<bool> currentState;
    friend bool operator==(const state& lhs, const state& rhs)
    {
        return lhs.currentState == rhs.currentState;
    }
};
struct button
{
    vector<int> buttonIndexes;
    state pressButton(state stat)
    {
        state newState;
        newState.currentState = stat.currentState;
        for(int index : buttonIndexes)
        {
            //flipping the specfic lights
            newState.currentState[index] = !newState.currentState[index];
        }
        return newState;
    }
    joltState pressButton(joltState stat)
    {
        joltState newState;
        newState.currentState = stat.currentState;
        for(int index : buttonIndexes)
        {
            //flipping the specfic lights
            newState.currentState[index]++;
        }
        return newState;
    }
    button(string indexString)
    {
        buttonIndexes = stripNumsInt(indexString);
    }
};
struct machine
{
    state targetState;
    vector<button> buttons;
    joltState targetJolt;
};
machine decodeLine(string line)
{
    vector<bool> targetStateData;
    vector<button> buttons;
    vector<int> targetJoltData;
    string tempNumString;
    inputSection currentSection = NONE;
    for(char lineCharacter : line)
    {
        if(lineCharacter == '[')
        {
            currentSection = TARGET;
            continue;
        }
        if(lineCharacter == ']')
        {
            currentSection = NONE;
            continue;
        }
        if(currentSection == TARGET)
        {
            if(lineCharacter == '.') {targetStateData.push_back(false);}
            else {targetStateData.push_back(true);}
            continue;
        }
        if(lineCharacter == '(')
        {
            currentSection = BUTTON;
            continue;
        }
        if(lineCharacter == ')')
        {
            buttons.push_back(button(tempNumString));
            tempNumString = "";
            currentSection = NONE;
            continue;
        }
        if(currentSection == BUTTON)
        {
            tempNumString += lineCharacter;
            continue;
        }
        if(lineCharacter == '{')
        {
            currentSection = JOLTAGE;
            continue;
        }
        if(lineCharacter == '}'){
            currentSection = NONE;
            targetJoltData = stripNumsInt(tempNumString);
            tempNumString = "";
            continue;
        }
        if(currentSection == JOLTAGE)
        {
            tempNumString += lineCharacter;
        }
    }
    machine mach;
    state targetState;
    targetState.currentState = targetStateData;
    mach.targetState = targetState;
    mach.buttons = buttons;
    joltState targetJolt;
    targetJolt.currentState = targetJoltData;
    mach.targetJolt = targetJolt;
    return mach;
}
vector<machine> getInput(string fileName)
{
    string fileLine;
    fstream file(fileName);
    vector<machine> machines;
    while(getline(file,fileLine))
    {
        machines.push_back(decodeLine(fileLine));
    }
    return machines;
}

struct node
{
    state currentState;
    int nodeDepth;
};

struct joltNode
{
    joltState currentJolt;
    int nodeDepth;
};
queue<node> searchQueue;
vector<state> visitedStates;
enum searchResult
{
    SUCCESS,
    FAILURE
};
//bfs
//process each node and push it's neighbours to the queue
//use hashmap to store whether state has been seen before
searchResult searchNode(node currentNode,machine mach)
{
    if(currentNode.currentState == mach.targetState) {return SUCCESS;}
    vector<button> buttons = mach.buttons;
    for(int i = 0;i < buttons.size();i++)
    {
        button but = mach.buttons[i];
        node newNode;
        state nodeState = but.pressButton(currentNode.currentState);
        bool visitedState = find(visitedStates.begin(),visitedStates.end(),nodeState) != visitedStates.end();

        if(visitedState) {continue;} //have visited already so we just skip

        visitedStates.push_back(nodeState); //marking it as visited (or well will be visited) for the future

        newNode.currentState = nodeState;
        newNode.nodeDepth = currentNode.nodeDepth + 1;
        searchQueue.push(newNode);
    }
    return FAILURE;
}
int calculateMinimumMoves(machine mach)
{
    while(searchQueue.size()>0)
    {
        searchQueue.pop();
    }
    visitedStates.clear();
    node startingNode;
    state startState;
    vector<bool> offStateData(mach.targetState.currentState.size(),false);
    startState.currentState = offStateData;
    startingNode.currentState = startState;
    startingNode.nodeDepth = 0;
    searchQueue.push(startingNode);
    visitedStates.push_back(startState);

    int moves = 0;
    while(searchQueue.size() > 0)
    {
        node nextNode = searchQueue.front();
        searchQueue.pop();
        searchResult result = searchNode(nextNode,mach);
        if(result == SUCCESS) {
            moves = nextNode.nodeDepth;
            break;
        }
    }
    if(moves == 0) {cout << "Search error" << '\n';}
    return moves;
}
int part1()
{
    vector<machine> machines = getInput("input10.txt");
    int total = 0;
    for(machine mach : machines)
    {
        int minMoves = calculateMinimumMoves(mach);
        cout << minMoves << '\n';
        total+=minMoves;
    }
    cout << "Total:" << total << '\n';
    return 0;
}
queue<joltNode> jSearchQueue;
vector<joltState> visitedJolts;
vector<array<joltState,2>> previousStates;

bool joltStateOverTarget(joltState currentJolt, joltState targetJolt)
{
    for(int i = 0; i < currentJolt.currentState.size();i++)
    {
        if(currentJolt.currentState[i] > targetJolt.currentState[i]) {return true;}
    }
    return false;
}

joltState getPrevJoltState(joltState currentState)
{
    for(int i = 0 ; i< previousStates.size();i++)
    {
        if(previousStates[i][1] == currentState)
        {
            return previousStates[i][0];
        }
    }
    cout <<"backtrack error" << '\n';
    return currentState;
}

searchResult searchJNode(joltNode currentNode,machine mach)
{
    if(currentNode.currentJolt == mach.targetJolt) {return SUCCESS;}
    joltNode nextNode;
    int maxButton = 0;
    vector<button> buttons = mach.buttons;
    for(int i = 0;i < buttons.size();i++)
    {
        button but = mach.buttons[i];
        if(but.buttonIndexes.size() > maxButton)
        {
            joltNode newNode;
            joltState nodeState = but.pressButton(currentNode.currentJolt);
            bool visitedJolt = find(visitedJolts.begin(),visitedJolts.end(),nodeState) != visitedJolts.end();

            if(visitedJolt) {continue;} //have visited already so we just skip
            if(joltStateOverTarget(nodeState,mach.targetJolt)) {continue;}

            newNode.currentJolt = nodeState;
            newNode.nodeDepth = currentNode.nodeDepth + 1;
            maxButton = but.buttonIndexes.size();
            nextNode = newNode;
        }   
    }
    if(maxButton == 0) //need to backtrack
    {
        joltState prevState = getPrevJoltState(currentNode.currentJolt);
        joltNode prevNode;
        prevNode.currentJolt = prevState;
        prevNode.nodeDepth = currentNode.nodeDepth -1;
        nextNode = prevNode;
        cout <<"b";

    }
    else //not backtracking so we need to add the backtrack
    {
        array<joltState,2> backtrack = {currentNode.currentJolt, nextNode.currentJolt};
        previousStates.push_back(backtrack);
        visitedJolts.push_back(nextNode.currentJolt);
    }

    jSearchQueue.push(nextNode);
    return FAILURE;
}
int calculateMinJoltMoves(machine mach)
{
    while(jSearchQueue.size()>0)
    {
        jSearchQueue.pop();
    }
    previousStates.clear();
    visitedJolts.clear();
    joltNode startingNode;
    joltState startState;
    vector<int> offStateData(mach.targetJolt.currentState.size(),0);
    startState.currentState = offStateData;
    startingNode.currentJolt = startState;
    startingNode.nodeDepth = 0;
    jSearchQueue.push(startingNode);

    int moves = 0;
    while(jSearchQueue.size() > 0)
    {
        joltNode nextNode = jSearchQueue.front();
        jSearchQueue.pop();
        searchResult result = searchJNode(nextNode,mach);
        if(result == SUCCESS) {
            moves = nextNode.nodeDepth;
            break;
        }
    }
    if(moves == 0) {cout << "Search error" << '\n';}
    return moves;
}
int part2()
{
    vector<machine> machines = getInput("input10.txt");
    int total = 0;
    for(machine mach : machines)
    {
        int minMoves = calculateMinJoltMoves(mach);
        cout << minMoves << '\n';
        total+=minMoves;
    }
    cout << "Total:" << total << '\n';
    return 0;
}
int main(){
    return part2();
}