#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<stack>
using namespace std;

string terminalSymbol[8] = { "(",")","+","-","*","/","n","$" };
string nonterminalSymbol[5] = { "E","T","F" };
string newGenerative[9] = { "S->E", "E->E+T","E->E-T","E->T","T->T*F","T->T/F","T->F","F->(E)","F->n" };
string followSet[3][10] = { {"$","+","-",")"},{"$","*","/","+","-",")"},{"$","*","/","+","-",")"} };
string AnalyseSheet[16][11];

int num = 0;   //DFA状态编号
typedef struct DFA
{
	int num;
	int pre;
	vector<string> validitems;
	vector<struct DFA*> next;
	vector<bool>position;
	vector<char>symbol;
}DFA;

vector<struct DFA*>mm;

bool ifsame(string s, vector<string>m)   //判断s是否是在m中
{
	for (int i = 0;i < m.size();i++)
		if (s == m[i])
			return true;
	return false;
}

string addPoint(string s)   //增加点
{
	s.insert(3, ".");
	return s;
}

void print(DFA*dfa)    //打印DFA
{
	int i, j;
	for (i = 0;i < dfa->validitems.size();i++)
	{
		cout << dfa->validitems[i] << endl;
	}
	cout << endl;
}

string movePoint(string s, int m)  //移动点
{
	int i;
	s.erase(m, 1);
	s.insert(m + 1, ".");
	return s;
}

int ifend(string s)   //判断是否是规约状态
{
	int i;
	for (i = 0;i < s.length();i++)
	{
		if (s[i] == '.'&&i < s.length() - 1)
			return i;
	}
	return -1;
}


void error()   //出错处理
{
	cout << "程序出错,字符串无法被识别" << endl;
}

int ifterminalSymbol(string s)   //判断是否为终结符
{
	int i;
	for (i = 0;i < 8;i++)
	{
		if (terminalSymbol[i] == s)
			return i;
	}
	return -1;
}



bool ifsameDFA(DFA*dfa,int&newnum)   //判断是否是相同的DFA
{
	int i,j,k,m;
	int flag1 = 1;
	int flag2 = 1;
	for (i = 0;i < mm.size();i++)
	{
		vector<string> s1(dfa->validitems);
		vector<string> s2(mm[i]->validitems);

		if (s1.size() != s2.size())
			continue;
		for (j = 0;j < s1.size();j++)
		{
			for (k = 0;k < s2.size();k++)
			if (s1[j] == s2[k])
			{
				s1[j] = "";
				s2[k] = "";
			}
		}
		
		for (m = 0; m < s1.size(); m++)
			if (s1[m] != "")
			{
				flag1 = 0;
			}
		for (m = 0;m < s2.size();m++)
			if (s2[m] != "")
			{
				flag2 = 0;
			}	
		if (flag1 == 1 && flag2 == 1)
		{
			newnum = i;
			return true;
		}
		
		flag1 = flag2 = 1;
	}
	return false;
}


int ifnonterminalSymbol(string s)   //判断是否是非终结符
{
	int i;
	for (i = 0;i < 5;i++)
	{
		if (nonterminalSymbol[i] == s)
			return i;
	}
	return -1;
}


DFA* findnewStart(DFA*dfa, int index)   //发现同时可以移进的项目
{
	int i,j;
	char c;
	DFA*d = new DFA();
	int result;
	string s;
	for (i = 0;i < dfa->validitems[index].length();i++)
	{
		if (dfa->validitems[index][i] == '.')
		{
			c = dfa->validitems[index][i + 1];
			dfa->validitems[index];
			
		    s=movePoint(dfa->validitems[index], i);
			d->validitems.push_back(s);
			d->position.push_back(true);
			dfa->symbol.push_back(c);
		}
	}
	for (i = 0;i < dfa->validitems.size();i++)
	{
		if (dfa->position[i])
		{
			result = ifend(dfa->validitems[i]);
			if(result!=-1)
			for (j = 0;j < dfa->validitems[i].length();j++)
			{
				if (dfa->validitems[i][j] == '.'&&dfa->validitems[i][j + 1] == c)
				{
					s=movePoint(dfa->validitems[i], j);
					d->validitems.push_back(s);
					d->position.push_back(true);
					dfa->position[i] = false;
				}
			}
		}
	}

	return d;
}

