#include<string>
#include<stack>
#include<map>
#include<iostream>
#include"NFA.h"

using namespace std;

map<char, int> in_stack;//RE入栈优先级
map<char, int> out_stack;//RE出栈优先级
map<string, string> RE;//储存RE
vector<NFA>setsofNFA;

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



void main()
{
	SetStack();
	string result;
	vector<string> inputs;

	inputs.push_back("a");
	inputs.push_back("(b|c)*");
	for (int i = 0; i < inputs.size(); i++) {
		result = TurnSuffix(inputs[i]);
		cout << result << endl;
		NFA newnfa;
		setsofNFA.push_back(newnfa.structNFA(result));
	}
	if(setsofNFA.size()>=1)
	for (int i = 1; i < setsofNFA.size(); i++)
	{
		setsofNFA[0]=linkNFA(setsofNFA[0], setsofNFA[i]);//合并简单NFA，成为复杂NFA
	}
	setsofNFA.push_back(setsofNFA[0]);
	setsofNFA[0] =mergeNFA(setsofNFA[0], setsofNFA[1]);
	system("pause");
}