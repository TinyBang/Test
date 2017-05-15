#ifndef PRE_H
#define PRO_H
#pragma once
#include <map>
#include <string>
#include <stack>
#include <fstream>
#include <iostream>
#include "NFA.h"
#include "DFA.h"
using namespace std;

class cLex
{
public:
	cLex();//��ʼ���򿪶����ļ�������ļ������ò��������ȼ�
	void PrintRE();//����lex��������ʽ����
	void TurnRE(string&);//����RE��'(''-'��
	void IntegrateRE(string&);//���Ϲ��򲿷�,�ӵ㣬�˴�����&����
	void LexRule();//������򲿷�
	string TurnSuffix(string re);//��REתΪ��׺���ʽ
	void GenerateCode(vector<cNode*>, map<int, string>);//��cpp������������岿��
	cNFA* GetNFA();//������ɺõ�NFA
	~cLex();//�ر��ļ�

private:
	ifstream inFile;//�ļ�����
	ofstream outFile;//�ļ����
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
	cNFA m_nfa;
};

#endif
