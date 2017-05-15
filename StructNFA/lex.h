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
	cLex();//初始化打开读入文件和输出文件，设置操作符优先级
	void PrintRE();//处理lex的正规表达式部分
	void TurnRE(string&);//处理RE，'(''-'等
	void IntegrateRE(string&);//整合规则部分,加点，此处点用&代替
	void LexRule();//处理规则部分
	string TurnSuffix(string re);//将RE转为后缀表达式
	void GenerateCode(vector<cNode*>, map<int, string>);//向cpp中输出代码主体部分
	cNFA* GetNFA();//获得生成好的NFA
	~cLex();//关闭文件

private:
	ifstream inFile;//文件输入
	ofstream outFile;//文件输出
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
	cNFA m_nfa;
};

#endif
