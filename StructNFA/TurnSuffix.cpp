#include<string>
#include<stack>
#include<map>
#include<iostream>
#include"NFA.h"
#include"DFA.h"
#include<fstream>

using namespace std;

map<char, int> in_stack;//RE入栈优先级
map<char, int> out_stack;//RE出栈优先级
map<string, string> RE;//储存RE
vector<NFA>setsofNFA;
ifstream ifile;
ofstream ofile;
int kuohao = 0;

void initcpp(DFA dfa);
void DFAToCode(DFANode,DFA);
void outputcpp(string);

 
void SetStack()//设置符号优先级，在中缀转后缀时使用                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
{
	in_stack.insert(make_pair('#', 0));
	in_stack.insert(make_pair('(', 1));
	in_stack.insert(make_pair('*', 3));
	in_stack.insert(make_pair('+', 3));
	in_stack.insert(make_pair('|', 7));
	in_stack.insert(make_pair('&', 5));
	in_stack.insert(make_pair(')', 8));
	out_stack.insert(make_pair('#', 0));
	out_stack.insert(make_pair('(', 8));
	out_stack.insert(make_pair('*', 6));
	out_stack.insert(make_pair('+', 6));
	out_stack.insert(make_pair('|', 2));
	out_stack.insert(make_pair('&', 4));
	out_stack.insert(make_pair(')', 1));
}

string TurnSuffix(string re)
{
	stack<char> s;
	map<char, int>::iterator iter1, iter2;

	char ch = '#', ch1;
	string tmp = "";
	int i = 0;
	s.push(ch);
	ch = re[i];

	while (!s.empty() && ch != '\0')
	{
		if (ch != '*' && ch != '|' && ch != '&' && ch != '(' && ch != ')'&&ch!='+')
		{//如果读到的是单词字母，把它们存在tmp中
			tmp.append(1, ch);
			ch = re[++i];
		}
		else
		{
			ch1 = s.top(); //将栈顶元素赋给ch1
			iter1 = in_stack.find(ch1);//查找key,ch1，进栈优先级
			iter2 = out_stack.find(ch);//出栈优先级

			int i1 = iter1->second;
			int i2 = iter2->second;

			if (i1 < i2)	//栈内优先级小于栈外优先级，ch进栈，读入下一个字符
			{
				s.push(ch);
				ch = re[++i];
			}
			else if (i1 > i2)//栈内优先级大于栈外优先级，退栈并输出
			{
				s.pop();
				tmp.append(1, ch1);
			}
			else//栈内外优先级相等，（，），#，退栈但不输出
			{
				//ch = re[i];
				if (ch == '(')///栈中有‘（’的时候‘（’压栈
				{
					s.push(ch);
				}
				else///栈中有‘（’的时候‘）’出栈
				{
					s.pop();
					if (ch1 == '(')
						ch = re[++i];
				}
			}
		}
		if (!(!s.empty() && ch != '\0') && s.top() != '#')//推出前检查
		{
			tmp.append(1, s.top());
		}

	}
	return tmp;
}


