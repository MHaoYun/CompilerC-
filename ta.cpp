#include"��ͷ.h"
enum style { I, C, K, P, Ch, St, default };/*��Ԫʽ����*/
static char *keyword[18] = { "int","float","char","void","if","else","switch","case","for","do","while","continue","break","default","sizeof","return" ,"cout","cin"};/*�ؼ��ֱ�*/
static char *delimiters[18] = { ">=","<=","==","=",">","<","+","-","*","/","{","}",",",";","(",")" ,"[","]" };/*�����*/
char variate[16][15] = {};/*������*/
enum TYP{in,real,ch,b,default1};/*���ͣ�����int��float,char,bool��*/
enum CAT{f,con,t,d,v,vn,vf,default2};/*���࣬����f������c������t���ͣ�d������v������vn�����βΣ�vf����ֵ�β�*/
enum ADDR{PFINFL,LENL,VALL,default3};/*��ַ���������������¼��*/
int idlocate[16];/*��¼��ʶ���ڴ������״γ��ֵ�λ��*/
int hg = 0;

struct twoele {/*��Ԫʽ���ݽṹ*/
	style kind;
	char value1[25];
	int value2;
};
twoele tok[1000];/*�ʷ����У���Ԫʽ�ṹ��*/

struct symbol {/*���ű�*/
	char name[15];
	TYP type;
	CAT kind;
	ADDR addr;
};

struct pfinfl {/*������*/
	int level;
	int off;
	int fn;
	symbol para[5];/*������*/
	int entry;
};

struct vall {/*���¼����������ṹ*/
	char name[15];
	char name1[15];
	int low;
	int up;
	struct vall *next;
};
vall *firstnode=new vall;

struct lenl {/*���ȱ�*/
	char name[10];
	int length;
};
lenl lengt[10];

void initlenl(lenl*lengt) {
	int i = 0;
	while (i < 10) {
		lengt[i].length = -1;
		strcpy_s(lengt[i++].name, "#");
	}
	lengt[0].length = 4;
	strcpy_s(lengt[0].name, "int");
	lengt[1].length = 4;
	strcpy_s(lengt[1].name, "float");
	lengt[2].length = 1;
	strcpy_s(lengt[2].name, "char");
	return;
}

void valladd(vall* head, int t1, int t2, char*id,char*id1) {/*��ӻ��¼*/
	vall *a = new vall;
	a->low = t1;
	a->up = t2;
	strcpy_s(a->name, id);
	strcpy_s(a->name1, id1);
	a->next = NULL;
	vall *b = head;
	while (b->next != NULL)b = b->next;
	b->next = a;
	return;
}

void initvall(vall* head) {/*��ʼ�����¼*/
	head->low = 0;
	head->up = 0;
	strcpy_s(head->name1, "");
	strcpy_s(head->name, "old sp");
	head->next = NULL;
	valladd(head, 1, 1, "���ص�ַ","");
	valladd(head, 2, 2, "ȫ��display","");
	return;
}

void vallprint(vall*head) {/*��ӡ���¼��*/
	vall *pr = head;
	char s[] = "vall.dat";
	std::fstream valladd(s, ios::out | ios::in);/*�򿪻��¼�ļ�*/
	cout << "���¼������:" << endl << "����"<<'\t'<<"        "<<"��ʶ��"<<'\t'<<"      "<<"�½�洢"<<'\t'<<"�Ͻ�洢" << endl;
	while (pr!= NULL) {
		cout << pr->name;
		valladd << pr->name<<endl;
		int c = strlen(pr->name);
		while (c < 16) {
			cout << " ";
			c++;
		}
		cout << pr->name1 << '\t' << "      ";
		valladd << pr->name1 << endl;
		cout << pr->low << "      " << '\t';
		valladd << pr->low << endl;
		cout << pr->up  << endl;
		valladd << pr->up << endl;
		pr = pr->next;
	}
	valladd.close();
	return;
}

symbol environment[100];/*���ű�*/

void initsymbol(symbol*environment,int length) {
	int i = 0;
	while (i<length) {
		strcpy_s(environment[i].name, "#");
		environment[i].type = default1;
		environment[i].kind = default2;
		environment[i++].addr = default3;
	}
	return;
}