void produceDFA(DFA*dfa)   //构造DFA	
{
	DFA*t;
	string s;
	DFA*tt;
	int newnum;
	int i, j, result, k, result1, result2;
	for (i = 0;i < dfa->validitems.size();i++)
	{
		for (j = 0;j < dfa->validitems[i].length();j++)
		{
			if (dfa->validitems[i][j] == '.')
			{
				string s1(dfa->validitems[i], j + 1, 1);
				result = ifnonterminalSymbol(s1);
				if (result != -1)
				{
					for (k = 0;k < 9;k++)
					{
						string s1(newGenerative[k], 0, 1);
						result1 = ifnonterminalSymbol(s1);
						if (result == result1)
						{
							s = addPoint(newGenerative[k]);
							if (!ifsame(s, dfa->validitems))
							{
								dfa->validitems.push_back(s);
								dfa->position.push_back(true);
							}
						}
					}
				}
			}
		}
	}
	if (ifsameDFA(dfa,newnum))
	{
		for (i = 0;i < mm[dfa->pre]->next.size();i++)
		{
			if (mm[dfa->pre]->next[i] == dfa)
			{
				vector<DFA*>::iterator it1 = mm[dfa->pre]->next.begin() + i;
				mm[dfa->pre]->next.erase(it1);
				mm[dfa->pre]->next.push_back(mm[newnum]);
				break;
			}
		}
		return;
	}

	cout <<num<< endl;
	print(dfa);
	dfa->num = num ;
	num = num + 1;
	mm.push_back(dfa);
	for (i = 0;i < dfa->validitems.size();i++)
	{
		result2 = ifend(dfa->validitems[i]);
		if (result2 != -1&&dfa->position[i])
		{
			dfa->position[i] = false;
			t=findnewStart(dfa, i);
			dfa->next.push_back(t);
			t->pre = dfa->num;
			produceDFA(t);
		}
	}
	//system("PAUSE");
}

int getGenerativeIndex(string s)
{
	int i;
	for (i = 0;i < 9;i++)
	{
		if (newGenerative[i] == s)
			return i;
	}
}

int getSymbolIndex(char c,int j)
{
	int i;
	for (i = 0;i < mm[j]->symbol.size();i++)
		if (mm[j]->symbol[i] == c)
			return i;
}



void printSheet()
{
	int i, j;
	for (i = 0;i < 16;i++)
	{
		for (j = 0;j < 11;j++)
		{
			if (AnalyseSheet[i][j] == "")
				AnalyseSheet[i][j] = "error";
			cout << AnalyseSheet[i][j] << "\t";
		}
		cout << endl;
	}
}

void produceAnalyseSheet()
{
	int i,j,k,m;
	int result,result1;
	string s1, s2,s3;
	int index;
	string c;
	char c1;
	
	for (i = 0;i < mm.size();i++)
	{
		for (j = 0;j < mm[i]->validitems.size();j++)
		{
			result = ifend(mm[i]->validitems[j]);
			s1.clear();
			s2.clear();
			s3.clear();
			if (result != -1)
			{
				index = getSymbolIndex(mm[i]->validitems[j][result + 1], i);
				index = mm[i]->next[index]->num;
				c = to_string(index);
				s3 += mm[i]->validitems[j][result + 1];
				result = ifterminalSymbol(s3);
				if (result != -1)
				{
					s1 = "S" + c;
					AnalyseSheet[i][result] = s1;
				}
				else
				{
					s1 =  c;
					result = ifnonterminalSymbol(s3);
					AnalyseSheet[i][result+8] = s1;
				}
			}	
			else
			{
				s1 += 'R';
				s2 = mm[i]->validitems[j].substr(0, mm[i]->validitems[j].length() - 1);
				index = getGenerativeIndex(s2);
				c = to_string(index);
				s1 += c;
				s3 += mm[i]->validitems[j][0];
				if (s3 == "S")
				{
					AnalyseSheet[i][7] = "ACC";
				}
				else
				{
					
					result = ifnonterminalSymbol(s3);
					for (k = 0;k < 10;k++)
					{
						s3.clear();
						
						if (followSet[result][k] != "")
						{
							s3 += followSet[result][k];
							result1 = ifterminalSymbol(s3);
							AnalyseSheet[i][result1] = s1;
						}
					}
				}
			}
		}
		//printSheet();
		//system("PAUSE");
	}
	

}

