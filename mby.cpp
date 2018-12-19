#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
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
} sY;
struct QT
{
    SYMBL s[4];
};
bool ifDiv(string str);
int optimization();
int divBlock();
extern vector<Quadruple> optdQT;
extern vector<Quadruple> qua_list;
extern vector<vector<Synbl> > sbl;
extern vector<Arr> arrs;
vector<SYMBL>sba;  //α���ű�  //�޻�Ծ��Ϣ��Ԫʽ
vector<QT>qtS;      //�л�Ծ��Ϣ��Ԫʽ
string rGroup[3]= {"","",""}; //�Ĵ��������� �ٶ����Ĵ���
int qtPos[2];  //���ڴ���ڼĴ������Ԫ������Ԫʽջ��ĵ�ַ
vector<string>cmpCode; //��Ż��ָ�� û�зֺ�
string cmpTmp="";//�����뻺���� ����һ�зǱ�ż���ʱ����ͷ��
int divCounts[7]= {0};//0Ϊwhile 1Ϊif 2 Ϊ��ϵ����
int typeSize[4]= {4,2,2,4}; //��Ŷ�Ӧռ��ʮ��������λ
vector<int>types;
vector<string>indica;
vector<SYMBL>activeVs; //�洢��Ծ����ʱ���� ������ƺ�ռ�ô�С
bool buildCodes();
int iCmpFn(string str);
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
string addrToType(int i) //����ʮ��������λת�����赥Ԫ��С����
{
    string str;
    switch(i)
    {
    case 8:
        str="DD";
        break;
    case 4:
        str="DW";
        break;
    case 2:
        str="DB";
        break;
    default:
        str="ERROR";
        break;
    }
    return str;
}
int nameToSize(string str) //��������Ѱ�Ҵ�С
{
    int strSize=0,i,j;
    if(str[0]!='t')
        for(i=0; i<sbl.size(); i++)
        {
            for(j=0; j<sbl[i].size(); j++)
            {
                if(sbl[i][j].name==str)
                {
                    strSize=typeSize[i];
                    break;
                }
            }
        }
    if(str[0]=='t')
        for(i=0; i<activeVs.size(); i++)
        {
            if(activeVs[i].name==str)
            {
                strSize=activeVs[i].L;
                break;
            }
        }
    return strSize;
}
string nameToAddr(string str) //���ݱ�������Ѱ���������ݶ��еĵ�ַ
{
    string addr="",vType="";
    stringstream stream1;
    int intAddr=-1,xNum; //xnum��������ת��
    int i=0,j=0;
    if( (str[0]-'0')<=9 && (str[0]-'0')>=0) //����Ƿ�Ϊ����
        return str;
    for(i=0; i<str.size(); i++) //����Ƿ�Ϊ����
        if(str[i]=='[')
        {
            iCmpFn("        MOV BX,"+nameToAddr(str.substr(i+1))+"*4");
            addr="["+str.substr(0,i)+"+BX]";
            return addr;
        }
    for(i=0; i<sbl.size(); i++)
    {
        for(j=0; j<sbl[i].size(); j++)
        {
            if(sbl[i][j].name==str)
            {
                intAddr=sbl[i][j].addr;
                vType=sbl[i][j].type+"S";
                break;
            }
        }
    }
    if(intAddr>=0)
    {
        stream1<<intAddr;
        addr=stream1.str();
        addr="["+vType+"+"+addr+"]";
    }
    else
    {
        addr=str;
    }
    return addr;
}
string opRelation(QT qtEq)
{
    string tmpJMP;
    if(qtEq.s[0].name=="<")
        tmpJMP="JB";
    else if(qtEq.s[0].name==">")
        tmpJMP="JA";
    else if(qtEq.s[0].name=="<=")
        tmpJMP="JBE";
    else if(qtEq.s[0].name==">=")
        tmpJMP="JAE";
    else if(qtEq.s[0].name=="==")
        tmpJMP="JE";
    else if(qtEq.s[0].name=="!=")
        tmpJMP="JNE";
    else
        tmpJMP="ERROR";
    return tmpJMP;
}
int ifSybCodes(QT qtEq,int i) //�жϴ�С��������
{
    string tmpJMP,tmpStr;
    if(qtEq.s[0].name=="<")
        tmpJMP="LT";
    else if(qtEq.s[0].name==">")
        tmpJMP="GT";
    else if(qtEq.s[0].name=="<=")
        tmpJMP="LE";
    else if(qtEq.s[0].name==">=")
        tmpJMP="GE";
    else if(qtEq.s[0].name=="==")
        tmpJMP="EQ";
    else if(qtEq.s[0].name=="!=")
        tmpJMP="NE";
    else
        tmpJMP="ERROR";
    if(tmpJMP!="ERROR")
    {
        if(rGroup[0]!="")
        {
            iCmpFn("        MOV "+nameToAddr(rGroup[0])+",AX");
            rGroup[0]="";
        }
        tmpStr="        MOV AX,"+nameToAddr(qtEq.s[1].name) +" "+tmpJMP+" "+nameToAddr(qtEq.s[2].name);
        iCmpFn(tmpStr);
        rGroup[0]=qtEq.s[3].name;
        qtPos[0]=i;
        qtPos[1]=3;
    }
    return 0;
}
bool buildDSEG()  //�������ݶλ����� δ��
{
    int tSize=0;
    int i=0;
    string aN[5]; //intתstring��ʱ����
    stringstream stream1;
    string tmpStr="DSEG    SEGMENT"; //�����������ʱ����
    cmpCode.push_back(tmpStr);

    for(i=0; i<sbl.size(); i++) //������ű��еĿռ�
    {
        int len=sbl[i].size();
        int maxAddr;
        if(len)
        {
            maxAddr=sbl[i][len-1].addr+typeSize[i];
            maxAddr/=typeSize[i];
            stream1<<maxAddr;
            tmpStr="        "+sbl[i][0].type+"S "+addrToType(typeSize[i])+" "+stream1.str()+" DUP(0)";
            cmpCode.push_back(tmpStr);
        }
    }
    for(i=0; i<arrs.size(); i++)
    {
        tSize=arrs[i].length/arrs[i].up;
        stringstream stream2;
        stream2<<arrs[i].up;
        tmpStr="        "+arrs[i].name+" "+addrToType(tSize)+" "+stream2.str()+" DUP(0)";
        cmpCode.push_back(tmpStr);
    }
    for(i=0; i<activeVs.size(); i++) //�����Ծ��ʱ�����Ŀռ�
    {
        tmpStr="        "+activeVs[i].name+" "+addrToType(activeVs[i].L)+" 0";
        cmpCode.push_back(tmpStr);
    }
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
    tmpStr="START: 	MOV AX,DSEG";
    cmpCode.push_back(tmpStr);
    tmpStr="        MOV DS,AX";
    cmpCode.push_back(tmpStr);
    tmpStr="        XOR AX,AX";
    cmpCode.push_back(tmpStr);
    buildCodes();
    iCmpFn("        MOV AH,02H");
    iCmpFn("        INT 21H");
    tmpStr="CSEG    ENDS";
    cmpCode.push_back(tmpStr);
    tmpStr="END START";
    cmpCode.push_back(tmpStr);
    return true;
}
string divCodes(QT qtEq)
{
    string tmpStr="";
    stringstream stream1;
    if(qtEq.s[0].name=="wh")
    {
        if(cmpTmp=="")
        {
            types.push_back(++divCounts[0]);
            stream1<<divCounts[0];
            cmpTmp="WH"+stream1.str()+":";
            indica.push_back("WH"+stream1.str());
        }
        else
            indica.push_back(cmpTmp.substr(0,cmpTmp.size()-2));
    }
    else if(qtEq.s[0].name=="do")
    {
        iCmpFn("        CMP AX,0");
        if(cmpTmp=="")
        {
            int tmpCnts=types.back();
            stream1<<tmpCnts;
            string tmpJmps="WHE"+stream1.str();
            iCmpFn("        JE  "+tmpJmps);
            indica.push_back(tmpJmps);
        }
        else
        {
            iCmpFn("        JE  "+cmpTmp.substr(0,cmpTmp.size()-2));
            indica.push_back(cmpTmp.substr(0,cmpTmp.size()-2));
        }
    }
    else if(qtEq.s[0].name=="we")
    {
        string tmpJmps=indica[indica.size()-1];
        tmpJmps.erase(remove(tmpJmps.begin(),tmpJmps.end(),'E'),tmpJmps.end());
        iCmpFn("        JMP "+tmpJmps);
        cmpTmp=indica.back()+":";
        types.pop_back();
        indica.pop_back();
        indica.pop_back();
    }
    else if(qtEq.s[0].name=="if")
    {
        iCmpFn("        CMP AX,0");
        if(cmpTmp=="")
        {
            types.push_back(++divCounts[1]);
            stream1<<divCounts[1];
            iCmpFn("        JE  IFE"+stream1.str());
        }
        else
            iCmpFn("        JE  "+cmpTmp.substr(0,cmpTmp.size()-2));
    }
    else if(qtEq.s[0].name=="ie")
    {
        int tmpCnts=types.back();
        stream1<<tmpCnts;
        cmpTmp="IFE"+stream1.str()+":";
    }
    return tmpStr;
}
int iCmpFn(string str)  //�����������ջ���� ���ֻ������Զ�����
{
    if(cmpTmp.size()<=8)
        str=cmpTmp+str.substr(cmpTmp.size());
    else
        str=cmpTmp+str.substr(8);
    cmpCode.push_back(str);
    cmpTmp="";
    return 0;
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
                iCmpFn("        MOV AX,"+nameToAddr(qtS[i].s[1].name));
                iCmpFn("        "+opToCmpil(qtS[i].s[0].name)+" AX,"+nameToAddr(qtS[i].s[2].name));
            }
            else if(rGroup[0]==qtS[i].s[1].name)
            {
                if(qtS[i].s[1].L+1)
                    iCmpFn("        MOV "+nameToAddr(qtS[i].s[1].name)+",AX");
                iCmpFn("        "+opToCmpil(qtS[i].s[0].name)+" AX,"+nameToAddr(qtS[i].s[2].name));
            }
            else
            {
                if(qtS[qtPos[0]].s[qtPos[1]].L+1)  //����ʵ�ִ���qtpos���Ԫ��λ�ö�λ
                    //iCmpFn("        ST AX,"+qtS[qtPos[0]].s[qtPos[1]].name);
                    iCmpFn("        MOV "+nameToAddr(qtS[qtPos[0]].s[qtPos[1]].name)+",AX");
                iCmpFn("        MOV AX,"+nameToAddr(qtS[i].s[1].name));
                iCmpFn("        "+opToCmpil(qtS[i].s[0].name)+" AX,"+nameToAddr(qtS[i].s[2].name));
            }
            rGroup[0]=qtS[i].s[3].name;
            qtPos[0]=i;
            qtPos[1]=3;
        }
        else  if(qtS[i].s[0].name=="=") //�Ǹ�ֵ���
        {
            if(rGroup[0]=="")
            {
                iCmpFn("        MOV AX,"+nameToAddr(qtS[i].s[1].name));
            }
            else if(rGroup[0]==qtS[i].s[1].name)
            {
                if(qtS[i].s[1].L+1)
                    //iCmpFn("        ST AX,"+qtS[i].s[1].name);
                    iCmpFn("        MOV "+nameToAddr(qtS[i].s[1].name)+",AX");
            }
            else  // !=B , !=A ?
            {
                if(qtS[qtPos[0]].s[qtPos[1]].L+1)
                    //iCmpFn("        ST AX,"+qtS[qtPos[0]].s[qtPos[1]].name);
                    iCmpFn("        MOV "+nameToAddr(qtS[qtPos[0]].s[qtPos[1]].name)+",AX");
                iCmpFn("        MOV AX,"+nameToAddr(qtS[i].s[1].name));
            }
            rGroup[0]=qtS[i].s[3].name;
            qtPos[0]=i;
            qtPos[1]=3;
        }
        else if( ifDiv(qtS[i].s[0].name) ) //�Ƿ�Ϊ�����ֿ����Ԫʽ
        {
            if(rGroup[0]!="")
            {
                if(qtS[qtPos[0]].s[qtPos[1]].L+1)
                    iCmpFn("        MOV "+nameToAddr(rGroup[0])+",AX");
                rGroup[0]="";
            }
            divCodes(qtS[i]);
        }
        else  if( opRelation(qtS[i])!="ERROR" ) //��ϵ�ȽϷ� ���뾭���Ż�
        {
            if(rGroup[0]!="") //���Ĵ����������Ϊ��
                if(qtS[qtPos[0]].s[qtPos[1]].L+1) //����Ĵ����������Ծ
                    iCmpFn("        MOV "+nameToAddr(qtS[qtPos[0]].s[qtPos[1]].name)+",AX");
            if(rGroup[0]!=qtS[i].s[1].name) //����Ĵ�����Ĳ���s1����¼Ĵ���
                iCmpFn("        MOV AX,"+nameToAddr(qtS[i].s[1].name));
            iCmpFn("        CMP AX,"+nameToAddr(qtS[i].s[2].name));
            stringstream stream1;
            stream1<<divCounts[2]++;
            iCmpFn("        "+opRelation(qtS[i])+" J"+stream1.str());
            iCmpFn("        MOV AX,0");
            cmpTmp="J"+stream1.str()+":";
            iCmpFn("        MOV AX,1");
            rGroup[0]=qtS[i].s[3].name;
            qtPos[0]=i;
            qtPos[1]=3;
        }
        else if(qtS[i].s[0].name=="%")
        {
            if(rGroup[0]!="")
                if(qtS[qtPos[0]].s[qtPos[1]].L+1)  //����ʵ�ִ���qtpos���Ԫ��λ�ö�λ
                    iCmpFn("        MOV "+nameToAddr(qtS[qtPos[0]].s[qtPos[1]].name)+",AX");
            iCmpFn("        MOV AX,"+nameToAddr(qtS[i].s[1].name));
            iCmpFn("        DIV "+nameToAddr(qtS[i].s[2].name));
            iCmpFn("        MOV AX,DX");
            rGroup[0]=qtS[i].s[3].name;
            qtPos[0]=i;
            qtPos[1]=3;
            //ifSybCodes(qtS[i],i);
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
    for(i=0; i<optdQT.size(); i++)
        for(j=1; j<4; j++)
        {
            if(findElem(optdQT[i].s[j])<0 && optdQT[i].s[j]!="_")
            {
                SYMBL symE;
                symE.name=optdQT[i].s[j];
                if( optdQT[i].s[j][0]!='t')
                    symE.L=0;
                else
                    symE.L=-1;
                sba.push_back(symE);
            }
        }
}