NFA linkNFA(NFA a, NFA b)
{
	for (int i = 0; i < b.AllNode.size(); i++)
	{
		if (b.AllNode[i].out1 != -1)
			b.AllNode[i].out1 += a.AllNode.size();
		if (b.AllNode[i].out2 != -1)
			b.AllNode[i].out2 += a.AllNode.size();
	}
	for (int i = 0; i < b.HeadAndTail.size(); i++)
	{
		b.HeadAndTail[i] += a.AllNode.size();
	}//修改b中每个点的序号
	if(a.HeadAndTail.size()>1)
	for (int i = 1; i < a.HeadAndTail.size(); i++)//把a的Tail指向b的head
	{
		a.AllNode[a.HeadAndTail[i]].out1 = b.HeadAndTail[0];
		a.AllNode[a.HeadAndTail[i]].State = 0;//对应的接受状态改为不可接受
	}
	else
	{
		a.AllNode[0].out1 = b.HeadAndTail[0];
	}
	for (int i = 0; i < b.AllNode.size(); i++)
	{
		a.AllNode.push_back(b.AllNode[i]);
	}
	for (int i = a.HeadAndTail.size(); i > 1; i--)
	{
		a.HeadAndTail.pop_back();
	}
	for (int i = 1; i < b.HeadAndTail.size(); i++)
	{
		a.HeadAndTail.push_back(b.HeadAndTail[i]);
	}
	return a;
}
NFA mergeNFA(NFA a, NFA b)
{
	NFA headNFA;
	headNFA = linkNFA(headNFA, a);
	for (int i = 0; i < b.AllNode.size(); i++)
	{
		if (b.AllNode[i].out1 != -1)
			b.AllNode[i].out1 += headNFA.AllNode.size();
		if (b.AllNode[i].out2 != -1)
			b.AllNode[i].out2 += headNFA.AllNode.size();
	}
	for (int i = 0; i < b.HeadAndTail.size(); i++)
	{
		b.HeadAndTail[i] += headNFA.AllNode.size();
	}//修改b中每个点的序号
	headNFA.AllNode[0].out2 = b.HeadAndTail[0];
	for (int i = 0; i < b.AllNode.size(); i++)
	{
		headNFA.AllNode.push_back(b.AllNode[i]);
	}
	for (int i = 1; i < b.HeadAndTail.size(); i++)
	{
		headNFA.HeadAndTail.push_back(b.HeadAndTail[i]);
	}
	return headNFA;
}
void generateCode(DFA dfa) {
	ofile.open("yylex.cpp", ios::out);
	initcpp(dfa);
	//DFANode currentNode = dfa.ALLNode[0];
	//DFAToCode(dfa.ALLNode[0], dfa);
	ofile.close();
}
void initcpp(DFA dfa)
{

	outputcpp("#include<vector>");
	outputcpp("#include<string>");
	outputcpp("#include<iostream>");
	outputcpp("#include<fstream>");
	outputcpp("using namespace std;");
	outputcpp("vector<pair<string,string>>lexresult;");
	outputcpp("string element=\"\";");
	//outputcpp("\n void main()");
//	outputcpp(" {\n ifstream ifile;");
	//outputcpp("	ifile.open(\"lex.l\", ios::in);");
	//outputcpp("for(;;){");
	//kuohao++;
//	outputcpp("if(ifile.eof()) break;");
//	outputcpp("char current=ifile.get();");
	outputcpp("struct DFANode\n{");
	outputcpp("vector<int> newNode;");
	outputcpp("vector<pair<int, int>> out;");
	outputcpp("int state;");
	outputcpp("int NodeNumber;");
	outputcpp("};");
	outputcpp("class DFA{");
	outputcpp("public:");
	outputcpp("DFA() {};");
	outputcpp("vector<DFANode> ALLNode;//所有DFANode的集合");
	outputcpp("vector<int> HeadAndTail;//DFA的头和可接受状态");
	outputcpp("};");

	outputcpp("int transState(int current, char edge, DFA dfa){");
	outputcpp("int des = -1;");
	outputcpp("for (int i = 0; i<dfa.ALLNode[current].out.size(); i++){");
	outputcpp("	if (dfa.ALLNode[current].out[i].first == edge){");
	outputcpp("des = dfa.ALLNode[current].out[i].second;");
	outputcpp("break;\n}\n}");
	outputcpp("if (des==-1) {\n cout<<\"error!\"<<endl;\nabort();\n}");
	outputcpp("return des;\n}");
	outputcpp("bool ifAcc(int currentNode, DFA dfa){");
	outputcpp("	if (dfa.ALLNode[currentNode].state == 1)");
	outputcpp("return true;");
	outputcpp("else");
	outputcpp("return false;\n}");
	outputcpp("	void main()\n{");
	outputcpp("ifstream ifile(\"test.txt\", ios::in);");
	outputcpp("DFA dfa;");
	for (int i = 0; i < dfa.ALLNode.size(); i++)
	{
		string a = "node";
		a += i + 48;
		outputcpp("DFANode " + a + ";");
		string c = a;
		c += ".NodeNumber=";
		c += dfa.ALLNode[i].NodeNumber + 48;
		c += ";";
		outputcpp(c);
		for (int j = 0; j < dfa.ALLNode[i].out.size(); j++)
		{
			string d = a;
			d += ".out.push_back(pair<int,int>('";
			d += dfa.ALLNode[i].out[j].first;
			d += "',";
			d += dfa.ALLNode[i].out[j].second + 48;
			d += "));";
			//DFANode node;
			//node.out.push_back(pair<int, int>(dfa.ALLNode[i].out[j].first, dfa.ALLNode[i].out[j].second);
			outputcpp(d);
		}
		string e = a;
		e += ".state=";
		e += dfa.ALLNode[i].state + 48;
		e += ";";
		outputcpp(e);
		string f = "dfa.ALLNode.push_back(";
		f += a;
		f += ");";
		outputcpp(f);
	}
	outputcpp("	int currentNode = 0;");
	outputcpp("char current=' ';");
	outputcpp("string action;");
	outputcpp("	for (;;) {");
	outputcpp("		current = ifile.get();");
	outputcpp("	if (current==-1) {");
	outputcpp("		if (ifAcc(currentNode, dfa)) {");
	outputcpp("if(element!=\"\"&&element!=\" \"){");
	outputcpp("			lexresult.push_back(pair<string, string>(element, action));");
	outputcpp("				element = \"\";");
	outputcpp("currentNode=0;\n}\n}");
	outputcpp("else abort();");
	outputcpp("break;\n}");
	outputcpp("	if (current != ' ') {");
	outputcpp("	element += current;");
	outputcpp("currentNode = transState(currentNode, current, dfa);");
	outputcpp("continue;\n  }");
	outputcpp("	else {");
	outputcpp("		if (ifAcc(currentNode, dfa)) {");
	outputcpp("			lexresult.push_back(pair<string, string>(element, action));");
	outputcpp("				element = \"\";");
	outputcpp("currentNode=0;");
	outputcpp("continue;\n}");
	outputcpp("	else {");
	outputcpp("	cout <<\"error!\" << endl;");
	outputcpp("abort();");	
	outputcpp("}\n}\n}	for (int i = 0; i < lexresult.size(); i++){\ncout << lexresult[i].first << \",\" << lexresult[i].second << endl;\n}\nsystem(\"pause\");\n}");
}

