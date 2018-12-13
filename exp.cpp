#include <bits/stdc++.h>
#include"head.h"
using namespace std;
extern vector<Word> words;
extern vector<Quadruple> qua_list;
extern struct Quadruple qua;
extern int token_i;
extern int t_k;
extern string tk;

struct Syn_elem {
    string s;
    int state;
}syn_elem;
stack<Syn_elem> syn; //�﷨ջ
stack<string> sem; //����ջ
string t = "t";

void qt(string s)
{
    string a, b;
     if(!sem.empty()) {
        a = sem.top();
        sem.pop();
     }
     if(!sem.empty()) {
        b = sem.top();
        sem.pop();
     }
     tk = t+int_to_str(++t_k);
     //��Ԫʽ����
     qua.s[0] = s;
     qua.s[1] = b;
     qua.s[2] = a;
     qua.s[3] = tk;
     out_qua();
     //cout<<"("<<s<<" "<<b<<" "<<a<<" "<<tk<<")"<<endl;
     sem.push(tk);
}
bool mate(string st_top, string w)
{
    if(st_top=="i"&&((isalpha(w[0])||isdigit(w[0]))&&w!="E"&&w!="T"&&w!="F")) return true;
    if(st_top == "w1" && (w == "+" || w == "-")) return true;
    if(st_top == "w2" && (w == "*" || w == "/")) return true;
    //if(st_top == "(" && w == "(") return true;
    //if(st_top == ")" && w == ")") return true;
    //if(st_top == ";" && w == ";") return true;
    return false;
}
int action(int state, string cur_w)
{
    syn_elem.s = cur_w;
    if(state == 0) {
        if(mate("i",cur_w)) {
            syn_elem.state = 8;
            syn.push(syn_elem);
            return -2;//-2�����ƽ�
        }
        else if(cur_w == "(") {
            syn_elem.state = 9;
            syn.push(syn_elem);
            return -2;
        }
        else if(cur_w == "E") {
            syn_elem.state = 1;
            syn.push(syn_elem);
            return -2;
        }
        else if(cur_w == "T") {
            syn_elem.state = 4;
            syn.push(syn_elem);
            return -2;
        }
        else if(cur_w == "F") {
            syn_elem.state = 7;
            syn.push(syn_elem);
            return -2;
        }
    }
    else if(state == 1) {
        if(mate("w1", cur_w)) {
            syn_elem.state = 2;
            syn.push(syn_elem);
            return -2;
        }
        else if(cur_w == ";") {
            return 0;//0����ɹ���ֹ
        }
    }
    else if(state == 2) {
        if(mate("i",cur_w)) {
            syn_elem.state = 8;
            syn.push(syn_elem);
            return -2;
        }
        else if(cur_w == "(") {
            syn_elem.state = 9;
            syn.push(syn_elem);
            return -2;
        }
        else if(cur_w == "T") {
            syn_elem.state = 3;
            syn.push(syn_elem);
            return -2;
        }
        else if(cur_w == "F") {
            syn_elem.state = 7;
            syn.push(syn_elem);
            return -2;
        }
    }
    else if(state == 3) {
        if(mate("w1", cur_w) || cur_w == ")" || cur_w == ";") {
            syn.pop();
            qt(syn.top().s);
            syn.pop();
            syn.pop();
            return 1;//�����õڼ����Լ
        }
        else if(mate("w2", cur_w)) {
            syn_elem.state = 5;
            syn.push(syn_elem);
            return -2;
        }
    }
    else if(state == 4) {
        if(mate("w1", cur_w) || cur_w == ")" || cur_w == ";") {
            syn.pop();
            return 2;
        }
        else if(mate("w2", cur_w)) {
            syn_elem.state = 5;
            syn.push(syn_elem);
            return -2;
        }
    }
    else if(state == 5) {
        if(mate("i", cur_w)) {
            syn_elem.state = 8;
            syn.push(syn_elem);
            return -2;
        }
        else if(cur_w == "(") {
            syn_elem.state = 9;
            syn.push(syn_elem);
            return -2;
        }
        else if(cur_w == "F") {
            syn_elem.state = 6;
            syn.push(syn_elem);
            return -2;
        }
    }
    else if(state == 6) {
        if(mate("i",cur_w)||mate("w1",cur_w)||mate("w2",cur_w)||cur_w=="("||cur_w==")"||cur_w==";") {
            syn.pop();
            qt(syn.top().s);
            syn.pop();
            syn.pop();
            return 3;
        }
    }
    else if(state == 7) {
        if(mate("i",cur_w)||mate("w1",cur_w)||mate("w2",cur_w)||cur_w=="("||cur_w==")"||cur_w==";") {
            syn.pop();
            return 4;
        }
    }
    else if(state == 8) {
        if(mate("i",cur_w)||mate("w1",cur_w)||mate("w2",cur_w)||cur_w=="("||cur_w==")"||cur_w==";") {
            sem.push(syn.top().s);
            syn.pop();
            return 5;
        }
    }
    if(state == 9) {
        if(mate("i",cur_w)) {
            syn_elem.state = 8;
            syn.push(syn_elem);
            return -2;
        }
        else if(cur_w == "(") {
            syn_elem.state = 9;
            syn.push(syn_elem);
            return -2;
        }
        else if(cur_w == "E") {
            syn_elem.state = 10;
            syn.push(syn_elem);
            return -2;
        }
        else if(cur_w == "T") {
            syn_elem.state = 4;
            syn.push(syn_elem);
            return -2;
        }
        else if(cur_w == "F") {
            syn_elem.state = 7;
            syn.push(syn_elem);
            return -2;
        }
    }
    else if(state == 10) {
        if(mate("w1", cur_w)) {
            syn_elem.state = 2;
            syn.push(syn_elem);
            return -2;
        }
        else if(cur_w == ")") {
            syn_elem.state = 11;
            syn.push(syn_elem);
            return -2;
        }
    }
    else if(state == 11) {
        if(mate("i",cur_w)||mate("w1",cur_w)||mate("w2",cur_w)||cur_w=="("||cur_w==")"||cur_w==";") {
            for(int i = 0; i < 3; i++)
                syn.pop();
        return 6;
        }
    }
    return -1;
}
void init()
{
    while(!sem.empty())
        sem.pop();
    while(!syn.empty())
        syn.pop();      //���ջ
    syn_elem.s = "none";
    syn_elem.state = 0;
    syn.push(syn_elem);
}

int exp()
{
    int token_i_tmp = token_i;   //�������˵�����ʽ�ķ��߲�ͨ��token_i���˵�����ʱ��״̬
    init();
    int flag = 0;        //�����;����+-*/��Ϊ�Ǹ����ʽ������ֻ�Ǹ���ʶ��
    for(; ; token_i++) {
        string str = words[token_i].value;
        if(str == "+" || str == "-" || str == "*" || str == "/") {
            flag = 1;
        }
        int x = action(syn.top().state, str);
        if(x == -1) {
            token_i = token_i_tmp;
            return 0;
        }
        else if(x == 0) {
            if(flag == 0) {
                token_i = token_i_tmp;
                return 0;
            }
            return 1;
        }
        else if(x == -2)
            continue;
        else {
            token_i--;
            if(x == 1 || x == 2)
                action(syn.top().state, "E");
            else if(x == 3 || x == 4)
                action(syn.top().state, "T");
            else if(x == 5 || x == 6)
                action(syn.top().state, "F");
        }
    }
}
