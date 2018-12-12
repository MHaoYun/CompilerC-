#include <iostream>
#include <vector>
#include "head.h"
//װ����Ԫʽת�����ش���
using namespace std;
struct SYMBL
{
    string name;
    int L;
};
struct sY
{
    string s[4];
}sY;
struct QT
{
    SYMBL s[4];
};
extern vector<Quadruple> qua_list;
vector<SYMBL>sba;  //α���ű�  //�޻�Ծ��Ϣ��Ԫʽ
vector<QT>qtS;      //�л�Ծ��Ϣ��Ԫʽ
string rGroup[3]= {"","",""}; //�Ĵ��������� ���Ĵ���
int qtPos[2];  //���ڴ���ڼĴ������Ԫ������Ԫʽջ��ĵ�ַ
vector<string>cmpCode; //��Ż��ָ�� û�зֺ�

bool buildCodes();

int getFreeR() //�ҿ��мĴ�������Ĵ���ʱʹ��
{
    int pos=-1,i=0;
    for(i=0; i<3; i++)
        if(rGroup[i]=="")
        {
            pos=i;
            break;
        }
    return pos;
}
string opToCmpil(string str)  //���������ת��Ϊ������Ե�ָ��
{
    string cmpStr="unknown";
    if(str=="+")
        cmpStr="ADD";
    else if(str=="-")
        cmpStr="SUB";
    else if(str=="*")
        cmpStr="MUL";
    else if(str=="/")
        cmpStr="DIV";
    return cmpStr;
}
bool buildDSEG()  //�������ݶλ����� δ��
{
    int nums[5]= {1};
    int i=0;
    string aN[5]; //intתstring��ʱ����
    stringstream stream1;
    for(i=0; i<5; i++)
    {
        stream1<<nums[i];
        aN[i]=stream1.str();
    }
    string tmpStr="DSEG    SEGMENT"; //�����������ʱ����
    cmpCode.push_back(tmpStr);
    tmpStr="        int    DD "+aN[0]+" DUP(0)";
    cmpCode.push_back(tmpStr);
    tmpStr="DSEG    ENDS";
    cmpCode.push_back(tmpStr);
    return true;
}
bool buildCSEG()
{
    string tmpStr="CSEG    SEGMENT";
    cmpCode.push_back(tmpStr);
    tmpStr="        ASSUME  CS:CSEG,DS:DSEG";
    cmpCode.push_back(tmpStr);
    buildCodes();
    tmpStr="CSEG    ENDS";
    cmpCode.push_back(tmpStr);
    return true;
}
bool buildCodes()
{
    int i=0,qtI=0,R=-1;
    string tmpStr; //�����ʱ������
    for(i=0; i<qtS.size(); i++)
    {
        if( opToCmpil(qtS[i].s[0].name)!="unknown" ) //������
        {
            if(rGroup[0]=="")
            {
                tmpStr="        LD R,"+qtS[i].s[1].name;
                cmpCode.push_back(tmpStr);
                tmpStr="        "+opToCmpil(qtS[i].s[0].name)+" R,"+qtS[i].s[2].name;
                cmpCode.push_back(tmpStr);
            }
            else if(rGroup[0]==qtS[i].s[1].name)
            {
                if(qtS[i].s[1].L+1)
                {
                    tmpStr="        ST R,"+qtS[i].s[1].name;
                    cmpCode.push_back(tmpStr);
                    tmpStr="        "+opToCmpil(qtS[i].s[0].name)+" R,"+qtS[i].s[2].name;
                    cmpCode.push_back(tmpStr);
                }
                else
                {
                    tmpStr="        "+opToCmpil(qtS[i].s[0].name)+" R,"+qtS[i].s[2].name;
                    cmpCode.push_back(tmpStr);
                }
            }
            else
            {
                if(qtS[qtPos[0]].s[qtPos[1]].L+1)  //����ʵ�ִ���qtpos���Ԫ��λ�ö�λ
                {
                    tmpStr="        ST R,"+qtS[qtPos[0]].s[qtPos[1]].name;
                    cmpCode.push_back(tmpStr);
                    tmpStr="        LD R,"+qtS[i].s[1].name;
                    cmpCode.push_back(tmpStr);
                    tmpStr="        "+opToCmpil(qtS[i].s[0].name)+" R,"+qtS[i].s[2].name;
                    cmpCode.push_back(tmpStr);
                }
                else
                {
                    tmpStr="        LD R,"+qtS[i].s[1].name;
                    cmpCode.push_back(tmpStr);
                    tmpStr="        "+opToCmpil(qtS[i].s[0].name)+" R,"+qtS[i].s[2].name;
                    cmpCode.push_back(tmpStr);
                }
            }
            rGroup[0]=qtS[i].s[3].name;
            qtPos[0]=i;
            qtPos[1]=3;
        }
        else  if(qtS[i].s[0].name=="=") //�Ǹ�ֵ���
        {
            if(rGroup[0]=="")
            {
                tmpStr="        LD R,"+qtS[i].s[1].name;
                cmpCode.push_back(tmpStr);
            }
            else if(rGroup[0]==qtS[i].s[1].name)
            {
                tmpStr="        ST R,"+qtS[i].s[1].name;
                cmpCode.push_back(tmpStr);
            }
            else  // !=B , !=A ?
            {
                if(qtS[qtPos[0]].s[qtPos[1]].L+1)
                {
                    tmpStr="        ST R,"+qtS[qtPos[0]].s[qtPos[1]].name;
                    cmpCode.push_back(tmpStr);
                    tmpStr="        LD R,"+qtS[i].s[1].name;
                    cmpCode.push_back(tmpStr);
                }
                else
                {
                    tmpStr="        LD R,"+qtS[i].s[1].name;
                    cmpCode.push_back(tmpStr);
                }
            }
            rGroup[0]=qtS[i].s[3].name;
            qtPos[0]=i;
            qtPos[1]=3;
        }
    }
    return true;
}
int buildCpl() //��Ԫʽ���ɻ��ָ�� û�зֺ�
{
    int i=0,qtI=0,R=-1;
    string tmpStr; //�����ʱ������
    buildDSEG();
    buildCSEG();
    return 0;
}
int findElem(string str)
{
    int order=-1,i=0;
    for(i=0; i<sba.size(); i++)
    {
        if(sba[i].name==str)
        {
            order=i;
            break;
        }
    }
    return order;
}
void initL()
{
    int i=0,j=0;
    for(i=0; i<qua_list.size(); i++)
        for(j=1; j<4; j++)
        {
            if(findElem(qua_list[i].s[j])<0)
            {
                SYMBL symE;
                symE.name=qua_list[i].s[j];
                if( qua_list[i].s[j][0]!='t')
                    symE.L=0;
                else
                    symE.L=-1;
                sba.push_back(symE);
            }
        }
}
void qtScanL()
{
    int i=0,j=0;
    QT qtElem;
    for(i=qua_list.size()-1; i>=0; i--)
    {
        for(j=0; j<4; j++)
        {
            qtElem.s[j].name=qua_list[i].s[j];
        }
        int order=0;
        order=findElem(qua_list[i].s[3]);
        qtElem.s[3].L=sba[order].L;
        sba[order].L=-1;
        order=findElem(qua_list[i].s[2]);
        qtElem.s[2].L=sba[order].L;
        sba[order].L=i;
        order=findElem(qua_list[i].s[1]);
        qtElem.s[1].L=sba[order].L;
        sba[order].L=i;
        qtS.push_back(qtElem);
    }
    reverse(qtS.begin(),qtS.end());
}
int runCompil()
{
    initL();
    qtScanL();
    buildCpl();
    int i=0;
    for(i=0; i<cmpCode.size(); i++)
    {
        cout<<cmpCode[i]<<endl;
    }
    return 0;
}
