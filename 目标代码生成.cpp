#include"标头.h"
enum type { number, ch, id, undefine };
int AL,BL,CL,DL,AH,BH,CH,DH;
char ctid[20][10] = { "+","-","*","/",">","<",">=","<=","==","=","if","else","ie","while","do","we" ,"return","cout","cin","#" };/*枚举动作*/
enum typ1{in,fl,chars,boo};
enum typ2{var,fu};
typ1 kindget(char cmp[15]);

struct actrecord {/*标识符活动记录,类型登记*/
	char id[15];
	int low;
	int up;
	typ1 s1;
	typ2 s2;
};
actrecord st1[100];

struct elem4 {/*四元式结构*/
	int idact;
	char id1[15];
	char id2[15];
	type type1;
	type type2;
	char id3[15];
};
elem4 action[100];

void actionprep() {/*四元式预处理，使之更适应生成目标代码*/
	elem4 exchange;
	cout << "下面进行四元式预处理..." << endl;
	for (int i = 0;; i++) {
		if (action[i].idact == -1)break;
		if ((action[i].idact == 10) | (action[i].idact == 13)) {
			exchange.idact = action[i].idact;
			strcpy_s(exchange.id1,action[i].id1);
			exchange.type1 = action[i].type1;
			strcpy_s(exchange.id2, action[i].id2);
			exchange.type2 = action[i].type2;
			strcpy_s(exchange.id3, action[i].id3);
			action[i].idact = action[i + 1].idact;
			strcpy_s(action[i].id1, action[i + 1].id1);
			action[i].type1 = action[i + 1].type1;
			strcpy_s(action[i].id2, action[i + 1].id2);
			action[i].type2 = action[i + 1].type2;
			strcpy_s(action[i].id3, action[i + 1].id3);
			action[i + 1].idact = exchange.idact;
			strcpy_s(action[i + 1].id1, exchange.id1);
			action[i + 1].type1 = exchange.type1;
			strcpy_s(action[i + 1].id2, exchange.id2);
			action[i + 1].type2 = exchange.type2;
			strcpy_s(action[i + 1].id3, exchange.id3);
			i++;
		}
		else if (action[i].idact <= 3) {
			exchange.idact = action[i].idact;
			strcpy_s(exchange.id1, action[i].id1);
			exchange.type1 = action[i].type1;
			strcpy_s(exchange.id2, action[i].id2);
			exchange.type2 = action[i].type2;
			strcpy_s(exchange.id3, action[i].id3);
			action[i].idact = action[i - 1].idact;
			strcpy_s(action[i].id1, action[i - 1].id1);
			action[i].type1 = action[i - 1].type1;
			strcpy_s(action[i].id2, action[i - 1].id2);
			action[i].type2 = action[i - 1].type2;
			strcpy_s(action[i].id3, action[i - 1].id3);
			action[i - 1].idact = exchange.idact;
			strcpy_s(action[i - 1].id1, exchange.id1);
			action[i - 1].type1 = exchange.type1;
			strcpy_s(action[i - 1].id2, exchange.id2);
			action[i - 1].type2 = exchange.type2;
			strcpy_s(action[i - 1].id3, exchange.id3);
		}
	}
	cout << "完成预处理的四元式如下" << endl;
	char optimize[] = "optimize.dat";
	std::fstream opt(optimize, ios::out | ios::in);/*打开四元式优化文件*/
	for (int i = 0; i < 100; i++) {
		if (action[i].idact == -1)break;
		cout << ctid[action[i].idact]; range(ctid[action[i].idact]);
		opt << action[i].idact << endl;
		cout << action[i].id1; range(action[i].id1);
		opt << action[i].id1 << endl;
		opt << action[i].type1<<endl;
		cout << action[i].id2; range(action[i].id2);
		opt << action[i].id2 << endl;
		opt << action[i].type2 << endl;
		cout << action[i].id3 << endl;
		opt << action[i].id3 << endl;
	}
	opt << -1 << endl;
	cout << "下面进行中间代码优化" << endl;
	opt.close();
	optimize_s();
	cout << "按下任意键对符号表进行预处理..." << endl;
	return;
}

