#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "../Helpful_Commands.cpp"
#include <unordered_map>
#include <queue>
#include <algorithm>
using namespace std;
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
            newState.currentState[index]--;
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
struct node
{
    joltState state;
    vector<bool> buttonsPressed;
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

enum searchResult
{
    SUCCESS,
    FAILURE
};
class joltStateHasher {
public:
  std::size_t operator()(joltState const& vec) const {
    std::size_t seed = vec.currentState.size();
    for(auto x : vec.currentState) {

        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}
};

unordered_map<joltState,int,joltStateHasher> lengthHashMap;

bool buttonHasIndex(button but, int index)
{
    for(int i=0;i < but.buttonIndexes.size();i++)
    {
        if(but.buttonIndexes[i] == index) {return true;}
    }
    return false;
}

bool comp(button a, button b)
{
    return a.buttonIndexes.size() > b.buttonIndexes.size();
}

bool isStateEven(joltState state)
{
    for(int i : state.currentState)
    {
        if(i % 2 != 0) {return false;} 
    }
    return true;
}

bool isStateNonNegative(joltState state)
{
    for( int i : state.currentState)
    {
        if ( i < 0) {return false;}
    }
    return true;
}

bool isStateZeroed(joltState state)
{
    for(int i : state.currentState)
    {
        if (i != 0) {return false;}
    }
    return true;
}

joltState getHalfState(joltState state)
{
    joltState newState;
    newState.currentState = state.currentState;
    for(int i = 0; i < state.currentState.size();i++)
    {
        newState.currentState[i] = state.currentState[i] / 2;
    }
    return newState;
}
queue<node> searchQueue;
void searchNode(node state, machine mach)
{
    if(mach.buttons.size() == state.buttonsPressed.size()) {return;} //end of graph

    node offNode; //node where we don't press the next button
    node onNode; //node where we press the next button
    for(int i = 0; i < state.buttonsPressed.size();i++){
        offNode.buttonsPressed.push_back(state.buttonsPressed[i]);
        onNode.buttonsPressed.push_back(state.buttonsPressed[i]);
    }

    offNode.state = state.state;
    offNode.buttonsPressed.push_back(false);
    searchQueue.push(offNode);

    onNode.state = mach.buttons[state.buttonsPressed.size()].pressButton(state.state);
    onNode.buttonsPressed.push_back(true);
    searchQueue.push(onNode);
}

vector<vector<button>> findEvenButSeqs(joltState state, machine mach)
{
    //we have a state and we want to try all combinations of buttons being pressed or not
    //which lead to all nonnegative even integers
    //doing this with a bfs
    vector<node> evenNodes;
    node startNode;
    for(int i = 0; i < state.currentState.size();i++)
    {
        startNode.state.currentState.push_back(state.currentState[i]);
    }
    searchQueue.push(startNode);

    while(searchQueue.size() != 0)
    {
        node nextNode = searchQueue.front();
        searchQueue.pop();
        if(!isStateNonNegative(nextNode.state)) {continue;}
        if(isStateEven(nextNode.state)) {evenNodes.push_back(nextNode);}
        searchNode(nextNode,mach);
    }

    vector<vector<button>> sequences;
    for( node n : evenNodes)
    {
        vector<button> seq;
        for(int i = 0; i < n.buttonsPressed.size();i++)
        {
            bool buttonPresent = n.buttonsPressed[i];
            if(buttonPresent) {seq.push_back(mach.buttons[i]);}
        }
        sequences.push_back(seq);
    }
    return sequences;
}

//this is the recursive function
int getShortestButtons(joltState currentState,machine mach)
{
    //if we are already at 0 0 0 0..., we can return 0
    if(isStateZeroed(currentState)) {return 0;}
    //if this state is in the length hashmap, we can just return the value in there
    if(lengthHashMap.count(currentState) != 0) {return lengthHashMap[currentState];}
    //for each machine, we find all the possible ways to get targets to even numbers
    //pressing each button zero or one times.
    vector<vector<button>> buttonSequences = findEvenButSeqs(currentState,mach);
    //we can then, for each possible way, half the numbers and search again recursively
    int minLength = 10000000;
    for(vector<button> butSeq : buttonSequences)
    {
        joltState state = currentState;
        for(button but : butSeq)
        {
            state = but.pressButton(state);
        }
        int length = butSeq.size() + 2 * getShortestButtons(getHalfState(state),mach); 
        if(length < minLength) {minLength = length;}
    }
    //then compare and return the way that is the least number of buttons (and add to the length hashmap)
    lengthHashMap[currentState] = minLength;
    return minLength;
}
int calculateMinJoltMoves(machine mach)
{
   lengthHashMap.clear();
   return getShortestButtons(mach.targetJolt,mach);
}
int main(){
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