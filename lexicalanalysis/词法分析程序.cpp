
#include "string.h"
#include<iostream>
#include<string>
#include <fstream> 
using namespace std;
#define buffersize 2000
ifstream file;    //���������ļ� 
ofstream outfile;    //��������ļ�(Ҳ���������ж�) 
ofstream errorfile;     //�����жϳ��� 
char buffer[buffersize];    //����һ�����������������ǿ��Դ��м�ֿ�Ϊǰ�벿�ֺͺ�벿������ 
int charnum=0;   //�ַ�����
int row=1;
int rownum=1;    //���� 
int columnnum=0;   //����
int position=0;
char tokenstring[100];  //��ʶ������
int tokennum=0; 
int notenum;//ע����Ŀ
char notestring[2000]; //ע������ 
char quotationstr[2000];  //�������� 
int quotation=0;

char keyword[32][20] =
{ "include","main","int","float",
"double","char","long","bool","short",
"if","else","for","while","do","struct",
"typedef","const","default",
"return","case","switch","break",
"continue","enum","goto","sizeof",
"static","void","union","unsigned",
"signed","extern" };     //����32���ؼ���

char numstring[1000][32];
char idstring[1000][32];
int numlength=0;   //��Ӧnumstring ���鳤�� 
int idlength=0;    //��Ӧ idstring ���鳤�� 
int searchnum=0;   //�ҵ�һ��Ԫ���������е�λ�� 
int searchid=0;   // �ҵ�һ��Ԫ���������е�λ�� 

void print()     //���Ժ��� 
{
	int i;
	for(i=0;i<20;i++)
	cout<<buffer[i];
}

void cinbuffer(int start)  //��ʱ�������ļ����ݲ��ַ��뻺���� 
{
	int i=0;
	char c;
	while(i<buffersize/2&&!file.eof())
	{
		file.read(&c, 1);
		buffer[i+start]=c;
		if(c=='\n') 
		{
			rownum++;
		}
		else charnum++;
		i++;
	 } 
	 if (file.eof())
	 {
	 	buffer[start+i-1] = '\0';
	 }
} 

void getchar(char&ch)  //�ӻ������ж�ȡ�ַ� 
{
	columnnum++;
	ch = buffer[position];
    if(position==buffersize/2-1)
	{
		cinbuffer(buffersize/2);
		position++;
	}
	else if(position==buffersize-1)
	{ 
		cinbuffer(0);
		position=0;
	}
	else
	    position++;
}



void ifspace(char&c)   //�ж��Ƿ��ǿո� 
{
	if(c==' ')
	{
		getchar(c);
		ifspace(c);
	}
}

int ifletter(char ch)   //�ж��Ƿ�����ĸ 
{
	if ((ch >= 97 && ch <= 122) || (ch >= 65 && ch <= 90))
		return 1;
	else
		return 0;
}

int ifnum()     //�ж����Ӷ��ɵ������Ƿ������ɳ������߸����� 
{
	for (int i = 0; i<numlength; i++)
	{
		if (strcmp(tokenstring, numstring[i]) == 0)
		{
			searchnum = i;
			return 1;
		}
	}
	return 0;
}
void error()   //������ 
{
	outfile << "�ʷ����������ִ���:λ�ڵ�" << row << "�У���" << columnnum << "��" << endl;
	errorfile << "�ʷ����������ִ���:λ�ڵ�" << row << "�У���" << columnnum << "��" << endl;
}
int ifid()   //�ж��Ƿ��Ǳ�ʶ�� 
{
	for(int i=0;i<idlength;i++)
	{
		if(strcmp(tokenstring,idstring[i])==0)
		{
			searchid=i;
			return 1;
		}
	}
	return 0;
}

void insertnumstring()   //�жϳ���һ������֮����뵽��Ӧ��ά������б��� 
{
	int i;
	for(i=0;i<tokennum;i++)
	{
		numstring[numlength][i]=tokenstring[i];
	}
	numstring[numlength][i]='\0';
	//strcpy_s(numstring[numlength], tokenstring);
	numlength++;
}

void insertidstring()   //�жϳ��Ǳ�ʶ��֮�󣬴����ά������ 
{
	int i;
	for(i=0;i<tokennum;i++)
	{
		idstring[idlength][i]=tokenstring[i];
	}
	idstring[idlength][i]='\0';
	//strcpy_s(idstring[idlength],tokenstring);
	idlength++;
}
void cat(char ch)   //���Ӻ��� 
{
	tokenstring[tokennum]=ch;
	tokennum++;
}

void print1()   //���Ժ��� 
{
	for( int i=0;i<tokennum;i++)
	cout<<tokenstring[i];
	cout<<endl;
}

