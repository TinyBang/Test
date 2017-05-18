#include<vector>
#include<string>
#include<iostream>
#include<fstream>
using namespace std;
vector<pair<string,string>>lexresult;
string element="";
struct DFANode
{
vector<int> newNode;
vector<pair<int, int>> out;
int state;
int NodeNumber;
};
class DFA{
public:
DFA() {};
vector<DFANode> ALLNode;//所有DFANode的集合
vector<int> HeadAndTail;//DFA的头和可接受状态
};
int transState(int current, char edge, DFA dfa){
int des = -1;
for (int i = 0; i<dfa.ALLNode[current].out.size(); i++){
	if (dfa.ALLNode[current].out[i].first == edge){
des = dfa.ALLNode[current].out[i].second;
break;
}
}
if (des==-1) {
 cout<<"error!"<<endl;
abort();
}
return des;
}
bool ifAcc(int currentNode, DFA dfa){
	if (dfa.ALLNode[currentNode].state == 1)
return true;
else
return false;
}
	void main()
{
ifstream ifile("test.txt", ios::in);
DFA dfa;
DFANode node0;
node0.NodeNumber=0;
node0.out.push_back(pair<int,int>('a',1));
node0.out.push_back(pair<int,int>('b',2));
node0.out.push_back(pair<int,int>('c',3));
node0.state=1;
dfa.ALLNode.push_back(node0);
DFANode node1;
node1.NodeNumber=1;
node1.out.push_back(pair<int,int>('b',4));
node1.out.push_back(pair<int,int>('c',5));
node1.state=1;
dfa.ALLNode.push_back(node1);
DFANode node2;
node2.NodeNumber=2;
node2.out.push_back(pair<int,int>('b',2));
node2.out.push_back(pair<int,int>('c',3));
node2.state=1;
dfa.ALLNode.push_back(node2);
DFANode node3;
node3.NodeNumber=3;
node3.out.push_back(pair<int,int>('b',2));
node3.out.push_back(pair<int,int>('c',3));
node3.state=1;
dfa.ALLNode.push_back(node3);
DFANode node4;
node4.NodeNumber=4;
node4.out.push_back(pair<int,int>('b',4));
node4.out.push_back(pair<int,int>('c',5));
node4.state=1;
dfa.ALLNode.push_back(node4);
DFANode node5;
node5.NodeNumber=5;
node5.out.push_back(pair<int,int>('b',4));
node5.out.push_back(pair<int,int>('c',5));
node5.state=1;
dfa.ALLNode.push_back(node5);
	int currentNode = 0;
char current=' ';
string action;
	for (;;) {
		current = ifile.get();
	if (current==-1) {
		if (ifAcc(currentNode, dfa)) {
if(element!=""&&element!=" "){
			lexresult.push_back(pair<string, string>(element, action));
				element = "";
currentNode=0;
}
}
else abort();
break;
}
	if (current != ' ') {
	element += current;
currentNode = transState(currentNode, current, dfa);
continue;
  }
	else {
		if (ifAcc(currentNode, dfa)) {
			lexresult.push_back(pair<string, string>(element, action));
				element = "";
currentNode=0;
continue;
}
	else {
	cout <<"error!" << endl;
abort();
}
}
}	for (int i = 0; i < lexresult.size(); i++){
cout << lexresult[i].first << "," << lexresult[i].second << endl;
}
system("pause");
}