void outputcpp(string a)
{
	a += "\n";
	for (int i = 0; i < a.length(); i++)
	{
		ofile.put(a[i]);
	}
}
/*void  DFAToCode(DFANode node, DFA dfa)
{
	if (node.out.size() != 0)
		for (int i = 0; i < node.out.size(); i++)
		{
			if (node.out[i].second != node.NodeNumber) {
				string c="";
				if (i == 0)
					c += "if(current=='";
				else
					c += "else if(current=='";
				c += node.out[i].first;
				c += "'){";
				outputcpp(c);
				outputcpp("element+=current;");
				outputcpp("current=ifile.get();");
				kuohao++;
				DFAToCode(dfa.ALLNode[node.out[i].second], dfa);
				if (i == node.out.size() - 1 && node.state == 1)
				{
					outputcpp("else{");
					if (node.state == 1) {
						outputcpp("if(element.size()!=0){");
						outputcpp("result.push_back(pair<string, string>(element, id)); ");
						outputcpp("element=\"\";");
						outputcpp("}");
						outputcpp("else abort;");
					}
					else {
						outputcpp("result.push_back(pair<string, string>(element, id)); ");
						outputcpp("element=\"\";");
					}
					outputcpp("continue;");
					outputcpp("}");
					outputcpp("}");
				}
			}
			else
			{
				outputcpp("for(;;)");
				string a="";
				a += "if(current=='";
				a += node.out[i].first;
				a += "'){";
				outputcpp(a);
				outputcpp("element+=current;");
				outputcpp("current=ifile.get();");
				outputcpp("continue;");
				outputcpp("}");
				outputcpp("else{");
				outputcpp("result.push_back(pair<string, string>(element, id)); ");
				outputcpp("element=\"\";");
				outputcpp("break;");
				outputcpp("}");
				outputcpp("}");
			}
		}
	else
	{
		if (node.state == 1)
		{
				outputcpp("}");
		////	outputcpp("else{");
		//	outputcpp("result.push_back(pair<string, string>(element, id)); ");
		//	outputcpp("element=\"\";");
		//	outputcpp("continue;");
		//	outputcpp("}");
		}
		else
		{
			cout << "error" << endl;
			abort;
		}
	}

}*/


void main()
{
	SetStack();
	string result;
	vector<string> inputs;
	string action;
	inputs.push_back("a");
	inputs.push_back("b|c*");
	inputs.push_back("b|c+");
	vector<string>action1;
	string a = "id";
	action1.push_back(a);
	action1.push_back(a);
	a = "integer";
	action1.push_back(a);
	for (int i = 0; i < inputs.size(); i++) {
		result = TurnSuffix(inputs[i]);
		cout << result << endl;
		NFA newnfa(action1[i]);
		setsofNFA.push_back(newnfa.structNFA(result));
	}
	if(setsofNFA.size()>=1)
	for (int i = 1; i < 2; i++)
	{
		setsofNFA[0]=linkNFA(setsofNFA[0], setsofNFA[i]);//合并简单NFA，成为复杂NFA
	}
	//setsofNFA.push_back(setsofNFA[0]);
	//for (int i = 1; i < setsofNFA.size(); i++)
	//{
		setsofNFA[0] = mergeNFA(setsofNFA[0], setsofNFA[2]);
	//}
	DFA dfa;
	dfa=dfa.turnDFA(setsofNFA[0]);
//	dfa.ALLNode[3].out .push_back( pair<int, int>(80,3));
	generateCode(dfa);
	system("pause");
}