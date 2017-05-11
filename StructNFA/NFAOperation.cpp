#include"NFA.h"
#include"NFAOperation.h"
NFA linkNFA(NFA a, NFA b)
{
	for (int i = 0; i < b.AllNode.size(); i++)
	{
		if (b.AllNode[i].out1 != -1)
			b.AllNode[i].out1 += a.AllNode.size();
		if (b.AllNode[i].out2 != -1)
			b.AllNode[i].out2 += a.AllNode.size();
	}
	for (int i = 0; i < b.getHeadAndTail().size(); i++)
	{
		b.getHeadAndTail()[i] += a.AllNode.size();
	}//�޸�b��ÿ��������
	for (int i = 1; i < a.getHeadAndTail().size(); i++)//��a��Tailָ��b��head
	{
		a.AllNode[a.getHeadAndTail()[i]].out1 = b.getHeadAndTail()[0];
		a.AllNode[a.getHeadAndTail()[i]].State = 0;//��Ӧ�Ľ���״̬��Ϊ���ɽ���
	}
	for (int i = 0; i < b.AllNode.size(); i++)
	{
		a.AllNode.push_back(b.AllNode[i]);
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
	for (int i = 0; i < b.getHeadAndTail().size(); i++)
	{
		b.getHeadAndTail()[i] += headNFA.AllNode.size();
	}//�޸�b��ÿ��������
	headNFA.AllNode[0].out2 = b.getHeadAndTail()[0];
	for (int i = 0; i < b.AllNode.size(); i++)
	{
		headNFA.AllNode.push_back(b.AllNode[i]);
	}
	return headNFA;
}