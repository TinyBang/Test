#include<string>
#include<stack>
#include<map>
#include<iostream>
#include"NFA.h"

using namespace std;

map<char, int> in_stack;//RE入栈优先级
map<char, int> out_stack;//RE出栈优先级
map<string, string> RE;//储存RE

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
void main()
{
	SetStack();
	string result;
	string inputstream = "a&(b|c)*";
	result = TurnSuffix(inputstream);
	cout << result << endl;
	NFA newnfa;
	newnfa.structNFA(result);
	system("pause");
}