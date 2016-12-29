#include"标头.h"
static int state = 1;
int location1 = 0;
char str[10000];
char strc[]="text.cpp";
char ch;
char token[25];
char know[30]="wordsequence.dat";
enum style{I,C,K,P,Ch,St,def};
static char *keyword[18] = { "int","float","char","void","if","else","switch","case","for","do","while","continue","break","default","sizeof","return","cout","cin" };/*关键字表*/
static char *delimiters[18] = { ">=","<=","==","=",">","<","+","-","*","/","{","}",",",";","(",")" ,"[","]"};/*界符表*/
char variate[16][10] = {};/*变量表*/
int serial = 0;

static struct twoele {/*二元式数据结构*/
	style kind;
	char value1[25];
	int value2;
};

static twoele TOKEN[1000];/*词法序列（二元式结构）*/

static void init_twoele(twoele*tok) {/*初始化二元式，同时将变量表初始化*/
	for (int i = 0; i < 1000; i++) {
		tok[i].kind = def;
		strcpy_s(tok[i].value1, "#");
		tok[i].value2 = 0;
	}
	for (int i = 0; i < 16; i++) strcpy_s(variate[i], "#");
	return;
}

void findvalue(char **cmp, int id) {/*输出关键字或界符*/
	cout << cmp[id - 1];
	return;
}

int tra(char*cmp) {/*查询获得单词是否为关键字，是则返回关键字序号*/
	for (int i = 0; i < 18; i++) {
		if (strcmp(keyword[i], cmp) == 0)return i + 1;
	}
	return 0;
}

void printf(twoele*tok) {/*输出单词序列*/
	cout << "类型                 值";
	cout << "            类型                 值";
	cout << "            类型                 值";
	cout << "            类型                 值" << endl;
	float ck = 10;
	for (int i = 0; i < 1000; i++) {
		if (tok[i].kind == def) break;/*单词序列为空则停止输出*/
		int j=0;
		float number=0;
		cout << '('<<i + 1<<')' ;
		if (i < 9)cout << ' ';
		switch (tok[i].kind) {
		case I:cout << "标识符           " << tok[i].value1; j=strlen(tok[i].value1); break;
		case C:cout << "常数             ";
			sscanf_s(tok[i].value1, "%f", &number);
			printf("%g", number);
			for (int f = 0;; f++) {
				if (tok[i].value1[f] == '\0')break;
				if (tok[i].value1[f] == '0'&&tok[i].value1[f + 1] == '0'&&tok[i].value1[f + 2] == '0')break;
				if (tok[i].value1[f] == '.'&&tok[i].value1[f + 1] == '0'&&tok[i].value1[f + 2] == '0')break;
				j++;
			}
			break;
		case K:cout << "关键字           "; findvalue(keyword,tok[i].value2); j = strlen(keyword[tok[i].value2-1]); break;
		case P:cout << "界符             "; findvalue(delimiters,tok[i].value2); j = strlen(delimiters[tok[i].value2 - 1]); break;
		case Ch:cout << "字符             " << tok[i].value1  ; j = strlen(tok[i].value1); break;
		case St:cout << "字符串           " << tok[i].value1; j = strlen(tok[i].value1); break;
		}
		while (j < 14) {
			cout << " ";
			j++;
		}
		if ((i+1) % 4 == 0)cout << endl;
	}
	cout << endl;
}

void addvar(char*cmp) {/*加入变量表*/
	int i = 0;
	for (; i < 16; i++) {
		if (strcmp(variate[i],"#") == 0)break;
		if (strcmp(variate[i], cmp) == 0)return;
	}
	strcpy_s(variate[i], cmp);
	return;
}

void prep(char ch) {/*预处理，过滤注释*/
	int state = 1;
	int cycle = 0;
	int back;
	if (ch == '/') {/*出现'/'则准备进行过滤注释*/
		state = 2;
		back = location1 - 1;
	}
	else {
		location1--;
		return;
	}
	ch = str[location1++];
	while (ch != EOF) {
		switch (state) {
		case 2:
			if (ch == '*')/*出现'*'则认为已进入注释字段*/
				state = 3;
			else {
				state = 1;
				cycle = 1;
			}
			break;
		case 3:if (ch == '*')/*再一次出现'*'则准备跳出注释字段*/
			state = 4; break;
		case 4:if (ch == '/')/*再一次出现'/'则跳出注释字段，否则继续进行注释过滤*/
			state = 5;
			   else
				   state = 4; break;
		case 5:cycle = 1;
		}
		if (cycle == 1)
			break;
		ch = str[location1++];
	}
	if (state == 1) {
		location1 = back;
	}
	return;
}