void pushActive(int L,string str,int elsize)
{
    int i=0,flag=0;
    if(L+1)
        if(str[0]=='t')
        {
            for(i=0; i<activeVs.size(); i++)
                if(activeVs[i].name==str)
                {
                    flag=1;
                    break;
                }
            if(!flag)
            {
                SYMBL tmpT;
                tmpT.name=str;
                tmpT.L=elsize;
                activeVs.push_back(tmpT);
            }
        }
}
void qtScanL() //���ɴ��л�Ծ��Ϣ����Ԫʽ�� ���ύ��Ծ��ʱ����
{
    int i=0,j=0,p=0,elemSize=0; //elemsize���ڴ洢��Ԫʽ����Ԫ�صĴ�С
    QT qtElem;
    for(i=optdQT.size()-1; i>=0; i--)
    {
        for(j=0; j<4; j++)
        {
            qtElem.s[j].name=optdQT[i].s[j];
        }
        if(!ifDiv(optdQT[i].s[0]))
        {
            for(p=1; p<4; p++)
                if(nameToSize(optdQT[i].s[3])!=0)
                    elemSize=nameToSize(optdQT[i].s[3]);
            int order=0;
            order=findElem(optdQT[i].s[3]);
            qtElem.s[3].L=sba[order].L;
            sba[order].L=-1;

            order=findElem(optdQT[i].s[2]);
            qtElem.s[2].L=sba[order].L;
            sba[order].L=i;

            order=findElem(optdQT[i].s[1]);
            qtElem.s[1].L=sba[order].L;
            sba[order].L=i;
            for(p=1; p<4; p++)
                pushActive(qtElem.s[p].L,qtElem.s[p].name,elemSize);
        }
        qtS.push_back(qtElem);
    }
    reverse(qtS.begin(),qtS.end());
}
int runCompil()
{
    ofstream out;
    out.open("compileCodes.ASM");
    divBlock();
    initL();
    qtScanL();
    buildCpl();
    int i=0;
    for(i=0; i<cmpCode.size(); i++)
    {
        out<<cmpCode[i]<<endl;
    }
    return 0;
}
