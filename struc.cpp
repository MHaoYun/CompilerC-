#include<bits/stdc++.h>
#include"head.h"
using namespace std;
extern vector<Word> words;
extern vector<Quadruple> qua_list;
extern vector<vector<Synbl> > sbl;
extern vector<vector<Synbl> > vall;
extern struct Quadruple qua;
extern struct Synbl symbol;
extern int token_i;
extern int t_k;   //������ʾ�������ʽtk�е�k
extern string tk;
extern ofstream fout7;
extern vector<string> struc_symbol;   //��һ��ֵ�ǽṹ�����֣��ڶ�����ʼ�ǽṹ���б�����

 int stru()
 {
     if(words[token_i].value == "struct") {
        token_i++;
        if(id()) {
            if(words[token_i].value == "{") {
                token_i++;
                stru_v();
                if(words[token_i].value == "}") {
                    token_i++;
                    if(id()) {
                        struc_symbol.push_back(words[token_i-1].value);
                        fout7<<words[token_i-1].value<<endl;
                        if(words[token_i].value == ";") {
                            token_i++;
                            return 1;
                        }
                    }
                }
            }
        }
     }
     return 0;
 }
 int stru_v()
 {
     int token_i_tmp = token_i;
     if(type()) {
        if(id()) {
            struc_symbol.push_back(words[token_i-1].value);
            fout7<<words[token_i-1].value<<endl;
            if(words[token_i].value == ";") {
                token_i++;
                if(stru_v()) {
                    return 1;
                }
            }
            else {
                token_i = token_i_tmp;
                return 0;
            }
        }
        else {
            token_i = token_i_tmp;
            return 0;
        }
     }
     else {
        token_i = token_i_tmp;
        return 0;
     }
     return 0;
 }