int main()
{
	int index, result, result1;
	int i, j, k;
	char*end;
	int count;
	string str,strIn,strStackState,strStackSymbol;
	int flag = 0;
	DFA*dfa=new DFA();
	index = 0;
	cout << "首先构造DFA状态" << endl;
	string s;
	s = newGenerative[0];
	s = addPoint(s);
	dfa->validitems.push_back(s);
	dfa->num = num;
	dfa->position.push_back(true);
	produceDFA(dfa);
	system("PAUSE");
	produceAnalyseSheet();
	printSheet();
	
	cout << "下面用LR(0)方法分析，请输入字符串:" << endl;
	cin >> str;
	cout << "栈内容" << "\t" << "输入" << "\t" << "分析动作" << "\t" << endl;
	str += '$';
	strIn = str;

	stack<string>s1;
	stack<int> s2;
	s2.push(0);
	s1.push("");
	strStackState += "0";
	strStackSymbol += " ";
	while (1)
	{
		string s(str, index, 1);
		if (s[0] <= 57 && s[0] >= 48)
		{
			i = index + 1;
			while (1)
			{
				if (str[i] <= 57 && str[i] >= 48)
					str.erase(i, 1);
				else break;
			}
			s = "n";
		}
		result = ifterminalSymbol(s);
		result1 = s2.top();
		if (result != -1)
		{
			if (AnalyseSheet[result1][result][0] == 'S')
			{
				index++;
				s1.push(s);
				strStackSymbol += s;
				string s(AnalyseSheet[result1][result], 1, AnalyseSheet[result1][result].length() - 1);
				int x = stoi(s);
				s2.push(x);
				strStackState += s;
				strIn = strIn.substr(1, strIn.length());
				cout << strStackState << "\t" << strIn << "\t" << AnalyseSheet[result1][result] << endl;
				cout << strStackSymbol << endl;
			}
			else if (AnalyseSheet[result1][result][0] == 'R')
			{
				string s(AnalyseSheet[result1][result], 1,AnalyseSheet[result1][result].length() - 1);
				int m= stoi(s);
				count = newGenerative[m].length() - 3;
				for (i = 0;i < count;i++)
				{
					s1.pop();
					if(s2.top()/10==0)
					strStackState = strStackState.substr(0, strStackState.length() - 1);
					else
						strStackState = strStackState.substr(0, strStackState.length() - 2);
					strStackSymbol = strStackSymbol.substr(0, strStackSymbol.length() - 1);
					s2.pop();
				}
				s= newGenerative[m].substr(0, 1);
				s1.push(s);
				strStackSymbol += s;
				result1 = ifnonterminalSymbol(s);
				result1 += 8;
				m = stoi(AnalyseSheet[s2.top()][result1]);
				strStackState += AnalyseSheet[s2.top()][result1];
				s2.push(m);

				cout << strStackState << "\t" << strIn << "\t" << AnalyseSheet[result1][result] << endl;
				cout << strStackSymbol << endl;
			}
			else if (AnalyseSheet[result1][result] == "ACC")
			{
				cout << "恭喜您，分析成功" << endl;
				break;
			}
			else
			{
				error();
				break;
			}
		}
		else
		{
			error();
			break;
		}
		//system("PAUSE");
	}
	system("PAUSE");
	return 0;
}