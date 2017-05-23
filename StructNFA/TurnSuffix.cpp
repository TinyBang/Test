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
map<char, int> in_stack;//RE��ջ���ȼ�
map<char, int> out_stack;//RE��ջ���ȼ�
map<string, string> RE;//����RE
map<string, string> id2re;//id��re��map
map<string, string> re2ac;//������ʽ�����嶯����map
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
	//��lex.l�ļ��ָ���
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
	}//���lex.l�������岿�ֵĽ���
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
	}//������ʽ�����ȡ���
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
		size_t offset = str.find_first_of(delim);//�ҵ��������еķֽ��
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
void SetStack()//���÷������ȼ�������׺ת��׺ʱʹ��                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
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
	string str = "", temp;	//str��������Ĵ���temp������ȡ{}�͡���֮�������
	char temp1;
	int i = 0;				//re�����±�
	int offset, intcount;
	char c = re[0];
	//map<string, string>::iterator iter;//������������ָ��
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
		{//����0-3�����0123
			char before = re[i - 1];
			char after = re[i + 1];
			str.erase(str.length() - 1, 1);//ɾ��ǰһ��������default��д�ģ�
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
			offset = re.find_first_of('}', i);//��i��ʼ�������ַ����е�һ����}��ĳ���ַ�ƥ����ַ�����������λ��
			for (int j = i + 1; j<offset; j++)//��ȡ{ }�е�����
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
			i = offset;//�������Ŵ�����
			c = re[++i];
			break;
		case '"':
			offset = re.find_first_of('"', i + 1);
			temp = re.substr(i + 1, offset - i - 1);//��ȡ��������֮��Ĳ���
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
		{//����������ǵ�����ĸ�������Ǵ���tmp��
			tmp.append(1, ch);
			ch = re[++i];
		}
		else
		{
			ch1 = s.top(); //��ջ��Ԫ�ظ���ch1
			iter1 = in_stack.find(ch1);//����key,ch1����ջ���ȼ�
			iter2 = out_stack.find(ch);//��ջ���ȼ�

			int i1 = iter1->second;
			int i2 = iter2->second;

			if (i1 < i2)	//ջ�����ȼ�С��ջ�����ȼ���ch��ջ��������һ���ַ�
			{
				s.push(ch);
				ch = re[++i];
			}
			else if (i1 > i2)//ջ�����ȼ�����ջ�����ȼ�����ջ�����
			{
				s.pop();
				tmp.append(1, ch1);
			}
			else//ջ�������ȼ���ȣ���������#����ջ�������
			{
				//ch = re[i];
				if (ch == '(')///ջ���С�������ʱ�򡮣���ѹջ
				{
					s.push(ch);
				}
				else///ջ���С�������ʱ�򡮣�����ջ
				{
					s.pop();
					if (ch1 == '(')
						ch = re[++i];
				}
			}
		}
		if (!(!s.empty() && ch != '\0') && s.top() != '#')//�Ƴ�ǰ���
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
	}//�޸�b��ÿ��������
	if(a.HeadAndTail.size()>1)
	for (int i = 1; i < a.HeadAndTail.size(); i++)//��a��Tailָ��b��head
	{
		a.AllNode[a.HeadAndTail[i]].out1 = b.HeadAndTail[0];
		a.AllNode[a.HeadAndTail[i]].State = 0;//��Ӧ�Ľ���״̬��Ϊ���ɽ���
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
	}//�޸�b��ÿ��������
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
void generateCode(DFA dfa) {/*��������*/
	ofile.open("yylex.cpp", ios::out);
	initcpp(dfa);
	ofile.close();
}
string transtostringnumber(int a) {/*��ascii�������ת����string���͵����ִ�*/
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
void initcpp(DFA dfa)/*���cpp�ļ�*//*cpp�ļ�������DFA��ÿ�θ���DFA�ж�״̬*/
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
	outputcpp("vector<DFANode> ALLNode;//����DFANode�ļ���");
	outputcpp("vector<int> HeadAndTail;//DFA��ͷ�Ϳɽ���״̬");
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
	outputcpp("	cout <<\"����ʶ��Ĳ�����\" << endl;");
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
			setsofNFA[num] = linkNFA(setsofNFA[num], setsofNFA[num + 1]);//�ϲ���NFA����Ϊ����NFA
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
	cout << "����NFA��ɣ�" << endl;
	DFA dfa;
	dfa.createSymbolTable(symbolTable1);
	dfa=dfa.turnDFA(setsofNFA[0]);
	cout << "ת��DFA��ɣ�" << endl;
	generateCode(dfa);
	cout << "ת��������ɣ�" << endl;
	system("pause");
}