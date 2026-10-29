#include <queue>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../Helpful_Commands.cpp"
using namespace std;
struct node
{
    string nodeName;
    vector<string> connections;
    friend bool operator==(const node& lhs, const node& rhs)
    {
        return lhs.nodeName == rhs.nodeName;
    }
};
struct reverseNode
{
    string nodeName;
    vector<string> upwardConnections;  
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
    fstream file("input115.txt");
    while(getline(file,fileLine))
    {
        nodes.push_back(createNode(fileLine));
    }
}
vector<reverseNode> reverseNodes;
int getReverseNodeIndex(string nodeName){
    for(int i = 0 ; i < reverseNodes.size();i++)
    {
        if(reverseNodes[i].nodeName == nodeName)
        {
            return i;
        }
    }
    return -1;
}
reverseNode getReverseNode(string nodeName)
{
    return reverseNodes[getReverseNodeIndex(nodeName)];
}
int createReverseNode(string nodeName)
{
    reverseNode newNode;
    newNode.nodeName = nodeName;
    int index = reverseNodes.size();
    reverseNodes.push_back(newNode);
    return index;
}
void addConnectionToReverseNode(string reverseNodeName,string connection)
{
    int index = getReverseNodeIndex(reverseNodeName);
    if(index == -1){
        index = createReverseNode(reverseNodeName);
    }
    reverseNode node = reverseNodes[index];
    node.upwardConnections.push_back(connection);
}
void createReverseNodes(){
    for(int i = 0; i < nodes.size();i++)
    {
        node currentNode = nodes[i];
        for(int j =0; j < currentNode.connections.size();j++)
        {
            string downwardNode = currentNode.connections[j];
            addConnectionToReverseNode(downwardNode,currentNode.nodeName);
        }
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
queue<node> searchQueue;
enum searchResult
{
    SUCESS,
    FAILURE,
    ERROR
};
void searchNode(node currentNode)
{
    for(int i = 0; i < currentNode.connections.size();i++)
    {
        node newNode = getNode(currentNode.connections[i]);
        searchQueue.push(newNode);
    }
}
int BFSNodes(node startNode, node endNode)
{
    searchQueue.push(startNode);
    int total = 0;
    while(searchQueue.size() >0)
    {
        node nextNode = searchQueue.front();
        searchQueue.pop();
        if(nextNode == endNode) {
            total++;
            continue;
        }
        searchNode(nextNode);
    }
    return total;
}
int reverseBFSNodes(reverseNode startNode, string avoidNode)
{
    return 0;
}
int main() {
    readInNodes();
    node startNode = getNode("svr");
    node endNode = getNode("dac");
    int part1 = BFSNodes(startNode,endNode);
    cout << part1 << '\n';
    int part2 = BFSNodes(getNode("dac"),getNode("fft"));
    cout << part2 << '\n';
    int part3 = BFSNodes(getNode("fft"),getNode("out"));
    cout << part3 << '\n';


    int dacFirstNum = part1 * part2 * part3;

    part1 = BFSNodes(getNode("svr"),getNode("fft"));
    cout << part1 << '\n';
    part2 = BFSNodes(getNode("fft"),getNode("dac"));
    cout << part2 << '\n';
    part3 = BFSNodes(getNode("dac"),getNode("out"));
    cout << part3 << '\n';

    int fftFirstNum = part1 * part2 * part3;

    cout << dacFirstNum + fftFirstNum;

    return 0;
}