static void scanner() {/*词法扫描*/
	int i = 0;
	ch = str[location1++];
	for (int i = 0; i < 25; i++)
	{
		token[i] = NULL;/*将单词比较窗口清空*/
	}

	prep(ch);
	ch = str[location1++];

	while (ch == ' ') {
		ch = str[location1++];
	}

	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z')) {/*进入关键字或变量判断*/
		token[i++] = ch;
		ch = str[location1++];
		while ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z') || (ch >= '0'&&ch <= '9')||(ch=='_'))
		{
			token[i++] = ch;
			ch = str[location1++];
		}
		token[i] = '\0';
		location1--;
		TOKEN[serial].kind = I;
		if (tra(token)) {/*将已获取单词与已知关键字进行比较，否则将单词加入变量组*/
			TOKEN[serial].kind = K;
			TOKEN[serial].value2 = tra(token);
		}
		else {/*填入变量，并与变量表进行比较*/
			strcpy_s(TOKEN[serial].value1, token);
			addvar(token);
		}
		serial++;
	}
	else if (ch >= '0'&&ch <= '9') {/*出现数字*/
		int num = ch - '0';
		bool k = false;
		float dot = 1;
		ch = str[location1++];
		while ((ch >= '0'&&ch <= '9') || (ch == '.')) {
			if (ch == '.') {
				ch = str[location1++];
				k = true;
				continue;
			}
			if (k == true) {
				dot /= 10;
			}
			num = num * 10 + ch - '0';/*循环乘10并加入后入位*/
			ch = str[location1++];
		}
		float numget = (float)num;
		numget *= dot;
		location1--;
		TOKEN[serial].kind = C;
		sprintf_s(TOKEN[serial++].value1, "%f", numget);
	}
	else {
		TOKEN[serial].kind = P;
		switch (ch) {/*界符判定*/
		case '>':
			if (str[location1] == '=') {
				TOKEN[serial].value2 = 1;
				location1++;
			}
			else
				TOKEN[serial].value2 = 5;
			serial++;
			break;
		case'<':
			if (str[location1] == '=') {
				TOKEN[serial].value2 = 2;
				location1++;
			}
			else
				TOKEN[serial].value2 = 6;
			serial++;
			break;
		case'=':
			if (str[location1] == '=') {
				TOKEN[serial].value2 = 3;
				location1++;
			}
			else
				TOKEN[serial].value2 = 4;
			serial++;
			break;
		case'+':TOKEN[serial++].value2 = 7; break;
		case'-':TOKEN[serial++].value2 = 8; break;
		case'*':TOKEN[serial++].value2 = 9; break;
		case'/':TOKEN[serial++].value2 = 10; break;
		case'{':TOKEN[serial++].value2 = 11; break;
		case'}':TOKEN[serial++].value2 = 12; break;
		case',':TOKEN[serial++].value2 = 13; break;
		case';':TOKEN[serial++].value2 = 14; break;
		case'(':TOKEN[serial++].value2 = 15; break;
		case')':TOKEN[serial++].value2 = 16;  break;
		case'[':TOKEN[serial++].value2 = 17;  break;
		case']':TOKEN[serial++].value2 = 18;  break;
		case'\'': {/*字符获取*/
			TOKEN[serial].kind = Ch;
			token[i++] = str[location1++];
			token[i] = '\0';
			strcpy_s(TOKEN[serial++].value1, token);
			if (str[location1++] != '\'') {
				cout << "单词有问题，字符应为单个存在..." << endl;
				getchar();
				exit(1);
			}
		}break;
		case'"': {/*字符串获取*/
			TOKEN[serial].kind = St;
			for (; i < 25; ) {
				if (str[location1] != '"') {
					token[i++] = str[location1++];
				}
				else break;
			}
			token[i] = '\0';
			if (str[location1++] != '"') {
				cout << "单词有问题，字符串应以”结束" << endl;
				getchar();
				exit(1);
			}
			strcpy_s(TOKEN[serial++].value1, token);
		}break;
		}
	}
}

char* tokenaly() {
	system("cls");
	/*
	cout << "手动输入现有文件的位置路径:";
	cin >> strc;
	*/
	std::fstream outfile(strc, ios::out | ios::in);/*打开代码文件*/
	for (int i = 0; i < 10000; i++) {
		str[i] = NULL;
	}
	if (!outfile) {
		cout << "open error!";
		exit(1);
	}
	outfile.get(str, 10000, EOF);/*将代码内容复制进去待分析字符串*/
	outfile.close();
	init_twoele(TOKEN);
	cout << "程序代码如下：" << endl << str << endl << "分析词法序号如下:" << endl;
	while (str[location1] != NULL) {
		scanner();
	}
	printf(TOKEN);
	char* path = know; // 你要创建文件的路径
	ofstream fout(path);
	for (int i = 0; i < serial; i++) {/*输出类型值和单词序列*/
		fout << TOKEN[i].kind << endl;
		switch (TOKEN[i].kind)
		{
		case P:
		case K:fout << TOKEN[i].value2<<endl; break;
		case I:
		case Ch:
		case St:
		case C:fout << TOKEN[i].value1<<endl;
		}
	}
	cout << "输出完毕！请按任意键继续进行语法分析";
	fout.close();
	getchar();
	return know;
}
