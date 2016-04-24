#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std;
const int totKeyWords = 50;//定义关键字的数量
const int totSymbols = 50;//定义定界符的数量
#define ID 100//定义标识符
#define NUM 101//定义常数
//存放处理后的字符串
char tempstr[255] = {};
//空格标志
bool temp = false;
//临时数组
char word[255] = {};
//keyword关键字
string keyword[totKeyWords] = { "main", "if", "then", "while", "do", "static", "defualt",
"do", "int", "double", "struct", "break", "else", "long", "swtich", "case", "typedf",
"char", "return", "const", "float", "short", "continue", "for", "void", "sizeof", "include", "define"};
//部分运算符，定界符等
char symbol[totSymbols] = { '+', '-', '*', '/', '=', ';', '(', ')', '#','{','}','<','>' };
//判断是否为字母
bool IsLetter(char ch) {
	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
		return true;
	return false;
}
//判断是否为数字
bool IsDigit(char ch) {
	if (ch >= '0'&&ch <= '9')
		return true;
	return false;
}
//判断是否为定界符等
int IsSymbol(char ch) {
	for (int i = 0; i < totSymbols; i++) {
		if (ch == symbol[i])
			return i;
	}
	return -1;
}
//判断是否为关键字
int IsKeyword(string str) {
	for (int i = 0; i < totKeyWords; i++) {
		if (str == keyword[i]) {
			return i;
		}
	}
	//在关键字表中没有找到
	return -1;
}
//空格处理
void HandleSpace(char a[]) {
	int j = 0;
	memset(word, 0, 255);//需要清空，不然可能残留上次的字符串
	temp = false;
	for (int i = 0; i < strlen(a); i++) {
		if (a[i] != ' ' && a[i] != '\t' ) {
			word[j++] = a[i];
			temp = false;
		} else {
			if (!temp&&a[i] != '\t') {
				word[j++] = a[i];
				temp = true;
			}
		}
	}
}
//处理"//"注释
void prePro() {
	int j = 0;
	memset(tempstr, 0, 255);
	for (int i = 0; i < strlen(word); i++) {
		if (word[i] == '/'&&word[i + 1] == '/') {
			while (i < strlen(word)) {
				i++;
			}
		} else {
			tempstr[j++] = word[i];
		}
	}
}
int main() {
	char instr[255] = {}; //接收输入字符串
	bool isComment = false; //多行注释标志,false为未处于注释区域
	string token;//存放字符串
	char *str = NULL;//存放每行的字符串
	freopen("test.cpp", "r", stdin);
	// freopen("result.txt", "w", stdout);//输出到文件
 	int linenumer = 1;
	while (fgets(instr,100,stdin) != NULL) {
		cout << "line:" << linenumer++;
		HandleSpace(instr);
		prePro();
		str = tempstr;
		while (str != NULL) {
			//一行一行开始词法分析
			for (int i = 0; i < strlen(str); i++) {
					//注释处理开始
				if (*(str + i) == '/') {
					if (*(str + i + 1) == '*') {
						isComment = true;
					}
				}
				//注释处理: */,注释区域结束
				if (*(str + i) == '*'&&isComment) {
					if (*(str + i + 1) == '/') {
						isComment = false;
						i+=2;
					}
				}
				//标识符，关键词
				if (IsLetter(*(str + i)) && (!isComment)) {
					while (IsLetter(*(str + i)) || IsDigit(*(str + i)) || *(str + i) == '_') {
						token += *(str + i);
						i++;
					}
					if (IsKeyword(token) != -1) {
						printf("(%d,%s) ", IsKeyword(token), token.c_str());
					} else {
						// if(judge(token))//判断是否为合法变量
							printf("(%d,%s) ", ID, token.c_str());
					}
					token = "";
				}
				if (IsDigit(*(str + i)) && (!isComment)) {
					bool mistake = false;
					while (IsDigit(*(str + i)) || IsLetter(*(str + i))) {
						if(IsLetter(*(str + i))){//找出词法分析的错误定位
							mistake = true;
						}
						token += *(str + i);
						i++;
					}
					if(!mistake)
						printf("(%d,%s) ",NUM, token.c_str());
					else
						printf("(错误 ：%s 不能作为变量名)", token.c_str());
					token = "";
				}
				if (*(str + i) == '<' && (!isComment)) {
					//<,<=,!=
					if (*(str + i) == '=') {
						printf("(103,<=) ");//<=
						i++;
					}
					if (*(str + i) == '<') {//<<
						printf("(104,<<) ");
						i++;
					} else {
						printf("(%d,<) ", IsSymbol('<'));
					}
				} else if (*(str + i) == '>' && (!isComment)) {
					//>,>=
					if (*(str + i + 1) == '=') {
						printf("(105,>=) ");
					} else {
						printf("(%d,>) ", IsSymbol('<'));
					}
				} else if (*(str + i) == '=' && (!isComment)) {
					//== =
					if (*(str + i + 1) == '=') {
						printf("(106,==) ");
						i ++;
					} else {
						printf("(%d,=) ", IsSymbol('='));
					}
				} else if (*(str + i) == '+' && (!isComment)) {
					//== =
					if (*(str + i + 1) == '+') {
						printf("(107,++) ");
						i ++;
					}
					if(*(str + i + 1) == '='){
						printf("(108,+=) ");
						i ++;
					}
					else {
						printf("(%d,+) ", IsSymbol('+'));
					}
				}else if (*(str + i) == '-' && (!isComment)) {
					//== =
					if (*(str + i + 1) == '-') {
						printf("(109,--) ");
						i ++;
					}
					if(*(str + i + 1) == '='){
						printf("(110,-=) ");
						i ++;
					}
					else {
						printf("(%d,+) ", IsSymbol('+'));
					}
				}else if (IsSymbol(*(str + i)) != -1 && (!isComment)) {
					//余下定界符等
					printf("(%d,%c) ", IsSymbol(*(str + i)), *(str + i));
				}
			}
			str = NULL;
		}
		//控制输出换行
	  printf("\n");
	}
	return 0;
}