int getlen(symbol k) {
	switch (k.type) {
	case in:return 4;
	case real:return 4;
	case ch:return 1;
	}
	return 0;
}

void funcpro(pfinfl*function) {
	initsymbol(function->para, 5);
	function->fn = 0;
	function->level = 0;
	function->off = 3;
	function->entry = -1;
	int c = 0;
	if (tok[idlocate[0] + 1].kind != P) {
		cout << "����������...";
		getchar();
		exit(1);
	}
	int end=0;
	for (int i = idlocate[0] + 2;; i++) {
		if ((tok[i].kind == P) && (tok[i].value2 == 16)) {
			break;
			end = i;
		}
		else if ((tok[i].kind == P) && (tok[i].value2 == 13))continue;
		function->fn++;
		strcpy_s(function->para[c].name, tok[i + 1].value1);
		switch (tok[i].value2) {
		case 1:function->para[c].type = in; break;
		case 2:function->para[c].type = real; break;
		case 3:function->para[c].type = ch; break;
		}
		function->para[c++].kind = v;
		i++;
	}
	c = 0;
	char add[10];
	_itoa_s(function->fn, add, 10);
	valladd(firstnode, 3, 3, "��������", add);
	int st = 4;
	for (int i = function->fn; i > 0; i--) {
		if (i == function->fn) {
			valladd(firstnode, st, st - 1 + getlen(function->para[c]), "��ʽ��Ԫ", function->para[c].name);
			st = st - 1 + getlen(function->para[c]);
			continue;
		}
		valladd(firstnode, st, st - 1 + getlen(function->para[c]), "", function->para[c].name);
		st = st + getlen(function->para[c]);
	}
	valladd(firstnode, st, st++, "display��", "");
	for (int i = 0;; i++) {
		if (idlocate[i] > end) {
			end = i;
			break;
		}
		if (idlocate[i] == -1)return;
	}
	for (int i = end;; i++) {
		if (idlocate[i] == -1)break;
		int ge;
		switch (tok[idlocate[i]-1].value2) {
		case 1:
		case 2:ge = 4; break;
		case 3:ge = 1;
		}
		if (i == end) {
			valladd(firstnode, st,st+ge-1 , "�ֲ�����", tok[idlocate[i]].value1);
			st = st + ge - 1;
			continue;
		}
		valladd(firstnode, st, st + ge - 1, "", tok[idlocate[i]].value1);
		st = st + ge;
	}
	return;
}

static void init_twoele(twoele*tok) {/*��ʼ����Ԫʽ��ͬʱ���������ʼ��*/
	for (int i = 0; i < 1000; i++) {
		tok[i].kind = default;
		strcpy_s(tok[i].value1, "#");
		tok[i].value2 = 0;
	}
	for (int i = 0; i < 16; i++) strcpy_s(variate[i], "#");
	return;
}

void tableadd(char idtable[16][15],char*cmp,int loc) {/*��ʶ����Ԫ�����*/
	int i = 0;
	for (;; i++) {
		if (strcmp(idtable[i], "#") == 0)break;
		if (strcmp(idtable[i], cmp) == 0)return;
	}
	strcpy_s(idtable[i], cmp);
	idlocate[hg++] = loc;
	return;
}

void finshanalysis() {/*��ɷ��ű�*/
	for (int g = 0;; g++) {
		if (strcmp(variate[g], "#") == 0)break;
		if (g == 0)environment[g].kind = f;
		else environment[g].kind = v;
		strcpy_s(environment[g].name, variate[g]);
		if (strcmp(keyword[tok[idlocate[g] - 1].value2-1],"int")==0) {
			environment[g].type = in;
		}
		else if (strcmp(keyword[tok[idlocate[g] - 1].value2 - 1], "float") == 0) {
			environment[g].type = real;
		}
		else if (strcmp(keyword[tok[idlocate[g] - 1].value2 - 1], "char") == 0){
			environment[g].type = ch;
		}
		else if (strcmp(keyword[tok[idlocate[g] - 1].value2 - 1], "bool") == 0){
			environment[g].type = b;
		}
		else {
			cout << "��������������...";
			getchar();
			exit(1);
		}
	}
	return;
}

