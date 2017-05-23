#include<string>
#include<stack>
#include<map>
#include<vector>
#include<iostream>
#include"NFA.h"
#include"DFA.h"
#include<fstream>
#define FSTART -1
#define FEND -2
#define SECOND -3
#define ERROR -4
using namespace std;

vector<int> symbolTable1;
map<char, int> in_stack;//RE入栈优先级
map<char, int> out_stack;//RE出栈优先级
map<string, string> RE;//储存RE
map<string, string> id2re;//id与re的map
map<string, string> re2ac;//正则表达式和语义动作的map
vector<NFA>setsofNFA;
ifstream ifile;
ofstream ofile;
vector<string> actionTable;
int line = 0;

void readFile();
int checkState(char c);
string turnRe(string str);

void initcpp(DFA dfa);
void DFAToCode(DFANode,DFA);
void outputcpp(string);
void readFile() {
	ifile.open("lex.l", ios::in);
	ofile.open("yylex.cpp", ios::out);
	if (!ifile.good())
	{
		cout << "Open file error!";
		return;
	}
	//将lex.l文件分隔开
	char c = ifile.get();
	int state = checkState(c);
	if (state != FSTART)
		cout << "File Error!";
	while (!ifile.eof() && state != FEND)
	{
		c = ifile.get();
		if (c == '%') {
			state = checkState(c);
			continue;
		}
		if (c == '\n')
			line++;
		ofile.put(c);
	}//完成lex.l辅助定义部分的解析
	while (!ifile.eof())
	{
		c = ifile.get();
		state = checkState(c);
		if (state == SECOND)
			break;
		ifile.unget();
		string id, re;
		ifile >> id >> re;
		ifile.get();
		re = turnRe(re);
		id2re[id] = re;
		cout << id << "	" << id2re[id] << endl;
	}//正则表达式定义读取完毕
	while (!ifile.eof())
	{
		c = ifile.get();
		state = checkState(c);
		if (state == SECOND)
			break;
		ifile.unget();
		string re, ac, str;
		getline(ifile, str);
		if (str == "")
			continue;
		string delim = "\t";
		size_t offset = str.find_first_of(delim);//找到规则定义中的分界符
		re = str.substr(0, offset);
		while (str[offset] == ' ' || str[offset] == '\t')
			offset++;
		ac = str.substr(offset, str.size() - offset);
		re2ac[re] = ac;
		//cout << re << re2ac[re] << endl;
	}


	c = 1;
	while ((c = ifile.get()) != -1)
	{
		ofile.put(c);
	}
	ifile.close();
	ofile.close();
	//readfile done.
}
int checkState(char c)
{
	if (c == '%')
	{
		char cc = ifile.get();
		switch (cc)
		{
		case '{':
			return -1;
		case '}':
			return -2;
		case '%':
			return -3;
		default:
			ifile.unget();
			return ERROR;
		}
	}
	return 0;
}
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
bool isInTable(int a, vector<int> v1)
{
	for (int i = 0; i < v1.size(); i++)
		if (a == v1[i])
			return true;

	return false;
}
string turnRe(string re)
{
	string str = "", temp;	//str用于输出的串，temp用于提取{}和“”之间的内容
	char temp1;
	int i = 0;				//re串的下标
	int offset, intcount;
	char c = re[0];
	//map<string, string>::iterator iter;//迭代器，类似指针
	string iter;
	map<string, string >::iterator l_it;;
	while (c != '\0')
	{
		switch (c)
		{
		case '[':
			str.append(1, '(');
			c = re[++i];
			break;
		case ']':
			str.append(1, ')');
			c = re[++i];
			break;
		case '-':
		{//例如0-3，输出0123
			char before = re[i - 1];
			char after = re[i + 1];
			str.erase(str.length() - 1, 1);//删除前一个（下面default中写的）
			if (*(str.end() - 1) != '(')
				str.append(1, '|');
			while (before<after)
			{
				str.append(1, before);
				if (!isInTable(before, symbolTable1))
					symbolTable1.push_back(before);
				str.append(1, '|');
				before++;
			}
			if (!isInTable(after, symbolTable1))
				symbolTable1.push_back(after);
			str.append(1, after);
			c = re[i + 2];
			i = i + 2;
			break;
		}
		case '{':
			offset = re.find_first_of('}', i);//从i开始查找在字符串中第一个与}中某个字符匹配的字符，返回它的位置
			for (int j = i + 1; j<offset; j++)//提取{ }中的内容
			{
				temp.append(1, re[j]);
			}
			l_it = id2re.find(temp);
			if (l_it != id2re.end())
				temp = id2re[temp];
			//str.append(1, '(');
			/*while (true)
			{
			int m = temp.find('(');
			int n = temp.find(')');
			if (m != -1)
			temp.erase('(');
			}*/
			str.append(temp.substr(1, temp.size() - 2));

			//str.append(1, ')');
			temp = "";
			i = offset;//从右括号处继续
			c = re[++i];
			break;
		case '"':
			offset = re.find_first_of('"', i + 1);
			temp = re.substr(i + 1, offset - i - 1);//提取两个引号之间的部分
			str.append(1, '(');
			str.append(temp);
			str.append(1, ')');
			i = offset;
			c = re[++i];
			break;
		case '(':
			str.append(1, '$');
			str.append(1, '(');
			c = re[++i];
			break;
			//case ')':
			//c = re[++i];
			//break;
		case '\\':
			temp1 = re[++i];
			if (temp1 == 't')
				str.append(1, '\t');
			else if (temp1 == 'n')
				str.append(1, '\n');
			else
				str.append(1, temp1);
			c = re[++i];
			break;


		default:
			str.append(1, c);

			c = re[++i];
			break;
		}
	}
	return str;



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
void generateCode(DFA dfa) {/*创建代码*/
	ofile.open("yylex.cpp", ios::out);
	initcpp(dfa);
	ofile.close();
}
string transtostringnumber(int a) {/*将ascii码的数字转化成string类型的数字串*/
	int tenth = 0;
	int temp = a;
	int temptenth = 1;
	string resultstring="";
	for (;;) {
		if (temp >= 10) {
			temp /= 10;
			tenth++;
			continue;
		}
		else
			break;
	}
	for (int i = 0; i < tenth; i++) {
		temptenth *= 10;
	}
	for (int i = 0; i <=tenth; i++){
		resultstring += a / temptenth+48;
		a %= temptenth;
		temptenth /= 10;
	}
	return resultstring;
}
void initcpp(DFA dfa)/*输出cpp文件*//*cpp文件保存了DFA，每次根据DFA判定状态*/
{

	outputcpp("#include<vector>");
	outputcpp("#include<string>");
	outputcpp("#include<iostream>");
	outputcpp("#include<fstream>");
	outputcpp("using namespace std;");
	outputcpp("vector<pair<string,string>>lexresult;");
	outputcpp("string element=\"\";");
	outputcpp("struct DFANode\n{");
	outputcpp("vector<int> newNode;");
	outputcpp("vector<pair<int, int>> out;");
	outputcpp("int state;");
	outputcpp("int NodeNumber;");
	outputcpp("string action;");
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
		a += transtostringnumber(i);
		outputcpp("DFANode " + a + ";");
		string c = a;
		c += ".NodeNumber=";
		c +=transtostringnumber( dfa.ALLNode[i].NodeNumber );
		c += ";";
		outputcpp(c);
		for (int j = 0; j < dfa.ALLNode[i].out.size(); j++)
		{
			string d = a;
			d += ".out.push_back(pair<int,int>('";
			d += dfa.ALLNode[i].out[j].first;
			d += "',";
			
			d +=transtostringnumber( dfa.ALLNode[i].out[j].second);
			d += "));";
			outputcpp(d);
		}
		string e = a;
		e += ".state=";
		e += dfa.ALLNode[i].state + 48;
		e += ";";
		outputcpp(e);
		string g = a;
		
		g += ".action=\"";
		g += dfa.ALLNode[i].action;
		g += "\";";
		outputcpp(g);
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
	outputcpp("if(currentNode!=0){");
	outputcpp("		if (ifAcc(currentNode, dfa)) {");
	outputcpp("if(element!=\"\"&&element!=\" \"){");
	outputcpp("			lexresult.push_back(pair<string, string>(element, dfa.ALLNode[currentNode].action));");
	outputcpp("				element = \"\";");
	outputcpp("currentNode=0;\n}\n}");
	outputcpp("else abort();");
	outputcpp("}");
	outputcpp("break;\n}");
	outputcpp("	if (current != ' '&&current!='+'&&current!='*'&&current!='('&&current!=')'&&current!='{'&&current!='}'&&current!='\\n'&&current!='=') {");
	outputcpp("	element += current;");
	outputcpp("currentNode = transState(currentNode, current, dfa);");
	outputcpp("continue;\n  }");
	outputcpp("	else {");
	vector<pair<string, string>> actiontable;
	actiontable.push_back(pair<string,string>("+", "add"));
	actiontable.push_back(pair<string, string>("*", "mult"));
	actiontable.push_back(pair<string, string>("(", "leftbra"));
	actiontable.push_back(pair<string, string>(")", "rightbra"));
	actiontable.push_back(pair<string, string>("{", "lefthuakuohao"));
	actiontable.push_back(pair<string, string>("}", "righthuakuohao"));
	actiontable.push_back(pair<string, string>("=", "equal"));
	for (int tmp = 0; tmp < actiontable.size(); tmp++)
	{
		outputcpp("if (current=='"+actiontable[tmp].first+"'){");
		outputcpp("if(currentNode!=0){");
		outputcpp("		if (ifAcc(currentNode, dfa)) {");
		outputcpp("			lexresult.push_back(pair<string, string>(element, dfa.ALLNode[currentNode].action));");
		outputcpp("				element = \"\";");
		outputcpp("currentNode=0;\n}");
		outputcpp("	else {");
		outputcpp("	cout <<\"error!\" << endl;");
		outputcpp("abort();\n}");
		outputcpp("}");
		outputcpp("element+=current;");
		outputcpp("lexresult.push_back(pair<string, string>(element, \""+actiontable[tmp].second+"\"));");
		outputcpp("element=\"\";");
		outputcpp("continue;\n}");
	}
	outputcpp("if (current=='\\n'||current==\" \"){");
	outputcpp("		if (ifAcc(currentNode, dfa)) {");
	outputcpp("			lexresult.push_back(pair<string, string>(element, dfa.ALLNode[currentNode].action));");
	outputcpp("				element = \"\";");
	outputcpp("currentNode=0;\n}");
	outputcpp("}");
	outputcpp("	else {");
	outputcpp("	cout <<\"不能识别的操作！\" << endl;");
	outputcpp("abort();\n}");
	outputcpp("}\n}\n	for (int i = 0; i < lexresult.size(); i++){\ncout << lexresult[i].first << \",\" << lexresult[i].second << endl;\n}\nsystem(\"pause\");\n}");
}

void outputcpp(string a)
{
	a += "\n";
	for (int i = 0; i < a.length(); i++)
	{
		ofile.put(a[i]);
	}
}


void main()
{
	readFile();
	SetStack();
	string result;
	vector<string> inputs;
	map<string, string>::iterator it = id2re.begin();
	int num = 0;
	for (; it != id2re.end(); ++it)
	{
		cout << it->first;
		if (it->second.find('$') != -1)
		{
			inputs.push_back(it->second.substr(0, it->second.find('$')));
			inputs.push_back(it->second.substr(it->second.find('$') + 1, it->second.size() - 1));
			for (int i = num; i < inputs.size(); i++) {
				result = TurnSuffix(inputs[i]);
				cout << result << endl;
				NFA newnfa(it->first);
				setsofNFA.push_back(newnfa.structNFA(result));
			}
			setsofNFA[num] = linkNFA(setsofNFA[num], setsofNFA[num + 1]);//合并简单NFA，成为复杂NFA
			setsofNFA.pop_back();
			inputs.pop_back();
		}
		else
		{
			inputs.push_back(it->second);
			result = TurnSuffix(inputs[num]);
			cout << result << endl;
			NFA newnfa(it->first);
			setsofNFA.push_back(newnfa.structNFA(result));
		}
		num++;
	}
	for (int i = 1; i < setsofNFA.size(); i++)
	{
		setsofNFA[0] = mergeNFA(setsofNFA[0], setsofNFA[i]);
	}
	cout << "构建NFA完成！" << endl;
	DFA dfa;
	dfa.createSymbolTable(symbolTable1);
	dfa=dfa.turnDFA(setsofNFA[0]);
	cout << "转换DFA完成！" << endl;
	generateCode(dfa);
	cout << "转换代码完成！" << endl;
	system("pause");
}