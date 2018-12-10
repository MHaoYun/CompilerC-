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
struct Synbl
{
    string name;
    string type;
    string cat;
    string addr;
};



//�ʷ�����
void lex();
int ch_to_num(char ch);
int state_change(int state, char ch);
Word search(char token[]);
Word state_to_code(int state_before, char token[]);
void reset(char ch, char token[], FILE *fp, int &state, int &j);
void parse(Word word);
void lex();

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

#endif // HEAD_H_INCLUDED
