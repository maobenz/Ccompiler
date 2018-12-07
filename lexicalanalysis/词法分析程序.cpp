
#include "string.h"
#include<iostream>
#include<string>
#include <fstream> 
using namespace std;
#define buffersize 2000
ifstream file;    //定义输入文件 
ofstream outfile;    //定义输出文件(也包括错误判断) 
ofstream errorfile;     //错误判断程序 
char buffer[buffersize];    //定义一个完整缓冲区，但是可以从中间分开为前半部分和后半部分来用 
int charnum=0;   //字符数量
int row=1;
int rownum=1;    //行数 
int columnnum=0;   //列数
int position=0;
char tokenstring[100];  //标识符数组
int tokennum=0; 
int notenum;//注释数目
char notestring[2000]; //注释数组 
char quotationstr[2000];  //引号内容 
int quotation=0;

char keyword[32][20] =
{ "include","main","int","float",
"double","char","long","bool","short",
"if","else","for","while","do","struct",
"typedef","const","default",
"return","case","switch","break",
"continue","enum","goto","sizeof",
"static","void","union","unsigned",
"signed","extern" };     //定义32个关键字

char numstring[1000][32];
char idstring[1000][32];
int numlength=0;   //相应numstring 数组长度 
int idlength=0;    //相应 idstring 数组长度 
int searchnum=0;   //找到一致元素在数组中的位置 
int searchid=0;   // 找到一致元素在数组中的位置 

void print()     //调试函数 
{
	int i;
	for(i=0;i<20;i++)
	cout<<buffer[i];
}

void cinbuffer(int start)  //此时将分析文件内容部分放入缓冲区 
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

void getchar(char&ch)  //从缓冲区中读取字符 
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



void ifspace(char&c)   //判断是否是空格 
{
	if(c==' ')
	{
		getchar(c);
		ifspace(c);
	}
}

int ifletter(char ch)   //判断是否是字母 
{
	if ((ch >= 97 && ch <= 122) || (ch >= 65 && ch <= 90))
		return 1;
	else
		return 0;
}

int ifnum()     //判断连接而成的数字是否可以组成常数或者浮点数 
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
void error()   //报错函数 
{
	outfile << "词法分析程序发现错误:位于第" << row << "行，第" << columnnum << "列" << endl;
	errorfile << "词法分析程序发现错误:位于第" << row << "行，第" << columnnum << "列" << endl;
}
int ifid()   //判断是否是标识符 
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

void insertnumstring()   //判断出是一个数字之后存入到相应二维数组进行保存 
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

void insertidstring()   //判断出是标识符之后，存入二维数组中 
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
void cat(char ch)   //连接函数 
{
	tokenstring[tokennum]=ch;
	tokennum++;
}

void print1()   //调试函数 
{
	for( int i=0;i<tokennum;i++)
	cout<<tokenstring[i];
	cout<<endl;
}

int ifkey()  //判断是否是关键字 
{
	for (int i = 0; i<32; i++)
	{
		if (strcmp(tokenstring, keyword[i]) == 0)
			return 1;
	}
	return 0;
}
void cin1(char&ch)  //读取下一个字符并连接 
{
	while(isdigit(ch))
	{
		cat(ch);
		getchar(ch);
	}
}

void copynote1(char&ch)  //识别第一种注释函数 
{
	while(ch!='\n')
	{
		notestring[notenum]=ch;
		getchar(ch);
		notenum++;
	}
}

void copynote2(char&ch)     //识别第二种注释函数 
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

void copyquotation(char ch)  //识别出注释之后将注释存入相应数组 
{
	quotationstr[quotation]=ch;
	quotation++;
}

int main()
{
	string str;
	char ch=' ';
	cout<<"您好，欢迎大家使用词法分析程序。"<<endl;
	cout<<"首先请大家输入想要分析的文件名称:"<<endl;
	cin>>str;
	file.open(str, ios::in);
	while(!file)
	{
		cout<<"不好意思,无法找到您想要分析的文件，请您重新输入。"<<endl;
		cin>>str;
		file.open(str, ios::in);
	}
	outfile.open("程序分析结果.txt");
	errorfile.open("错误分析程序.txt"); 
	cinbuffer(0);   //首先将文件内容放入缓冲区的一半
	while(ch!='\0')
	{
		getchar(ch);
		ifspace(ch);
		switch(ch)
		{
			case'/':     //首先判断注释 
				getchar(ch);
				notenum=0;
				if(ch=='/')
				{
					getchar(ch);
					copynote1(ch);
					notestring[notenum]='\0';
					outfile << "<注释," <<notestring<<">" << endl;
				}
				else if(ch=='*')
				{
					getchar(ch);
					copynote2(ch);
					notestring[notenum]='\0';
					outfile << "<注释," <<notestring<<">" << endl;
				}
			break;
			case'a':case'b':case'c':case'd':case'e':case'f':case'g':case'h':case'i':case'j':case'k':case'l':case'm':case'n':case'o':case'p':case'q':case'r':case's':case't':case'u':
			case'v':case'w':case'x':case'y':case'z':case'A':case'B':case'C':case'D':case'E':case'F':case'G':case'H':case'I':case'J':case'K':case'L':case'M':case'N':case'O':case'P':
			case'Q':case'R':case'S':case'T':case'U':case'V':case'W':case'X':case'Y':case'Z':case'_':  //判断关键字标识符 
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
					outfile << "关键字:" << tokenstring << endl;
				}
				break;
			case '0':case'1':case'2':case'3':case'4':case'5':case '6':case'7':case'8':case'9':
				//判断常数及数字 
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
			case'<':   //下面就是各种各样的符号 
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
				outfile<<"<esc,换行>"<<endl;
				row++;
				columnnum=0;
				break;
			case'\t':
				outfile<<"<esc,制表>"<<endl;
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
				outfile<<"<quotation，"<<quotationstr<<">"<<endl; 
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
		outfile<<"代码词法分析完毕，以下为统计结果："<<endl;
		outfile<<"语句行数:"<<rownum<<endl;
		outfile<<"各类单词个数:"<<idlength<<endl;
		outfile<<"字符总数:"<<charnum<<endl;
		cout<<"感谢您的使用，代码全部的词法分析已结束，相应分析结果以及错误已在文件中显示。"<<endl; 
		file.close();
	    outfile.close();
	    errorfile.close();
	    
	    system("pause");
	return 0;
}
