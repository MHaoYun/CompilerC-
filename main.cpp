#include<bits/stdc++.h>
#include"head.h"
using namespace std;
int runCompil();
vector<Word> words;  //token����
vector<Quadruple> qua_list;  //��Ԫʽ����
//vector<Synbl> synbl; //���ű�
vector<vector<Synbl> > sbl(4);  //
vector<Arr> arrs;    //�����
vector<int> consl; //������
vector<string> const_C; //���ַ�
vector<string> const_S; //���ַ���
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
