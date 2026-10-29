#include <deque>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../Helpful_Commands.cpp"
using namespace std;
typedef long long ll;
struct node
{
    string nodeName;
    vector<string> connections;
    friend bool operator==(const node& lhs, const node& rhs)
    {
        return lhs.nodeName == rhs.nodeName;
    }
};
struct pathNode
{
    node currentNode;
    ll nodeMultipler;
};
vector<node> nodes;
node createNode(string line)
{
    vector<string> words = stripWords(line);
    node newNode;
    string nodeName = words[0].substr(0,words[0].size()-1); //still has the : so we get rid of it
    newNode.nodeName = nodeName;
    words.erase(words.begin()); //removing the node name
    newNode.connections = words;
    return newNode;

}
void readInNodes()
{
    string fileLine;
    fstream file("input11.txt");
    while(getline(file,fileLine))
    {
        nodes.push_back(createNode(fileLine));
    }
}
node getNode(string nodeName)
{
    for(int i = 0; i < nodes.size();i++)
    {
        node currentNode = nodes[i];
        if(currentNode.nodeName == nodeName)
        {
            return currentNode;
        }
    }
    cout << "Node search error";
    return nodes[0];
}
deque<pathNode> searchQueue;
enum searchResult
{ 
    SUCESS,
    FAILURE,
    ERROR
};
int getDuplicateIndex(node duplicatedNode)
{
    int index = -1;
    for(int i = 0;i < searchQueue.size();i++)
    {
        if(searchQueue[i].currentNode == duplicatedNode)
        {
            index = i;
        }
    }
    return index;
}
void searchNode(pathNode currentNode)
{
    for(int i = 0; i < currentNode.currentNode.connections.size();i++)
    {
        node newNode = getNode(currentNode.currentNode.connections[i]);
        pathNode newPNode;
        newPNode.currentNode = newNode;
        newPNode.nodeMultipler = currentNode.nodeMultipler;
        int duplicatedIndex = getDuplicateIndex(newNode);
        if(duplicatedIndex == -1) //not duplicated
        {
            searchQueue.push_back(newPNode);
        }
        else{
            searchQueue[duplicatedIndex].nodeMultipler += newPNode.nodeMultipler;
        }
    }
}
ll BFSNodes(node startNode, node endNode)
{
    pathNode startPNode;
    startPNode.currentNode = startNode;
    startPNode.nodeMultipler = 1;
    searchQueue.push_back(startPNode);
    ll total = 0;
    while(searchQueue.size() >0)
    {
        pathNode nextNode = searchQueue.front();
        searchQueue.pop_front();
        if(nextNode.currentNode == endNode) {
            total+=nextNode.nodeMultipler;
            continue;
        }
        searchNode(nextNode);
    }
    return total;
}
int main() {
    readInNodes();
    node startNode = getNode("svr");
    node endNode = getNode("dac");
    ll part1 = BFSNodes(startNode,endNode);
    cout << part1 << '\n';
    ll part2 = BFSNodes(getNode("dac"),getNode("fft"));
    cout << part2 << '\n';
    ll part3 = BFSNodes(getNode("fft"),getNode("out"));
    cout << part3 << '\n';


    ll dacFirstNum = part1 * part2 * part3;

    part1 = BFSNodes(getNode("svr"),getNode("fft"));
    cout << part1 << '\n';
    part2 = BFSNodes(getNode("fft"),getNode("dac"));
    cout << part2 << '\n';
    part3 = BFSNodes(getNode("dac"),getNode("out"));
    cout << part3 << '\n';

    ll fftFirstNum = part1 * part2 * part3;

    cout << dacFirstNum + fftFirstNum;

    return 0;
}