#include"标头.h"
enum tvp { number, ch, id, undefine };
char acid[20][10] = { "+","-","*","/",">","<",">=","<=","==","=","if","else","ie","while","do","we" ,"return","cout","cin","#" };/*枚举动作*/

struct opts {/*准备优化的四元式结构*/
	int idact;
	char id1[15];
	char id2[15];
	tvp type1;
	tvp type2;
	char id3[15];
};
opts opt1[100];

struct DAG_node {/*DAG图节点及链节点*/
	char mes[15];/*节点信息*/
	tvp it;/*节点类型*/
	struct DAG_node *synonymous;
};

struct DAG {/*DAG图*/
	int ope;/*节点运算符*/
	int level;
	struct DAG *left;/*左叶子*/
	struct DAG *right;/*右叶子*/
	struct DAG_node ide;
};
/*
DAG traverse(DAG root) {
	return ;
}
*/

void DAGPRO(int low, int up) {
	DAG image[100];/*DAG图*/
	DAG *tem1[100];/*临时变量DAG*/
	DAG *tem2[100];
	for (int i = 0; i < 200; i++) {
		tem1[i] = new DAG;
		tem2[i] = new DAG;
	}
	for (int i = low; i <= up; i++) {/*产生树结点*/
		image[i].ope = opt1[i].idact;
		image[i].level = 0;
		switch (opt1[i].idact) {
		case 0:
		case 1:
		case 2:
		case 3: {/*加减乘除*/
			image[i].left = tem1[i];
			image[i].right = tem2[i];
			tem1[i]->ope = 9;
			tem2[i]->ope = 9;
			image[i].ide.it = id;
			strcpy_s(image[i].ide.mes, opt1[i].id3);
			strcpy_s(tem1[i]->ide.mes, opt1[i].id1);
			strcpy_s(tem2[i]->ide.mes, opt1[i].id2);
			switch (opt1[i].type1) {
			case number:tem1[i]->ide.it = number; break;
			case ch:tem1[i]->ide.it = ch; break;
			case id:tem1[i]->ide.it = id;
			}
			switch (opt1[i].type2) {
			case number:tem2[i]->ide.it = number; break;
			case ch:tem2[i]->ide.it = ch; break;
			case id:tem2[i]->ide.it = id;
			}
			tem1[i]->level = -1;
			tem2[i]->level = -1;
			tem1[i]->right = NULL;
			tem1[i]->left = NULL;
			tem2[i]->right = NULL;
			tem2[i]->left = NULL;
			tem1[i]->ide.synonymous = NULL;
			tem2[i]->ide.synonymous = NULL;
		}break;
		case 9: {/*等于*/
			image[i].left = NULL;
			image[i].right = NULL;
			strcpy_s(image[i].ide.mes, opt1[i].id3);
			switch (opt1[i].type1) {
			case number:image[i].ide.it = number; break;
			case ch:image[i].ide.it = ch; break;
			case id:image[i].ide.it = id;
			}
			image[i].ide.synonymous = new DAG_node;
			strcpy_s(image[i].ide.synonymous->mes, opt1[i].id1);
			image[i].ide.synonymous->it = image[i].ide.it;
			image[i].ide.synonymous->synonymous = NULL;
		}break;
		}
	}
	/*产生树*/
	for (int i = low; i <= up; i++) {/*常数合并*/
		switch (image[i].ope) {
		case 0:
		case 1:
		case 2:
		case 3: {
			if ((tem1[i]->ide.it == number) && (tem2[i]->ide.it == number)) {
				int value1 = atoi(tem1[i]->ide.mes);
				int value2 = atoi(tem2[i]->ide.mes);
				int value3 = 0;
				switch (image[i].ope) {
				case 0:value3 = value1 + value2; break;
				case 1:value3 = value1 - value2; break;
				case 2:value3 = value1*value2; break;
				case 3:value3 = value1 / value2; break;
				}
				tem1[i] = NULL;
				tem2[i] = NULL;
				image[i].ope = 9;
				image[i].left = NULL;
				image[i].right = NULL;
				image[i].ide.synonymous = new DAG_node;
				_itoa_s(value3, image[i].ide.synonymous->mes, 10);
				image[i].ide.synonymous->it = number;
				image[i].ide.synonymous->synonymous = NULL;
			}
		}
		}
	}
	for (int i = low; i <= up; i++) {/*常值表达式节省*/
		if (image[i].ope == 9) {
			if (image[i].ide.synonymous->it == id) {
				char cmp[15];
				strcpy_s(cmp, image[i].ide.synonymous->mes);
				for (int j = low; j < i; j++) {
					if (image[j].ope == 9) {
						if (strcmp(cmp, image[j].ide.mes) == 0) {
							if (image[j].ide.synonymous->it == number) {
								if (image[j].ide.mes[0] == 't') {
									image[i].ide.synonymous->it = number;
									strcpy_s(image[i].ide.synonymous->mes, image[j].ide.synonymous->mes);
									image[j].ope = -2;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	for (int i = low; i <= up; i++) {/*公共子表达式节省*/
		if ((image[i].ope >= 0) && (image[i].ope <= 3)) {
			for (int j = low; j < i; j++) {
				if (image[i].ope == image[j].ope) {
					if (strcmp(tem1[i]->ide.mes, tem1[j]->ide.mes) == 0) {
						if (strcmp(tem2[i]->ide.mes, tem2[j]->ide.mes) == 0) {
							char cmp[15];
							char change[15];
							strcpy_s(cmp, image[i].ide.mes);
							image[i].ope = -2;
							strcpy_s(change, image[j].ide.mes);
							for (int k = i + 1; k <= up; k++) {
								if (image[k].ope == 9) {
									if (strcmp(image[k].ide.synonymous->mes, cmp) == 0)strcpy_s(image[k].ide.synonymous->mes, change);
								}
							}
							break;
						}
					}
				}
			}
		}
	}
	for (int i = low; i <= up; i++) {
		if ((opt1[i].idact == 17)|(opt1[i].idact==18))continue;
		opt1[i].idact = image[i].ope;
		if (image[i].ope == -1)break;
		strcpy_s(opt1[i].id3, image[i].ide.mes);
		switch (image[i].ope) {
		case -2:break;
		case 0:
		case 1:
		case 2:
		case 3: {
			strcpy_s(opt1[i].id1, tem1[i]->ide.mes);
			strcpy_s(opt1[i].id2, tem2[i]->ide.mes);
			opt1[i].type1 = tem1[i]->ide.it;
			opt1[i].type2 = tem2[i]->ide.it;
		}break;
		case 9:
			strcpy_s(opt1[i].id1, image[i].ide.synonymous->mes);
			opt1[i].type1 = image[i].ide.synonymous->it;
		}
	}
	return;
}

void basicblockdiv() {/*基本块划分*/
	int low = 0;
	int up = 0;
	for (;;up++) {
		switch (opt1[up].idact) {
		case 10:/*if*/
		case 13:/*while*/{
			DAGPRO(low, up-2);
			if (opt1[up].idact == 13)up++;
			low = up + 1;
			up = low;
		}break;
		case 16:/*return*/
		case 11:/*else*/
		case 12:/*ie*/
		case 15:/*whileend*/{
			DAGPRO(low, up - 1);
			low = up + 1;
			up = low;
		}
		}
		if (opt1[up].idact == -1)break;
	}
	DAGPRO(low, up);
	return;
}

void optimize_s() {
	int i = 0;
	int c1 = 3;
	int c2 = 3;
	char optimize[] = "optimize.dat";
	std::fstream opt(optimize, ios::out | ios::in);/*打开四元式优化文件*/
	while (!opt.eof()) {
		opt >> opt1[i].idact;
		opt >> opt1[i].id1;
		opt >> c1;
		opt1[i].type1 = (tvp)c1;
		opt >> opt1[i].id2;
		opt >> c2;
		opt1[i].type2 = (tvp)c2;
		opt >> opt1[i++].id3;
	}
	basicblockdiv();
	char end[] = "end.dat";
	std::fstream finish(end, ios::out | ios::in);/*打开四元式完成优化文件*/
	for (int i = 0;; i++) {
		if (opt1[i].idact == -2)continue;
		finish << opt1[i].idact<<endl;
		finish << opt1[i].id1<<endl;
		finish << opt1[i].type1<<endl;
		finish << opt1[i].id2<<endl;
		finish << opt1[i].type2<<endl;
		finish << opt1[i].id3<<endl;
		if (opt1[i].idact == -1)break;
	}
	finish.close();
	cout << "中间代码优化结束，前端加优化基本完成，按下任意键以继续生成目标代码...";
	opt.close();
	getchar();
	return;
}