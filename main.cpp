#include<bits/stdc++.h>
#include"head.h"
using namespace std;
int runCompil();
vector<Word> words;  //token����
vector<Quadruple> qua_list;  //��Ԫʽ����
//vector<Synbl> synbl; //���ű�
vector<vector<Synbl> > sbl(4);  //���������ű�1int2char3bool4float
vector<vector<Synbl> > vall(4); //�Ӻ������ű��ṹͬ������
vector<Arr> arrs;    //�����������
vector<Arr> sub_arrs; //�Ӻ��������
vector<int> consl; //������
vector<string> const_C; //���ַ�
vector<string> const_S; //���ַ���
vector<string> struc_symbol;   //��һ��ֵ�ǽṹ�����֣��ڶ�����ʼ�ǽṹ���б�����
//int token_i = 0;
//int qua_i;

int main()
{
    lex();
    //for(int i = 0; i < words.size(); i++)
    //    cout<<words[i].type<<" "<<words[i].value<<endl;
    if(translate()) {
        cout<<"success"<<endl;
    }
    else
        cout<<"fail"<<endl;
    runCompil();
    //assembly();
}
