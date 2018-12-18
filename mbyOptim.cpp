#include <iostream>
#include <vector>
#include <sstream>
#include "head.h"
#include "stdio.h"
//��Ԫʽ�Ż���ش��� ����������ɵ���Ԫʽ����:qua_list
//������ͬ���͵�optdQT
//û������������Խ��õĺ��� ���Բ���
using namespace std;
extern vector<Quadruple> qua_list;
int qtOut();
int rebuildQT();
struct DAGnode
{
    int n;
    string ope="";
    string M="";
    vector<string>A;
    DAGnode *sblngs[2]= {NULL,NULL};
};
vector<Quadruple>tmpQTS;
vector<DAGnode>DAG;
vector<Quadruple>optdQT; //�Ż������Ԫʽ
int xyPos[2]= {-2,-2}; //���ڴ洢ifExists��Ѱ�ҵĶ�Ԫλ�� �ڶ�λλ-1��������Ԫ����
string divSymbls[7]= {"wh","do","we","if","el","ie","end"};
float preCompute(float a,float b,string ope)
{
    float c=0;
    switch(ope[0])
    {
    case '+':
        c=a+b;
        break;
    case '-':
        c=a-b;
        break;
    case '*':
        c=a*b;
        break;
    case '/':
        c=a/b;
        break;
    case '%':
        c=(int)a%(int)b;
        break;
    default:
        c=-19999;
    }
    return c;
}
int opType(string str)
{
    int tmp=1;
    if(str[0]<=122 && str[0]>=97)
        tmp=0;
    else if(str[0]<=90 && str[0]>=65)
        tmp=0;
    return tmp;
}
bool ifExist(string str)
{
    int i=0,j=0;
    bool exist=false;
    for(i=0; i<DAG.size() && exist==false; i++)
    {
        if(DAG[i].M==str)
        {
            exist=true;
            xyPos[0]=i;
            xyPos[1]=-1;
            break;
        }
        for(j=0; j<DAG[i].A.size(); j++)
        {
            if(DAG[i].A[j]==str)
            {
                exist=true;
                xyPos[0]=i;
                xyPos[1]=j;
                break;
            }
        }
    }
    return exist;
}
int findPos(string str) //Ѱ����Ԫ�ص�λ��
{
    int i=0,pos=-1;
    for(i=0; i<DAG.size() ; i++)
    {
        if(DAG[i].M==str)
        {
            pos=i;
            break;
        }
    }
    return pos;
}
int findOpPos(string str) //Ѱ�Ҳ�������λ��
{
    int i=0,pos=-1;
    for(i=0; i<DAG.size() ; i++)
    {
        if(DAG[i].ope==str)
        {
            pos=i;
            break;
        }
    }
    return pos;
}
int findBC(string str,DAGnode *sblngs)  //�����ж��Ƿ����A=B?Cʱ ��B��C�ĵ����ж�
{
    int i=0,pos=-2; //-1Ϊ��Ԫ�� 0�������Ǹ��� -2Ϊ������
    if(sblngs->M==str)
        pos=-1;
    else
    {
        for(i=0; i<sblngs->A.size(); i++)
        {
            if(sblngs->A[i]==str)
            {
                pos=i;
                break;
            }
        }
    }
    return pos;
}
int delA(string str)
{
    int i=0,j=0;
    for(i=0; i<DAG.size(); i++)
    {
        for(j=0; j<DAG[i].A.size(); j++)
        {
            if(DAG[i].A[j]==str)
            {
                DAG[i].A.erase(DAG[i].A.begin()+j);
                j--;
            }
        }
    }
    return 0;
}
int buildNode(string str)
{
    DAGnode tmpNode;
    tmpNode.n=DAG.size()+1;
    tmpNode.ope="";
    tmpNode.M=str;
    DAG.push_back(tmpNode);
    return tmpNode.n-1;
}
bool ifConst(string str)
{
    bool result=false;
    if( (str[0]-'0')<10 && (str[0]-'0') >0)  //����һ���������͵�һλ��Ȼ������
        result=true;
    return result;
}
int optimization()
{
    int qtI=0;//��ȡ��Ԫʽ��λ�� ��ͷ
    int i=0,j=0,pos=0; //ѭ����ʱ����
    bool exist=false;
    while(qtI<tmpQTS.size())
    {
        DAGnode tmpNode;
        exist=false;
        if(tmpQTS[qtI].s[0]=="=") //A=B �� A=C1  �淶���ڰ����Ż�ppt
        {
            exist=ifExist(tmpQTS[qtI].s[1]);
            if(!exist)  //������B��ΪB�����ڵ� Ȼ�󽫽ڵ���ջ
            {
                tmpNode.n=DAG.size()+1;
                tmpNode.ope="";
                tmpNode.M=tmpQTS[qtI].s[1];
                exist=ifExist(tmpQTS[qtI].s[3]);
                if(exist)
                    delA(tmpQTS[qtI].s[3]);
                tmpNode.A.push_back(tmpQTS[qtI].s[3]);
                DAG.push_back(tmpNode);
                tmpNode.A.clear();
            }
            else   //����B����B�ϸ���A ���Ż�
            {
                int Bpos=findPos((tmpQTS[qtI].s[1]));
                exist=ifExist(tmpQTS[qtI].s[3]);
                if(exist)
                    delA(tmpQTS[qtI].s[3]);
                DAG[Bpos].A.push_back(tmpQTS[qtI].s[3]);
            }
        }
        else if(ifConst(tmpQTS[qtI].s[1]) && ifConst(tmpQTS[qtI].s[2]))   //A=C1?C2  ���ڶ���λΪ����
        {
            if(!ifExist(tmpQTS[qtI].s[1]))
                buildNode(tmpQTS[qtI].s[1]);
            if(!ifExist(tmpQTS[qtI].s[2]))
                buildNode(tmpQTS[qtI].s[2]);
            float C,C1,C2;
            stringstream stream1;
//stream1<<tmpQTS[qtI].s[1];
            C1=atof(tmpQTS[qtI].s[1].c_str());
            //stream1<<tmpQTS[qtI].s[2];
            C2=atof(tmpQTS[qtI].s[2].c_str());
            //����C
            C=preCompute(C1,C2,tmpQTS[qtI].s[0]);
            stream1<<C;
            delA(tmpQTS[qtI].s[3]);
            //cout<<stream1.str()<<endl;
            int tPos=findPos(stream1.str());
            if(tPos==-1)
            {
                tPos=buildNode(stream1.str()); //ָ���½ڵ��λ��
            }
            DAG[tPos].A.push_back(tmpQTS[qtI].s[3]);
        }
        else if(tmpQTS[qtI].s[0].size()<=2 && opType(tmpQTS[qtI].s[0]) )  //�ݶ�ֻ�����������λ���� ֻ���Ƕ�Ԫ
        {
            if(!ifExist(tmpQTS[qtI].s[1]))
                buildNode(tmpQTS[qtI].s[1]);
            if(!ifExist(tmpQTS[qtI].s[2]))
                buildNode(tmpQTS[qtI].s[2]);
            int tPos=findOpPos(tmpQTS[qtI].s[0]);
            delA(tmpQTS[qtI].s[3]); //�������Ϊ���ӵ�����A
            if(tPos!=-1)
            {
                int j=0;
                for(j=tPos; j<DAG.size(); j++)
                {
                    if( findBC(tmpQTS[qtI].s[1],DAG[j].sblngs[0])!=-2 && findBC(tmpQTS[qtI].s[2],DAG[j].sblngs[1]) ) //�ڲ������ڵ����Դ��ֱ�ΪB��C
                    {
                        DAG[j].A.push_back(tmpQTS[qtI].s[0]); //��A������ȥ
                        tPos=-2;
                        break;
                    }
                }
            }
            if(tPos!=-2)  //δ���A���Ӳ��� ��û�з���������ʽ��
            {
                tmpNode.M=tmpQTS[qtI].s[3];
                tmpNode.ope=tmpQTS[qtI].s[0];
                tmpNode.n=DAG.size()+1;
                //B��C��λ��
                if(ifExist(tmpQTS[qtI].s[1]))
                    tmpNode.sblngs[0]=&DAG[xyPos[0]];
                if(ifExist(tmpQTS[qtI].s[2]))
                    tmpNode.sblngs[1]=&DAG[xyPos[0]];
                DAG.push_back(tmpNode);
            }
        }
        qtI++;
    }
    rebuildQT();
    //tmpQTS.clear();
    vector <Quadruple>().swap(tmpQTS);
    //tmpQTS=optdQT;
    //qtOut();
    return 0;
}
int qtOut()
{
    int i=0;
    for(i=0; i<optdQT.size(); i++)
    {
        cout<<"("<<optdQT[i].s[0]<<","<<optdQT[i].s[1]<<","<<optdQT[i].s[2]<<","<<optdQT[i].s[3]<<")"<<endl;
    }
}
int rebuildQT()  //�����Ԫʽ����ջ
{
    int i=0,j=0;
    Quadruple tmpQT;
    for(i=0; i<DAG.size(); i++)
    {
        if(DAG[i].sblngs[0]!=NULL && DAG[i].sblngs[1]!=NULL)
        {
            tmpQT.s[0]=DAG[i].ope;
            tmpQT.s[1]=DAG[i].sblngs[0]->M;
            tmpQT.s[2]=DAG[i].sblngs[1]->M;
            tmpQT.s[3]=DAG[i].M;
            optdQT.push_back(tmpQT);
        }
        else if(DAG[i].sblngs[0]!=NULL)
        {
            tmpQT.s[0]=DAG[i].ope;
            tmpQT.s[1]=DAG[i].sblngs[0]->M;
            tmpQT.s[2]="_";
            tmpQT.s[3]=DAG[i].M;
            optdQT.push_back(tmpQT);
        }
        for(j=0; j<DAG[i].A.size(); j++)
        {
            if(DAG[i].A[j][0]!='t')
            {
                tmpQT.s[0]="=";
                tmpQT.s[1]=DAG[i].M;
                tmpQT.s[2]="_";
                tmpQT.s[3]=DAG[i].A[j];
                optdQT.push_back(tmpQT);
            }
        }
    }
    vector <DAGnode>().swap(DAG);
    return 0;
}
bool ifDiv(string str)
{
    bool re=false;
    int i=0;
    for(i=0; i<7; i++) //����Ѱ��
    {
        if(divSymbls[i]==str)
        {
            re=true;
            break;
        }
    }
    return re;
}
int divBlock()
{
    int QTI=0;
    Quadruple tmpQT;
    for(QTI=0; QTI<qua_list.size(); QTI++)
    {
        tmpQT=qua_list[QTI];
        if(ifDiv(qua_list[QTI].s[0]))
        {
            if(tmpQTS.size()!=0)
                optimization();
            optdQT.push_back(tmpQT);
        }
        else
        {
            tmpQTS.push_back(tmpQT);
        }
    }
    qtOut();
    return 0;
}
