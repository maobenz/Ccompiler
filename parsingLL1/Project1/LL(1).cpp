#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<stack>
using namespace std;

string firstSet[5][5] = { {"(","n"},{"w","+","-"},{"(","n"},{"*", "/" ,"w"},{"(","n"} };     //first��������
string followSet[5][10] = { {"$",")"} ,{"$",")"},{"$",")","+","-"},{"$",")","+","-"},{"*","/","+","-",")","$"} };    //follow��������
string Generative[10] = { "E->TG","G->+TG","G->-TG","G->w","T->FH","H->*FH","H->/FH","H->w","F->(E)","F->n"};
string analyseSheet[5][8];
//vector<vector<string> > analyseSheet[5][7];
string terminalSymbol[8] = { "n","+","-","*","/","(",")","$" };
string nonterminalSymbol[5] = { "E","G","T","H","F" };

int ifterminalSymbol(string s)   //�ж��Ƿ�Ϊ�ս��
{
	int i;
	for (i = 0;i < 8;i++)
	{
		if (terminalSymbol[i] == s)
			return i;
	}
	return -1;
}

int ifnonterminalSymbol(string s)   //�ж��Ƿ��Ƿ��ս��
{
	int i;
	for (i = 0;i < 5;i++)
	{
		if (nonterminalSymbol[i] == s)
			return i;
	}
	return -1;
}

int getlength(int result)
{
	int i;
	for (i = 0;i < 10;i++)
		if (firstSet[result][i] == "")
			return i;
}

void getfirstset(vector<string>&first, string s)    //�õ�����ʽ�ұߵ�first����
{
	int i,j;
	int flag = 0;
	int result;
	for (i = 3;i < s.length();i++)
	{
		string s3(s, i, 1);
		result = ifterminalSymbol(s3);
		if (result != -1)
		{
			first.push_back(terminalSymbol[result]);
			return;
		}
		else
		{
			result = ifnonterminalSymbol(s3);
			if (result != -1)
			{
				//int length = getlength(result);
				for (j = 0;j < 5;j++)
				{
					if (firstSet[result][j] != "w"&&firstSet[result][j] != "")
						first.push_back(firstSet[result][j]);
					else if(firstSet[result][j]== "w")
						flag = 1;
				}
			}
			else flag = 1;
		}
		if (flag == 0)
			return;
	}
}


void produceAnalysesheet()  //����������
{
	int i,j;
	int result;
	int result1;
	string s1;
	vector<string> first;
	for (i = 0;i < 10;i++)
	{
		string s1(Generative[i], 0, 1);
		result1 = ifnonterminalSymbol(s1);
		first.clear();
		getfirstset(first, Generative[i]);
		if (first.empty())
		{
			for (j = 0;j < 5;j++)
			{
				result = ifterminalSymbol(followSet[result1][j]);
				if(result!=-1)
				analyseSheet[result1][result] = Generative[i];
			}
		}
		else
		{
			for (j = 0;j < first.size();j++)
			{
				result = ifterminalSymbol(first[j]);
				analyseSheet[result1][result] = Generative[i];
			}
		}
	}
}

void printSheet()
{
	int i, j;
	cout << "\t" ;
	for (i = 0;i < 8;i++)
		cout << terminalSymbol[i] << "\t";
	cout << endl;
	for (i = 0;i <5;i++)
	{
		cout << nonterminalSymbol[i] << "\t";
		for (j = 0;j < 8; j++)
			cout << analyseSheet[i][j]<<"\t";
		cout << endl << endl;
	}
}

void error()
{
	cout << "�������,�ַ����޷���ʶ��" << endl;
}



bool ifsame(string s,vector<string>m)
{
	for (int i = 0;i < m.size();i++)
		if (s == m[i])
			return true;
	return false;
}


int main()
{
	string str;
	int index = 0;
	int result,result1;
	string X,x1;
	string stackpre;    //��ʾ��ջ����
	string strleft;
	int i;
	
	produceAnalysesheet();
	printSheet();
	cout << "��������Ҫ�������ַ���:" << endl;
	cin >> str;
	str += '$';
	stack<string> s;
	s.push("$");
	s.push("E");
	cout << "������������:" << endl;
	cout << "����ջ" << "\t";
	cout << "����" << "\t";
	cout << "���" << "\t";
	cout << endl;
	stackpre += '$';
	stackpre += s.top();
	while (!s.empty())
	{
		X = s.top();
		strleft = str.substr(index, str.length() - index - 1);
		cout << stackpre <<  "\t";
		cout << strleft << "\t";
		result = ifterminalSymbol(X);
		string s1(str, index, 1);
		if (s1[0] >= 48 && s1[0] <= 57)
			s1 = "n";
		if (result != -1)
		{
			if (X == s1)
			{
				cout << "����" << s1 << endl;
				s.pop();
				stackpre.erase(stackpre.length() - 1, 1);
				index++;
				string s1(str, index, 1);
				if (s1[0] >= 48 && s1[0] <= 57)
					s1 = "n";
			}
			else
			{
				error();
				break;
			}
		}
		else
		{
			result = ifterminalSymbol(s1);
			result1 = ifnonterminalSymbol(X);
			
			s.pop();
			stackpre.erase(stackpre.length() - 1, 1);
			x1 = analyseSheet[result1][result];
			for (i = x1.length()-1;i > 2;i--)
			{
				string s1(x1, i, 1);
				if (s1[0] >= 48 && s1[0] <= 57)
					s1 = "n";
				if (s1 != "w")
				{
					s.push(s1);
					stackpre += s1;
				}
			}
			cout << analyseSheet[result1][result] << endl;
		}
	}
	cout << "��������ɹ����ַ����ѱ�ʶ��" << endl;
	system("PAUSE");
	
	return 0;
}