int ifkey()  //�ж��Ƿ��ǹؼ��� 
{
	for (int i = 0; i<32; i++)
	{
		if (strcmp(tokenstring, keyword[i]) == 0)
			return 1;
	}
	return 0;
}
void cin1(char&ch)  //��ȡ��һ���ַ������� 
{
	while(isdigit(ch))
	{
		cat(ch);
		getchar(ch);
	}
}

void copynote1(char&ch)  //ʶ���һ��ע�ͺ��� 
{
	while(ch!='\n')
	{
		notestring[notenum]=ch;
		getchar(ch);
		notenum++;
	}
}

void copynote2(char&ch)     //ʶ��ڶ���ע�ͺ��� 
{
	char c;
	while(1)
	{
		if(ch=='*')
		{
			getchar(c);
			if(c=='/')
			break;
			else position--;
		 } 
		 notestring[notenum]=ch;
		 getchar(ch);
		 notenum++;
	}
}

void copyquotation(char ch)  //ʶ���ע��֮��ע�ʹ�����Ӧ���� 
{
	quotationstr[quotation]=ch;
	quotation++;
}

int main()
{
	string str;
	char ch=' ';
	cout<<"���ã���ӭ���ʹ�ôʷ���������"<<endl;
	cout<<"��������������Ҫ�������ļ�����:"<<endl;
	cin>>str;
	file.open(str, ios::in);
	while(!file)
	{
		cout<<"������˼,�޷��ҵ�����Ҫ�������ļ��������������롣"<<endl;
		cin>>str;
		file.open(str, ios::in);
	}
	outfile.open("����������.txt");
	errorfile.open("�����������.txt"); 
	cinbuffer(0);   //���Ƚ��ļ����ݷ��뻺������һ��
	while(ch!='\0')
	{
		getchar(ch);
		ifspace(ch);
		switch(ch)
		{
			case'/':     //�����ж�ע�� 
				getchar(ch);
				notenum=0;
				if(ch=='/')
				{
					getchar(ch);
					copynote1(ch);
					notestring[notenum]='\0';
					outfile << "<ע��," <<notestring<<">" << endl;
				}
				else if(ch=='*')
				{
					getchar(ch);
					copynote2(ch);
					notestring[notenum]='\0';
					outfile << "<ע��," <<notestring<<">" << endl;
				}
			break;
			case'a':case'b':case'c':case'd':case'e':case'f':case'g':case'h':case'i':case'j':case'k':case'l':case'm':case'n':case'o':case'p':case'q':case'r':case's':case't':case'u':
			case'v':case'w':case'x':case'y':case'z':case'A':case'B':case'C':case'D':case'E':case'F':case'G':case'H':case'I':case'J':case'K':case'L':case'M':case'N':case'O':case'P':
			case'Q':case'R':case'S':case'T':case'U':case'V':case'W':case'X':case'Y':case'Z':case'_':  //�жϹؼ��ֱ�ʶ�� 
				tokennum=0;
				while(isdigit(ch)||ifletter(ch)||ch=='_')
				{
					cat(ch);
					getchar(ch);
					//if(ch=='\n') row--;
				}
				position--;
				columnnum--;
				tokenstring[tokennum]='\0';
				if(ifkey()==0)
				{
					if(ifid())
					{
						outfile << "<id," <<searchid<<">:"<<tokenstring << endl;
					}
					else 
					{
						insertidstring();
						outfile << "<id," <<idlength-1<<">:"<<tokenstring << endl;
					}
				}
				else
				{
					outfile << "�ؼ���:" << tokenstring << endl;
				}
				break;
			case '0':case'1':case'2':case'3':case'4':case'5':case '6':case'7':case'8':case'9':
				//�жϳ��������� 
				tokennum = 0;
				cin1(ch);
				if(ch=='.')
				{
					cat(ch);
					getchar(ch);
					if(!isdigit(ch))
					{
						error();
						break;
					}
					else cat(ch);
				}
				cin1(ch);
				if(ch=='E')
				{
					cat(ch);
					getchar(ch);
					if(ch=='+'||ch=='-')
					{
						cat(ch);
						getchar(ch);
						if(!isdigit(ch))
						{
							error();
							break;
					    }
					    else 
						cin1(ch);
					}
				}
				else if (isdigit(ch))
				cin1(ch);
				tokenstring[tokennum]='\0';
				position--;
				columnnum--;
				if(ifnum()==1)
				{
					outfile << "<num," << searchnum << ">:" << tokenstring << endl;
				}
				else 
				{
					insertnumstring();
					outfile << "<num," << numlength - 1 << ">:" << tokenstring << endl;
				}
				break;
			case'<':   //������Ǹ��ָ����ķ��� 
				getchar(ch);
				if(ch=='=')
				outfile << "<relop,<= >"<< endl;
				else if(ch=='>')
				outfile << "<relop,<> >"<<endl;
				else
				{
					outfile << "<relop,< >"<<endl;
					position--;
					columnnum--;
				}
				break;
			case'>':
				getchar(ch);
				if(ch=='=')
				outfile << "<relop,>= >"<< endl;
				else
				{
					outfile << "<relop,> >"<<endl;
					position--;
					columnnum--;
				}
				break;
			case'=':
				getchar(ch);
				if(ch=='=')
				outfile << "<relop,== >"<< endl;
				else
				{
					outfile << "<relop,= >"<< endl;
					position--;
					columnnum--;
				}
				break;
			case':':
				getchar(ch);
				if(ch=='=')
				outfile << "<assign_op, := >"<< endl;
				else 
				{
					outfile << "<:,- >"<< endl;
					position--;
					columnnum--;
				}
				break;
			case'+':
				getchar(ch);
				if(ch=='=')
				outfile<<"<assign_op, += >"<< endl;
				else if(ch=='+')
				outfile<<"<++, - >"<< endl;
				else
				{
					outfile<<"<+, - >"<< endl;
					position--;
					columnnum--;
				}
				break;
			case'-':
				getchar(ch);
				if(ch=='=')
				outfile<<"<assign_op, -= >"<< endl;
				else if(ch=='-')
				outfile<<"<--, - >"<< endl;
				else
				outfile<<"<-, - >"<< endl;
				break;
			case'*':
				getchar(ch);
				if(ch=='=')
				outfile<<"<assign_op, *= >"<< endl;
				else
				{
					outfile<<"<*, - >"<< endl;
					position--;
					columnnum--;
				}
				break;
			case'\\':
				getchar(ch);
				if(ch=='=')
				outfile<<"<assign_op, \\= >"<< endl;
				else
				{
					outfile<<"<\\, - >"<< endl;
					position--;
					columnnum--;
				}
				break;
			case'(':
				outfile<<"<(, - >"<< endl;
				break;
			case')':
				outfile<<"<) , - >"<< endl;
				break;
			case'\n':
				outfile<<"<esc,����>"<<endl;
				row++;
				columnnum=0;
				break;
			case'\t':
				outfile<<"<esc,�Ʊ�>"<<endl;
				break;
			case ';':
				outfile << "<';',->" << endl;
				break;
			case '{':
				outfile << "<'{',->" << endl;
				break;
			case '}':
				outfile << "<'}',->" << endl;
				break;
			case '[':
				outfile << "<'[',->" << endl;
				break;
			case ']':
				outfile << "<']',->" << endl;
				break;
			case ',':
				outfile << "<',',->" << endl;
				break;
			case '#':
				outfile << "<'#',->" << endl;
				break;
			case '.':
				outfile << "<'.',->" << endl;
				break;
			case '&':
			    getchar(ch);
				if(ch=='&')
				  outfile << "<logicop,and>" << endl;
				else 
				{
					position--;
					columnnum--;
					outfile << "<'&',->" << endl;
				}
				break;
			case '|':
			    getchar(ch);
				if(ch=='|')
				  outfile << "<logicop,or>" << endl;
				else 
				{
					position--;
					columnnum--;
					outfile << "<'|',->" << endl;
				}
				break;
			case '!':
			    getchar(ch);
				if(ch=='=')
				  outfile << "<relop,!=>" << endl;
				else 
				{
					position--;
					columnnum--;
					outfile<<"<'!',->" << endl;
				}
				break;
			case '?':
				outfile << "<?,->" << endl;
				break;
			case '\0':
				outfile << "<esc,NULL>" << endl;
				break;
			case'"':
				outfile <<"<\",->"<<endl;
				quotation=0;
				getchar(ch);
				while(ch!='"')
				{
					copyquotation(ch);
					getchar(ch);
				}
				quotationstr[quotation]='\0';
				outfile<<"<quotation��"<<quotationstr<<">"<<endl; 
				outfile <<"<\",->"<<endl;
				break;
			case'\'':
				outfile<<"<',->"<<endl;
				break;
			default:
				error();
				break;		
				
		}
	}
		outfile<<"����ʷ�������ϣ�����Ϊͳ�ƽ����"<<endl;
		outfile<<"�������:"<<rownum<<endl;
		outfile<<"���൥�ʸ���:"<<idlength<<endl;
		outfile<<"�ַ�����:"<<charnum<<endl;
		cout<<"��л����ʹ�ã�����ȫ���Ĵʷ������ѽ�������Ӧ��������Լ����������ļ�����ʾ��"<<endl; 
		file.close();
	    outfile.close();
	    errorfile.close();
	    
	    system("pause");
	return 0;
}
