#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<string>
#include"Lex.h"
using namespace std;


ifstream ifile;
ofstream ofile;
vector<string> actionTable;
int line = 0;
map<string, string> id2re;
map<string, string> re2ac;
int kuohao = 0;

void readFile();
int checkState(char c);
string turnRe(string str);
#define FSTART -1
#define FEND -2
#define SECOND -3
#define ERROR -4

cLex::cLex()
{
	readFile();

}


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
		ifile >> id>>re;
		ifile.get();			
		re = turnRe(re);
		id2re[id] = re;
		cout <<id<< "	"<<id2re[id] << endl;
	}//正则表达式定义读取完毕
	while (!ifile.eof())
	{
		c = ifile.get();
		state = checkState(c);
		if (state == SECOND)
			break;
		ifile.unget();
		string re, ac,str;
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

string turnRe(string re)
{
		string str = "", temp;	//str用于输出的串，temp用于提取{}和“”之间的内容
		char temp1;
		int i = 0;				//re串的下标
		int offset, intcount;
		char c = re[0];
		//map<string, string>::iterator iter;//迭代器，类似指针
		string iter;
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
					str.append(1, '|');
					before++;
				}
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
				str.append(1, '(');
				str.append(temp);
				str.append(1, ')');
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
map<char, int> in_stack;//RE入栈优先级
map<char, int> out_stack;//RE出栈优先级
void SetStack()//设置符号优先级，在中缀转后缀时使用                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
{
	in_stack.insert(make_pair('#', 0));
	in_stack.insert(make_pair('(', 1));
	in_stack.insert(make_pair('*', 7));
	in_stack.insert(make_pair('|', 3));
	in_stack.insert(make_pair('&', 5));
	in_stack.insert(make_pair(')', 8));

	out_stack.insert(make_pair('#', 0));
	out_stack.insert(make_pair('(', 8));
	out_stack.insert(make_pair('*', 6));
	out_stack.insert(make_pair('|', 2));
	out_stack.insert(make_pair('&', 4));
	out_stack.insert(make_pair(')', 1));
}

string TurnSuffix(string re)
{
	SetStack();
	stack<char> s;
	map<char, int>::iterator iter1, iter2;

	char ch = '#', ch1;
	string tmp = "";
	int i = 0;
	s.push(ch);
	ch = re[i];

	while (!s.empty() && ch != '\0')
	{
		if (ch != '*' && ch != '|' && ch != '&' && ch != '(' && ch != ')')
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

bool isSame(vector<int> v1, vector<int> v2)
{
	if (v1.size() != v2.size())
		return false;
	for (int i = 0; i < v1.size(); i++)
	{
		if (v1[i] != v2[i])
			return false;
	}
	return true;
}

void generateCode(DFA dfa) {
	initcpp();
	DFANode currentNode = dfa.ALLNode[0];
	

}
void outputcpp(string a)
{
	a += "\n";
	for (int i = 0; i < a.length(); i++)
	{
		ofile.put(a[i]);
	}
}
string  DFAToCode(DFANode node,DFA dfa)
{
	if(node.out.size()!=0)
	for (int i = 0; i < node.out.size(); i++)
	{
		string c = "if(current==" + node.out[0].first;
		c += "){";
		outputcpp(c);
		outputcpp("element+=current;");
		outputcpp("continue;");
		kuohao++;
		DFAToCode(dfa.ALLNode[node.out[i].second],dfa);
		outputcpp("}");
	}
	else
	{
		if (node.state == 1)
		{
			outputcpp("else{");
			outputcpp("result.push_back(pair<string, string>(element, id)); ");
			outputcpp("element=\"\";");
			outputcpp("continue;");
			outputcpp("}");
		}
		else
		{
			cout << "error" << endl;
			abort;
		}
	}

}



void initcpp()
{
	
	ofile.open("yylex.cpp", ios::out);
	outputcpp("vector<pair<string,string>>result");
	outputcpp("string elemnts=\"\"");
	string c="\n void main()";
	outputcpp(c);
	kuohao++;
	outputcpp(" {\n ifstream ifile;");
	outputcpp("	ifile.open(\"lex.l\", ios::in);");
	outputcpp("for(;;){");
	kuohao++;
	outputcpp("if(ifile.get()==eof) break;");

	outputcpp("char current=ifile.get();");
	ofile.close();

}

int main()
{
	//readFile();
	string re = "a(b|c)*";
	cout << TurnSuffix(re);
}
