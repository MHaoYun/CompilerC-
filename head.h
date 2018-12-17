#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED

#include<bits/stdc++.h>
using namespace std;

//token�����дʷ���Ԫ��Ϣ
struct Word
{
    string type;
    int code;
    string value;
};

//һ����Ԫʽ�����е�4��ֵ
struct Quadruple
{
    string s[4];
};

//���ű�
struct Arr
{
    string name;
    string type;
    int addr;
    int up;
    int length;
};
struct Synbl
{
    string name;
    string type;
    string cat;
    int addr;
};
/*
struct Typel  //ֻ�������飬�ṹ��Ȳ����䣬�������ͱ�û��
{
    string name;
    string typ;
    struct Arr arr;
};
*/
struct Pfinfl
{
    string name;
    int level;     //�����
    int off;        //����
    int para_num;  //��������
    int entry;
    struct Synbl para[10];
};
struct Vall
{
    int old_sp;    //sp
    int rt_addr;   //���ص�ַ
    int para_num;  //�βθ���
    vector<string> formal_para; //�β�
};


//�ʷ�����
void lex();
int ch_to_num(char ch);
int state_change(int state, char ch);
Word search(char token[]);
Word state_to_code(int state_before, char token[]);
void reset(char ch, char token[], FILE *fp, int &state, int &j);
void parse(Word word);


//�﷨����
int translate();
int fn();
int factor();
int r_value();
int rt();
int evaluation();
void fn_body();
int para();
int id();
int type();
void out_qua();
string int_to_str(int& k);

int exp();
int fn_while();
int fn_if();
int arr();

//���ű�
void fill_symbol();
void out_symbol();
int un_def();
int re_def();
void file_symbol();

#endif // HEAD_H_INCLUDED
