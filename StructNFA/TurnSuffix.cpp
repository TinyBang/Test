#include<string>
#include<stack>
#include<map>
#include<iostream>
#include"NFA.h"

using namespace std;

map<char, int> in_stack;//RE��ջ���ȼ�
map<char, int> out_stack;//RE��ջ���ȼ�
map<string, string> RE;//����RE
vector<NFA>setsofNFA;

void SetStack()//���÷������ȼ�������׺ת��׺ʱʹ��                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
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
		setsofNFA[0]=linkNFA(setsofNFA[0], setsofNFA[i]);//�ϲ���NFA����Ϊ����NFA
	}
	setsofNFA.push_back(setsofNFA[0]);
	setsofNFA[0] =mergeNFA(setsofNFA[0], setsofNFA[1]);
	system("pause");
}