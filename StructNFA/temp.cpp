
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
using namespace std;

vector<pair<string, string>> lexresult;
string element="";


struct DFANode
{
	vector<int> newNode;//构成的NFANode
	vector<pair<int, int>> out;//指向的DFANode
	int state;//是否为可接受状态
	int NodeNumber;
};

class DFA
{
public:
	DFA() {};
	vector<DFANode> ALLNode;//所有DFANode的集合
	vector<int> HeadAndTail;//DFA的头和可接受状态
};
//初始化DFA

int transState(int current,char edge,DFA dfa)
{
	int des=-1;
	for (int i=0;i<dfa.ALLNode[current].out.size();i++)
	{
		if (dfa.ALLNode[current].out[i].first == edge)
			des = dfa.ALLNode[current].out[i].second;
		break;
	}
	return des;
}
bool ifAcc(int currentNode,DFA dfa)
{
	if (dfa.ALLNode[currentNode].state == 1)
		return true;
	else
		return false;
}
void main()
{
	ifstream ifile("lex.l",iostream::in);
	DFA dfa;

	int currentNode = 0;
	char current;
	string action;
	for (;;){
		if (!ifile.eof()) {
			current = ifile.get();
		}
		else
			break;
		if (current != ' '){
			element += current;
			currentNode=transState(currentNode, current, dfa);
			continue;
		}
		else{
			if (ifAcc(currentNode, dfa)){
				lexresult.push_back(pair<string, string>(element, action));
				element = "";
				continue;
			}
			else{
				cout << "error!" << endl;
				abort();
			}
		}
	}
}

