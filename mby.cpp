#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "head.h"
//装有四元式转汇编相关代码
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
vector<SYMBL>sba;  //伪符号表  //无活跃信息四元式
vector<QT>qtS;      //有活跃信息四元式
string rGroup[3]= {"","",""}; //寄存器组名称 假定三寄存器
int qtPos[2];  //用于存放在寄存器里的元素在四元式栈里的地址
vector<string>cmpCode; //存放汇编指令 没有分号
string cmpTmp="";//汇编代码缓存区 在下一行非标号集合时附在头部
int divCounts[7]= {0};
int typeSize[4]= {8,2,2,8}; //存放对应占用十六进制数位
vector<int>types;
vector<string>indica;
bool buildCodes();
int iCmpFn(string str);
int getFreeR() //找空闲寄存器，多寄存器时使用
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
string opToCmpil(string str)  //将运算符等转换为汇编语言的指令
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
    else if(str=="%")
        cmpStr="MOD";
    return cmpStr;
}
string addrToType(int i) //根据十六进制数位转换所需单元大小类型
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
string nameToAddr(string str) //根据变量名称寻找其在数据段中的地址
{
    string addr="",vType="";
    stringstream stream1;
    int intAddr=-1,xNum; //xnum用于类型转换
    int i=0,j=0;
    if( (str[0]-'0')<=9 && (str[0]-'0')>=0) //检查是否为常数
        return str;
    for(i=0; i<str.size(); i++) //检查是否为数组
        if(str[i]=='[')
        {
            addr="["+str.substr(0,i)+"+"+nameToAddr(str.substr(i+1))+"*8]";
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
int ifSybCodes(QT qtEq,int i) //判断大小的算符编成
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
            iCmpFn("        MOV "+nameToAddr(rGroup[0])+",EAX");
            rGroup[0]="";
        }
        tmpStr="        MOV EAX,"+nameToAddr(qtEq.s[1].name) +" "+tmpJMP+" "+nameToAddr(qtEq.s[2].name);
        iCmpFn(tmpStr);
        rGroup[0]=qtEq.s[3].name;
        qtPos[0]=i;
        qtPos[1]=3;
    }
    return 0;
}
bool buildDSEG()  //建立数据段汇编代码 未完
{
    int nums[5]= {1};
    int i=0;
    string aN[5]; //int转string临时变量
    stringstream stream1;
    string tmpStr="DSEG    SEGMENT"; //汇编语句输出临时变量
    cmpCode.push_back(tmpStr);

    for(i=0; i<sbl.size(); i++)
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
    tmpStr="        XOR EAX,EAX";
    cmpCode.push_back(tmpStr);
    buildCodes();
    iCmpFn("        MOV AH,02H");
    iCmpFn("        INT 21H");
    tmpStr="CSEG    ENDS";
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
        iCmpFn("        CMP EAX,0");
        if(cmpTmp=="")
        {
            int tmpCnts=types.back();
            stream1<<tmpCnts;
            string tmpJmps="WHE"+stream1.str();
            iCmpFn("        JNE  "+tmpJmps);
            indica.push_back(tmpJmps);
        }
        else
        {
            iCmpFn("        JNE  "+cmpTmp.substr(0,cmpTmp.size()-2));
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
        iCmpFn("        CMP EAX,0");
        if(cmpTmp=="")
        {
            types.push_back(++divCounts[1]);
            stream1<<divCounts[1];
            iCmpFn("        JNE  IFE"+stream1.str());
        }
        else
            iCmpFn("        JNE  "+cmpTmp.substr(0,cmpTmp.size()-2));
    }
    else if(qtEq.s[0].name=="ie")
    {
        int tmpCnts=types.back();
        stream1<<tmpCnts;
        cmpTmp="IFE"+stream1.str()+":";
    }
    return tmpStr;
}
int iCmpFn(string str)  //将汇编语言入栈函数
{
    str=cmpTmp+str.substr(cmpTmp.size());
    cmpCode.push_back(str);
    cmpTmp="";
    return 0;
}
bool buildCodes()
{
    int i=0,qtI=0,R=-1;
    string tmpStr; //存放临时汇编语句
    for(i=0; i<qtS.size(); i++)
    {
        if( opToCmpil(qtS[i].s[0].name)!="unknown" ) //操作符
        {
            if(rGroup[0]=="")
            {
                iCmpFn("        MOV EAX,"+nameToAddr(qtS[i].s[1].name));
                iCmpFn("        "+opToCmpil(qtS[i].s[0].name)+" EAX,"+nameToAddr(qtS[i].s[2].name));
            }
            else if(rGroup[0]==qtS[i].s[1].name)
            {
                if(qtS[i].s[1].L+1)
                    iCmpFn("        MOV "+nameToAddr(qtS[i].s[1].name)+",EAX");
                iCmpFn("        "+opToCmpil(qtS[i].s[0].name)+" EAX,"+nameToAddr(qtS[i].s[2].name));
            }
            else
            {
                if(qtS[qtPos[0]].s[qtPos[1]].L+1)  //利用实现存在qtpos里的元素位置定位
                    //iCmpFn("        ST EAX,"+qtS[qtPos[0]].s[qtPos[1]].name);
                    iCmpFn("        MOV "+nameToAddr(qtS[qtPos[0]].s[qtPos[1]].name)+",EAX");
                iCmpFn("        MOV EAX,"+nameToAddr(qtS[i].s[1].name));
                iCmpFn("        "+opToCmpil(qtS[i].s[0].name)+" EAX,"+nameToAddr(qtS[i].s[2].name));
            }
            rGroup[0]=qtS[i].s[3].name;
            qtPos[0]=i;
            qtPos[1]=3;
        }
        else  if(qtS[i].s[0].name=="=") //是赋值语句
        {
            if(rGroup[0]=="")
            {
                iCmpFn("        MOV EAX,"+nameToAddr(qtS[i].s[1].name));
            }
            else if(rGroup[0]==qtS[i].s[1].name)
            {
                if(qtS[i].s[1].L+1)
                    //iCmpFn("        ST EAX,"+qtS[i].s[1].name);
                    iCmpFn("        MOV "+nameToAddr(qtS[i].s[1].name)+",EAX");
            }
            else  // !=B , !=A ?
            {
                if(qtS[qtPos[0]].s[qtPos[1]].L+1)
                    //iCmpFn("        ST EAX,"+qtS[qtPos[0]].s[qtPos[1]].name);
                    iCmpFn("        MOV "+nameToAddr(qtS[qtPos[0]].s[qtPos[1]].name)+",EAX");
                iCmpFn("        MOV EAX,"+nameToAddr(qtS[i].s[1].name));
            }
            rGroup[0]=qtS[i].s[3].name;
            qtPos[0]=i;
            qtPos[1]=3;
        }
        else if( ifDiv(qtS[i].s[0].name) )
        {
            if(rGroup[0]!="")
            {
                if(qtS[qtPos[0]].s[qtPos[1]].L+1)
                    iCmpFn("        MOV "+nameToAddr(rGroup[0])+",EAX");
                rGroup[0]="";
            }
            divCodes(qtS[i]);
        }
        else
        {
            ifSybCodes(qtS[i],i);
        }
    }
    return true;
}
int buildCpl() //四元式生成汇编指令 没有分号
{
    int i=0,qtI=0,R=-1;
    string tmpStr; //存放临时汇编语句
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
void qtScanL()
{
    int i=0,j=0;
    QT qtElem;
    for(i=optdQT.size()-1; i>=0; i--)
    {
        for(j=0; j<4; j++)
        {
            qtElem.s[j].name=optdQT[i].s[j];
        }
        if(!ifDiv(optdQT[i].s[0]))
        {
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