void pri(symbol*out,int pr) {
	if (pr == 1) {
		char s1[] = "symboltable.dat";
		std::fstream symboltable(s1, ios::out | ios::in);/*�򿪴����ļ�*/
		for (int g = 0;; g++) {
			if (out[g].type == default1)break;
			cout << out[g].name;
			symboltable << out[g].name << endl;
			int j = strlen(out[g].name);
			while (j < 10) {
				cout << " ";
				j++;
			}
			switch (out[g].type) {
			case in:cout << "int        "; symboltable << "int" << endl; break;
			case real:cout << "float      "; symboltable << "float" << endl;  break;
			case ch:cout << "char       "; symboltable << "char" << endl; break;
			}
			switch (out[g].kind) {
			case f:cout << "function  "; symboltable << "function" << endl; break;
			case v:cout << "var       "; symboltable << "var" << endl; break;
			}
			switch (out[g].addr) {
			case PFINFL:cout << "PFINFL    "; symboltable << "PFINFL" << endl; break;
			case LENL:cout << "LENL      "; symboltable << "LENL" << endl; break;
			case VALL:cout << "VALL      "; symboltable << "VALL" << endl;  break;
			default:cout << "      "; symboltable << " " << endl;
			}
			cout << endl;
		}
		symboltable.close();
	}
	else {
		for (int g = 0;; g++) {
			if (out[g].type == default1)break;
			cout << out[g].name;
			int j = strlen(out[g].name);
			while (j < 10) {
				cout << " ";
				j++;
			}
			switch (out[g].type) {
			case in:cout << "int        ";break;
			case real:cout << "float      ";   break;
			case ch:cout << "char       ";  break;
			}
			switch (out[g].kind) {
			case f:cout << "function  "; break;
			case v:cout << "var       ";  break;
			}
			switch (out[g].addr) {
			case PFINFL:cout << "PFINFL    "; break;
			case LENL:cout << "LENL      "; break;
			case VALL:cout << "VALL      ";  break;
			}
			cout << endl;
		}
	}
}

void tabprint(pfinfl*function) {/*��ӡ���ű����������¼�����ȱ�*/
	cout << "���ű���֯����" << endl;
	cout << "����      ����       ����        ��ַ" << endl;
	pri(environment,1);
	cout << "���ȱ�����:" << endl<<"����"<<'\t'<<"����"<<endl;
	for (int g = 0;; g++) {
		if (lengt[g].length == -1)break;
		cout << lengt[g].name << '\t';
		cout << lengt[g].length << endl;
	}
	cout << "����������(����������:" << endl;
	cout << "��κ�    ������     ��������    ��ڵ�ַ" << endl;
	cout << function->level << "         " << function->off << "          " << function->fn << "           " << "δ����" << endl;
	cout << "����      ����       ����        ��ַ" << endl;
	pri(function->para,0);
	vallprint(firstnode);
	return;
}

void analysistable() {
	pfinfl *func=new pfinfl;
	char sequence[] = "wordsequence.dat";
	for (int i = 0; i < 16; i++) {/*��ʼ����ʶ��λ�ü�¼*/
		idlocate[i] = -1;
	}
	std::fstream outfile(sequence, ios::out | ios::in);/*�򿪴����ļ�*/
	int i = 0;
	if (!outfile) {
		cout << "open error!";
		getchar();
		getchar();
		exit(1);
	}
	init_twoele(tok);
	while (!outfile.eof()) {
		int sty;
		outfile >> sty;
		tok[i].kind = (style)sty;
		switch (tok[i].kind) {
		case P:
		case K:outfile >> tok[i++].value2; break;
		case I: {
			outfile >> tok[i++].value1;
			tableadd(variate, tok[i-1].value1,i-1);
			break;
		}
		case Ch:
		case St:
		case C:outfile >> tok[i++].value1; break;
		}
	}
	tok[i - 1].kind = default;
	initsymbol(environment,100);
	initlenl(lengt);
	initvall(firstnode);
	finshanalysis();
	funcpro(func);
	tabprint(func);
	cout << "���ű���д��ɣ�����������Խ����м���뷭��";
	getchar();
	translate1();
	return;
}