void symprint() {
	for (int i = 0;; i++) {
		if (strcmp(st1[i].id, "#") == 0)break;
		cout << st1[i].id << '\t'<<"      " << st1[i].low <<" "<< '\t' <<"      "<< st1[i].up << '\t'<<"      ";
		switch (st1[i].s1) {
		case in:cout << "int" << '\t'; break;
		case fl:cout << "float" << '\t'; break;
		case chars:cout << "char" << '\t'; break;
		case boo:cout << "bool" << '\t'; break;
		}
		cout << "      ";
		switch (st1[i].s2) {
		case var:cout << "var" << '\t'; break;
		case fu:cout << "function" ; break;
		}
		cout << endl;
	}
}

void symbolprep() {/*对符号表进行预处理，为生成中间代码做准备*/
	char s1[] = "symboltable.dat";
	char s2[] = "vall.dat";
	int i = 0;
	std::fstream act1(s1, ios::out | ios::in);/*打开符号表文件*/
	std::fstream act2(s2, ios::out | ios::in);/*打开活动记录文件*/
	act1 >> st1[i].id;
	char cmp[15];
	act2 >> cmp;
	while (strcmp(st1[i].id, cmp) != 0) {
		strcpy_s(cmp, "#");
		act2 >> cmp;
	}
	act2 >> st1[i].low;
	act2 >> st1[i].up;
	strcpy_s(cmp, "");
	act1 >> cmp;
	if (strcmp(cmp, "int") == 0) st1[i].s1 = in;
	else if (strcmp(cmp, "char") == 0)st1[i].s1 = chars;
	else if (strcmp(cmp, "float") == 0)st1[i].s1 = fl;
	strcpy_s(cmp, "");
	act1 >> cmp;
	st1[i++].s2 = fu;
	while (!act1.eof()) {
		act1 >> st1[i].id;
		act2 >> cmp;
		act2 >> st1[i].low;
		act2 >> st1[i].up;
		strcpy_s(cmp, "");
		act1 >> cmp;
		if (strcmp(cmp, "int") == 0) st1[i].s1 = in;
		else if (strcmp(cmp, "char") == 0)st1[i].s1 = chars;
		else if (strcmp(cmp, "float") == 0)st1[i].s1 = fl;
		act1 >> cmp;
		st1[i++].s2 = var;
	}
	strcpy_s(st1[i-1].id, "#");
	int low=0;
	int gdot=0;
	for (int i = 0;; i++) {
		if (strcmp(st1[i].id, "#") == 0) {
			gdot = i;
			low = st1[i - 1].up + 1;
			break;
		}
	}
	for (int i = 0;; i++) {
		if (action[i].idact == -1)break;
		switch (action[i].idact) {
		case 0:
		case 1:
		case 2:
		case 3: {
			strcpy_s(st1[gdot].id, action[i].id3);
			st1[gdot].low = low;
			/*
			typ1 s=in;
			if (action[i].type1 == id) {
				s= kindget(action[i].id1);
			}
			else if (action[i].type2 == id) {
				s = kindget(action[i].id2);
			}
			switch (s) {
			case in: {
				st1[gdot].up = low + 4 - 1;
				st1[gdot].s1 = in;
			}break;
			case fl: {
				st1[gdot].up = low + 8 - 1;
				st1[gdot].s1 = fl;
			}
			}
			*/
			st1[gdot].up = low + 4 - 1;
			st1[gdot].s1 = in;
			st1[gdot].s2 = var;
			low = st1[gdot++].up+1;
		}break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8: {
			strcpy_s(st1[gdot].id, action[i].id3);
			st1[gdot].low = low;
			st1[gdot].up = low;
			st1[gdot].s1 = boo;
			st1[gdot++].s2 = var;
			low++;
		}break;
		default:;
		}
	}
	strcpy_s(st1[gdot].id, "#");
	cout << "预处理完成的目标代码符号表如下..." << endl;
	cout << "符号名        存储下界        存储上界        类型            种类" << endl;
	symprint();
	cout << "按下任意键以生成最后的目标代码..." << endl;
	getchar();
}

