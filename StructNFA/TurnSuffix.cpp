#include<string>
#include<stack>
#include<map>
#include<iostream>
#include"NFA.h"

using namespace std;

map<char, int> in_stack;//RE��ջ���ȼ�
map<char, int> out_stack;//RE��ջ���ȼ�
map<string, string> RE;//����RE

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