typ1 kindget(char cmp[15]) {
	for (int i = 0;; i++) {
		if (strcmp(st1[i].id, cmp) == 0) return st1[i].s1;
	}
}
void targetlanpro() {/*生成目标代码*/
	char target[] = "target.asm";
	char four[] = "end.dat";
	std::fstream fin(four, ios::out | ios::in);/*打开完成优化的四元式文件*/
	int w = 0;
	int w1 = 3;
	int w2 = 3;
	while (!fin.eof()) {
		fin >> action[w].idact;
		fin >> action[w].id1;
		fin >> w1;
		action[w].type1 = (type)w1;
		fin >> action[w].id2;
		fin >> w2;
		action[w].type2 = (type)w2;
		fin >> action[w++].id3;
	}
	fin.close();
	std::fstream act(target, ios::out | ios::in);/*打开目标代码文件*/
	act << "DSEG    SEGMENT  " << endl;
	for (int i = 0;; i++) {
		if (strcmp(st1[i].id, "#") == 0)break;
		if (st1[i].s2 == fu)continue;
		int s = strlen(st1[i].id);
		act << st1[i].id;
		while (s < 8) {
			act << " ";
			s++;
		}
		switch (st1[i].s1) {
		case fl:
		case in:act << "DW 0" << endl; break;
			/*
		case fl:act << "DD 0" << endl; break;
		*/
		case chars:act << "DB 0" << endl; break;
		case boo:act << "DB 0" << endl; break;
		}
	}
	act << "string  DB 'PLEASE INPUT:','$'" << endl;
	act << "cnt     DW 1000 DUP(0)" << endl;
	act << "float   DD 1000 DUP(0)" << endl;
	act << "bool    DB 1000 DUP(0)" << endl;
	act << "char    DB 1000 DUP(0)" << endl;
	int rpg = 0;
	int bat = 0;
	act << "DSEG    ENDS" << endl;
	act << "SSEG    SEGMENT STACK" << endl;
	act << "SSEG    ENDS" << endl;
	act << "CSEG    SEGMENT" << endl;
	act << "        ASSUME  CS:CSEG,DS:DSEG" << endl;
	act << "        ASSUME  SS:SSEG" << endl;
	char function[4][20];
	std::fstream fun("wordsequence.dat", ios::out | ios::in);/*打开词法序列文件*/
	fun >> function[0];
	fun >> function[1];
	fun >> function[2];
	fun >> function[3];
	fun.close();
	act << function[3] << ':' << endl;
	act << "        MOV AX,DSEG" << endl;
	act << "        MOV DS,AX" << endl;
	act << "        MOV AX,SSEG" << endl;
	act << "        MOV SS,AX" << endl;
	int ifendid = 0;
	int whileendid = 0;
	int elseid = 0;
	int doid = 0;
	int actid = 0;
	int showid = 0;
	int ifid = 0;
	int hexid = 0;
	for (int i = 0;; i++) {
		if (action[i].idact == -1)break;
		if (action[i].idact == 16)continue;
		typ1 kind1;
		kind1 = kindget(action[i].id3);
		switch (action[i].idact) {
		case 0: 
		case 1: {/*加和减比较类似，合并处理*/
			act << "        MOV AX,";
			switch (action[i].type1) {
			case number: {
				int value = atoi(action[i].id1);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 4 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}break;
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; getchar(); exit(1);
			case id:act << action[i].id1 << endl;
			}
			switch (action[i].idact) {
			case 0:act << "        ADD AX,"; break;
			case 1:act << "        SUB AX,";
			}
			switch (action[i].type2) {
			case number: {
				int value = atoi(action[i].id2);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 4 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}break;
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; getchar(); exit(1);
			case id:act << action[i].id2 << endl;
			}
			act << "        MOV " << action[i].id3 << ",AX" << endl;
		}break;
		case 2: {
			switch (action[i].type1) {
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; getchar(); exit(1);
			case id: {
				act << "        MOV SI,OFFSET " << action[i].id1 << endl;
				act << "        MOV AL,BYTE PTR [SI]" << endl;
			}break;
			case number: {
				act << "        MOV AL,";
				int value = atoi(action[i].id1);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 2 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}
			}
			switch (action[i].type2) {
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; getchar(); exit(1);
			case id: {
				act << "        MOV SI,OFFSET " << action[i].id2 << endl;
				act << "        MUL BYTE PTR [SI]" << endl;
			}break;
			case number: {
				act << "        MUL ";
				int value = atoi(action[i].id2);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 2 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}
			}
			act << "        MOV " << action[i].id3 << ",AX" << endl;
		}break;
		case 3: {
			act << "        MOV AX,";
			switch (action[i].type2) {
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; getchar(); exit(1);
			case id:act << action[i].id2 << endl; break;
			case number: {
				int value = atoi(action[i].id2);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 4 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}
			}
			switch (action[i].type1) {
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; getchar(); exit(1);
			case id: {
				act << "        MOV SI,OFFSET " << action[i].id1 << endl;
				act << "        DIV BYTE PTR [SI]" << endl;
			}break;
			case number: {
				act << "        DIV ";
				int value = atoi(action[i].id1);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 2 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}
			}
			act << "        AND AH,00H" << endl;
			act << "        MOV " << action[i].id3 << ",AX" << endl;
		}break;
		case 4: 
		case 5: 
		case 6: 
		case 7: 
		case 8: {/*五种逻辑运算符比较类似，合并处理*/
			act << "compare" << actid << ":   " << endl;
			act << "        MOV AX,";
			switch (action[i].type1) {
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; getchar(); exit(1);
			case id: {
				act << action[i].id1 << endl;
			}break;
			case number: {
				int value = atoi(action[i].id1);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 4 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}
			}
			act << "        MOV BX,";
			switch (action[i].type2) {
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; getchar(); exit(1);
			case id: {
				act << action[i].id2 << endl; 
			}break;
			case number: {
				int value = atoi(action[i].id2);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 4 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}
			}
			act << "        CMP AX,BX" << endl;
			act << "        ";
			switch (action[i].idact) {
			case 4:act << "JA "; break;
			case 5:act << "JB "; break;
			case 6:act << "JAE "; break;
			case 7:act << "JBE "; break;
			case 8:act << "JNE "; break;
			}
		}break;
		case 9: {
			switch (action[i].type1) {
			case number: {
				switch (kind1) {
				case fl:
				case in: {
					int value = atoi(action[i].id1);
					char cmp2[15];
					_itoa_s(value, cmp2, 16);
					act << "        MOV " << action[i].id3 << ",";
					int c0 = 4 - strlen(cmp2);
					while (c0 > 0) {
						act << '0';
						c0--;
					}
					act << cmp2 << 'H'<<endl;
				}break;
				case chars:cout << "数据类型不匹配，语句有问题..." << endl;getchar();exit(1);
				}
			}break;
			case ch: {
				act << "        MOV " << action[i].id3 << ",'" << action[i].id1 << "'" << endl;
			}break;
			case id:
				typ1 kind2 = kindget(action[i].id1);
				/*
				if (kind1 != kind2) {
					cout << "数据类型不匹配，语句有问题..." << endl;
					getchar();
					exit(1);
				}
				*/
				switch (kind2) {
				case in:
				case fl: {
					act << "        MOV AX," << action[i].id1 << endl;
					act << "        MOV " << action[i].id3 << ",AX" << endl;
				}break;
				case chars:
					act << "        MOV AL," << action[i].id1 << endl;
					act << "        MOV " << action[i].id3 << ",AL" << endl;
				}
			}
		}break;
		case 10: {/*if判断*/
			act << "else" << elseid << endl;
			act << "if" << ifid++ << ':' << endl;
		}break;
		case 11: {
			act << "        JMP ifend" << ifendid << endl;
			act << "else" << elseid++ << ":" << endl;
		}break;
		case 12: {/*ifend*/
			actid++;
			act << "ifend" << ifendid++ << ':' << endl;
		}break;
		case 13: {/*while循环*/
			act << "whileend" << whileendid << endl;
		}break;
		case 14: {
			act << "do" << doid++ << ":" << endl;
		}break;
		case 15: {/*whileend*/
			act << "        JMP compare" << actid++ << endl;
			act << "whileend" << whileendid++ << ":" << endl;
		}break;
		case 16: {/*return*/
			act << "        MOV AH,4CH" << endl;
			act << "        INT 21H" << endl;
		}break;
		case 17: {
			act << "        MOV CL,16" << endl;
			act << "show" << showid << ':' << endl;
			act << "        SUB CL,04" << endl;
			act << "        MOV AX,"<<action[i].id3 << endl;
			act << "        SHR AX,CL" << endl;
			act << "        AND AX,000FH" << endl;
			act << "        MOV DL,AL" << endl;
			act << "        CMP DL,10" << endl;
			act << "        JB  hex"<<hexid << endl;
			act << "        ADD DL,07H" << endl;
			act << "hex" << hexid++ <<':'<< endl;
			act << "        ADD DL,30H" << endl;
			act << "        MOV AH,02H" << endl;
			act << "        INT 21H" << endl;
			act << "        CMP CL,0" << endl;
			act << "        JNE show" << showid++<<endl;
			act << "        MOV DL,'H'" << endl;
			act << "        MOV AH,02H" << endl;
			act << "        INT 21H" << endl;
			act << "        MOV DL,0AH" << endl;
			act << "        MOV AH,02H" << endl;
			act << "        INT 21H" << endl;
			act << "        MOV DL,0dH" << endl;
			act << "        INT 21H" << endl;
		}break;
		case 18: {
			act << "        MOV DX,OFFSET string" << endl;
			act << "        MOV AH,09H" << endl;
			act << "        INT 21H" << endl;
			act << "        MOV AH,01" << endl;
			act << "        INT 21H" << endl;
			act << "        SUB AL,30H" << endl;
			act << "        AND AX,00FFH" << endl;
			act << "        MOV " << action[i].id3 << ",AX" << endl;
			act << "        MOV DL,0AH" << endl;
			act << "        MOV AH,02H" << endl;
			act << "        INT 21H" << endl;
			act << "        MOV DL,0dH" << endl;
			act << "        INT 21H" << endl;
		}
		default:;
		}
	}
	act << "CSEG    ENDS" << endl;
	act << "END     " << function[3] << endl;
	act.close();
	cout << "生成的目标代码如下所示" << endl;
	std::fstream acto(target, ios::out | ios::in);/*打开目标代码文件*/
	char stri[10000];
	for (int i = 0; i < 10000; i++) {
		stri[i] = NULL;
	}
	acto.get(stri, 10000, EOF);/*将代码内容复制进去待分析字符串*/
	acto.close();
	cout << stri;
	return;
}

void targetcode() {
	char four[] = "fourelement.dat";
	std::fstream act(four, ios::out | ios::in);/*打开四元式文件*/
	int c1 = 3;
	int c2 = 3;
	int i = 0;
	while (!act.eof()) {
		act >> action[i].idact;
		act >> action[i].id1;
		act >> c1;
		action[i].type1 = (type)c1;
		act >> action[i].id2;
		act >> c2;
		action[i].type2 = (type)c2;
		act >> action[i++].id3;
	}
	act.close();
	actionprep();
	symbolprep();
	targetlanpro();
	cout << "目标代码完成生成，按下任意键结束编译...";
	getchar();
	return;
}