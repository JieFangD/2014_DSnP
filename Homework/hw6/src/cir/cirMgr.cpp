/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include <string>
#include <sstream>
#include <map>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;
unsigned CirGate::_gRef=0;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine const (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

string int2str(int i){
   stringstream istr;
   string str;
   istr<<i;
   istr>>str;
   return str;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool
CirMgr::readCircuit(const string& fileName)
{
   ifstream fin(fileName.c_str(),ios::in);
   string str;
   while(getline(fin,str)){ 
      input.push_back(str);
   }
   fin.close();
   //for(int i=0;i<input.size();i++)
     // cout<<input[i]<<endl;
   if(input.size()==0){
      cerr << "Error: cannot open design \""<<fileName<<"\"!!"<<endl;
      return false;
   }
   if(!handleError(input)){
      if(handleInput()) 
         return true;
   }
   return false;
}
bool CirMgr::handleError(vector<string> str){
   int n[5],num,k,aig[3];
   string word[8],tmp;
   stringstream istring(input[0]);
   for(int i=0;i<6;i++)
      istring>>word[i];
   stringstream nstring(input[0]);
   nstring>>word[0];
   for(int i=0;i<5;i++)
      nstring>>n[i];
   if(word[1]==word[6]){
      errMsg="number of vars";
      colNo=4;
      parseError(MISSING_NUM);
      return true;
   }
   else if(word[2]==word[6]){
      errMsg="number of PIs";
      colNo=5+word[1].size();
      parseError(MISSING_NUM);
      return true;
   }
   else if(word[3]==word[6]){
      errMsg="number of latches";
      colNo=6+word[1].size()+word[2].size();
      parseError(MISSING_NUM);
      return true;
   }
   else if(word[4]==word[6]){
      errMsg="number of POs";
      colNo=7+word[1].size()+word[2].size()+word[3].size();
      parseError(MISSING_NUM);
      return true;
   }
   else if(word[5]==word[6]){
      errMsg="number of AIGs";
      colNo=8+word[1].size()+word[2].size()+word[3].size()+word[4].size();
      parseError(MISSING_NUM);
      return true;
   }
   colNo=8+word[1].size()+word[2].size()+word[3].size()+word[4].size()+word[5].size();
   if(str[0][colNo]!=NULL){
      lineNo=0;
      colNo=8+word[1].size()+word[2].size()+word[3].size()+word[4].size()+word[5].size();
      parseError(MISSING_NEWLINE);
      return true;
   }
   if(n[0]<n[1]+n[4]){
      errMsg="Num of variables";
      errInt=n[0];
      parseError(NUM_TOO_SMALL);
      return true;
   }
   if(n[2]!=0){
      errMsg="latches";
      parseError(ILLEGAL_NUM);
      return true;
   }
   if(str.size()<1+n[1]){
      errMsg="PI";
      parseError(MISSING_DEF);
      return true;
   }
   else if(str.size()<1+n[1]+n[3]){
      errMsg="PO";
      parseError(MISSING_DEF);
      return true;
   }
   else if(str.size()<1+n[1]+n[3]+n[4]){
      errMsg="AIG";
      parseError(MISSING_DEF);
      return true;
   }  
   for(int i=0;i<str.size();i++){
      lineNo=i;
      for(int j=0;j<str[i].size();j++){
         colNo=j;
         if((str[i][0]==' '||(str[i][j-1]==' '&&str[i][j]==' '))&&i<1+n[1]+n[3]+n[4]){
            parseError(EXTRA_SPACE);
            return true;
         }
         if(str[i][j]==9){
            errInt=9;
            parseError(ILLEGAL_WSPACE);
            return true;
         }
         if(i==0&&str[0][3]!=' '){
            stringstream tstring(str[0]);
            tstring>>tmp;
            if(isdigit(str[i][3])){
               colNo=3;
               parseError(MISSING_SPACE);
            }
            else{
               errMsg=tmp;
               parseError(ILLEGAL_IDENTIFIER);
            }
            return true;
         }
         if(i>=1&&i<1+n[1]){
            stringstream tstring(str[i]);
            tstring>>num;
            if(num%2==1&&num/2!=0){
               errMsg="PI ";
               errInt=num;
               parseError(CANNOT_INVERTED);
               return true;
            }
            if(num/2>n[0]){
               errInt=num;
               parseError(MAX_LIT_ID);
               return true;
            }
         }
         if(i>=1+n[1]&&i<1+n[1]+n[3]){
            stringstream tstring(str[i]);
            tstring>>num;
            if(num/2>n[0]){
               errInt=num;
               parseError(MAX_LIT_ID);
               return true;
            }
         }
         if(i>=1+n[1]+n[3]&&i<1+n[1]+n[3]+n[4]){
            stringstream tstring(str[i]);
            tstring>>num;
            if(num%2==1&&num!=1){
               errMsg="AIG ";
               errInt=num;
               parseError(CANNOT_INVERTED);
               return true;
            }
            if(num/2==0){
               errInt=num;
               parseError(REDEF_CONST);
               return true;
            }
         }
         if(i>=1+n[1]+n[3]+n[4]){
            if(str[i][0]=='i'||str[i][0]=='o'){
               if(str[i][1]==' '){
                  colNo=1;
                  parseError(EXTRA_SPACE);
                  return true;
               }
               if(!isdigit(str[i][1])){
                  k=2;
                  errMsg=str[i][1];
                  while(str[i][k]!=' '){
                     errMsg+=str[i][k];
                     k++;
                  }
                  parseError(ILLEGAL_SYMBOL_TYPE);
                  return true;
               }
               if(isdigit(str[i][1])){
                  k=2;
                  while(isdigit(str[i][k]))
                     k++;
                  if(str[i][k]==NULL||str[i][k+1]==NULL){
                     errMsg="\"symbolic name\"";
                     parseError(MISSING_NUM);
                     return true;
                  }
                  k++;
                  while(str[i][k]!=NULL){ 
                     errInt=(int)str[i][k];
                     if(errInt>=0&&errInt<=31){
                        parseError(ILLEGAL_SYMBOL_NAME);
                        return true;
                     }
                     k++;
                  }
               }
               if(str[i][0]=='i'&&((int)str[i][1]-48)>=n[1]){
                  errInt=(int)str[i][1]-48;
                  errMsg="PI index";
                  parseError(NUM_TOO_BIG);
                  return true;
               }
               else if(str[i][0]=='o'&&((int)str[i][1]-48)>=n[3]){
                  errInt=(int)str[i][1]-48;
                  errMsg="PO index";
                  parseError(NUM_TOO_BIG);
                  return true;
               }
            }
            else if(str[i][0]!='c'||str[i][0]==NULL){
               errMsg=str[i][0];
               parseError(ILLEGAL_SYMBOL_TYPE);
               return true;
            }
            else if(str[i][0]=='c'){
               if(str[i][1]!=NULL){
                  parseError(MISSING_NEWLINE);
                  return true;
               }
               return false;
            }
         }
      }
   }
   for(int i=1+n[1]+n[3];i<1+n[1]+n[3]+n[4];i++){
      k=0;
      lineNo=i;
      while(isdigit(str[i][k]))
         k++;
      if(str[i][k]!=' '){
         errMsg="AIG input literal";
         colNo=k+1;
         parseError(MISSING_NUM);
         return true;
      }
      else if(str[i][k]==' '){
         k++;
         while(isdigit(str[i][k]))
            k++;
         if(str[i][k]!=' '){
            errMsg="AIG input literal";
            colNo=k+1;
            parseError(MISSING_NUM);
            return true;
         }
         else{
            k++;
            while(isdigit(str[i][k]))
               k++;
            if(str[i][k]!=NULL){
               colNo=k;
               parseError(MISSING_NEWLINE);
               return true;
            }
         }
      }
   }
   for(int i=1+n[1]+n[3];i<1+n[1]+n[3]+n[4];i++){
      stringstream tstring(str[i]);
      for(k=0;k<3;k++){
         tstring>>aig[k];
         if(aig[k]/2>n[0]){
            errInt=aig[k];
            parseError(MAX_LIT_ID);
            return true;
         }
      }
   }              
   return false;
}

bool CirMgr::handleInput(){
   int num,i;
   string word;
   stringstream istring(input[0]);
   istring>>word;
   for(i=0;i<5;i++)
      istring>>a[i];
   for(i=1;i<1+a[1];i++){	//PI
      CirGate* newGate=new CirGate;
      stringstream istring(input[i]);      
      istring>>num;
      if(AllGate.find(num/2)!=AllGate.end()){//errorhandle
         errInt=num;
         errGate=AllGate[num/2];
         parseError(REDEF_GATE);
         return false;
      }
      if(num/2==0){//errorhandle
         errInt=num;
         parseError(REDEF_CONST);
         return false;
      }
      newGate->GateID=num/2;
      newGate->Type="PI";
      newGate->LineNo=i+1;
      AllGate[newGate->GateID]=newGate;
      //cout<<newGate->GateID<<endl<<newGate->Type<<endl<<newGate->LineNo<<endl;
   }
   for(i;i<1+a[1]+a[3];i++){	//PO
      stringstream istring(input[i]);      
      istring>>num;
      if(AllGate.find(num/2)==AllGate.end()){
         CirGate* newGate=new CirGate;
         newGate->GateID=num/2;
         if(num/2==0){
            newGate->Type="CONST";
            newGate->LineNo=0;  
         }
	  AllGate[newGate->GateID]=newGate;
      }
      CirGate* newGate=new CirGate;
      newGate->GateID=a[0]+i-a[1];
      newGate->Type="PO";
      newGate->LineNo=i+1;
      AllGate[newGate->GateID]=newGate;
      newGate->faninL=AllGate[num/2];
      if(num%2)
         newGate->invertL=true;
      (AllGate[num/2]->fanout).push_back(newGate);  
      //cout<<newGate->GateID<<endl<<newGate->Type<<endl<<newGate->LineNo<<endl<<newGate->fanout.size();
   }
   for(i;i<input.size();i++){ //AIG
      stringstream istring(input[i]);
      istring>>word;
      if(!isdigit(word[0]))
         break;
      stringstream str(input[i]);
      int b[3];
      for(int j=0;j<3;j++){
         str>>b[j];
      }
      if(AllGate.find(b[0]/2)!=AllGate.end()&&AllGate[b[0]/2]->Type!="UNDEF"&&AllGate[b[0]/2]->Type!="PO"){//errorhandle
         errInt=b[0];
         errGate=AllGate[b[0]/2];
         parseError(REDEF_GATE);
         return false;
      }
      if(AllGate.find(b[0]/2)==AllGate.end()){
         CirGate* newGate=new CirGate;
         newGate->GateID=b[0]/2;
	  AllGate[newGate->GateID]=newGate;
      }
      AllGate[b[0]/2]->Type="AIG";
      AllGate[b[0]/2]->LineNo=i+1;
      if(AllGate.find(b[1]/2)==AllGate.end()){
         CirGate* newGate=new CirGate;
         newGate->GateID=b[1]/2;
         if(b[1]/2==0){
            newGate->Type="CONST";
            newGate->LineNo=0;  
         }
	  AllGate[newGate->GateID]=newGate;
      }
      if(AllGate.find(b[2]/2)==AllGate.end()){
         CirGate* newGate=new CirGate;
         newGate->GateID=b[2]/2;
         if(b[2]/2==0){
            newGate->Type="CONST";
            newGate->LineNo=0;
         }
	  AllGate[newGate->GateID]=newGate;
      }
      AllGate[b[0]/2]->faninL=AllGate[b[1]/2];
      if(b[1]%2)
         AllGate[b[0]/2]->invertL=true;
      AllGate[b[0]/2]->faninR=AllGate[b[2]/2];
      if(b[2]%2)
         AllGate[b[0]/2]->invertR=true;
      (AllGate[b[1]/2]->fanout).push_back(AllGate[b[0]/2]);
      (AllGate[b[2]/2]->fanout).push_back(AllGate[b[0]/2]);
   }
   for(i;i<input.size();i++){ //symbol section
      stringstream istring(input[i]);
      string _symb;
      istring>>word;
      if(input[i][0]=='c')
         break;
      map<int,CirGate*>::const_iterator it;
      if(input[i][0]=='i'){
         for(it=AllGate.begin();it!=AllGate.end();it++){
            if((it->second)->Type=="PI"&&((it->second)->Symbol).size()==0){
               for(int j=3;j<input[i].size();j++)
                  _symb+=input[i][j];
               ((it->second)->Symbol)=_symb;
               Symb.push_back(input[i]);
               break;
            }
         }
      }
      else if(input[i][0]=='o'){
         for(it=AllGate.begin();it!=AllGate.end();it++){
            if((it->second)->Type=="PO"&&((it->second)->Symbol).size()==0){
               for(int j=3;j<input[i].size();j++)
                  _symb+=input[i][j];
               ((it->second)->Symbol)=_symb;
               Symb.push_back(input[i]);
               break;
            }
         }
      }
   }
   CirGate::setGlobalRef();
   for(i=0;i<a[3];i++){
      AllGate[a[0]+1+i]->setToGlobalRef();
      if(!AllGate[a[0]+1+i]->faninL->isGlobalRef())
         dfsTraversal(AllGate[a[0]+1+i]->faninL);
      else{
         string str;
         str="PO  ";
         str+=(int2str(AllGate[a[0]+1+i]->GateID));
         str+=' ';
         if(AllGate[a[0]+1+i]->faninL->Type=="UNDEF")
            str+='*';
         if(AllGate[a[0]+1+i]->invertL)
            str+="!";
         str+=(int2str(AllGate[a[0]+1+i]->faninL->GateID));
         if(AllGate[a[0]+1+i]->Symbol.size()!=0){
            str+=" (";
            str+=AllGate[a[0]+1+i]->Symbol;
            str+=")";
         }
         Netlist.push_back(str);
      }
   }
   return true;
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
   cout<<"Circuit Statistics"<<endl
       <<"=================="<<endl
       <<"  PI"<<setw(12)<<right<<a[1]<<endl
       <<"  PO"<<setw(12)<<right<<a[3]<<endl
       <<"  AIG"<<setw(11)<<right<<a[4]<<endl
       <<"------------------"<<endl
       <<"  Total"<<setw(9)<<right<<a[1]+a[3]+a[4]<<endl;
}

void
CirMgr::printNetlist() const
{
   cout<<endl;
   for(int i=0;i<Netlist.size();i++)
      cout<<'['<<i<<"] "<<Netlist[i]<<endl;
}

void
CirMgr::printPIs() const
{
   int num;
   cout << "PIs of the circuit:";
   for(int i=0;i<a[1];i++){
      stringstream istring(input[i+1]);
      istring>>num;
      cout<<' '<<num/2;
   }
   cout<<endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit:";
   map<int,CirGate*>::const_iterator it;
   for(it=AllGate.begin();it!=AllGate.end();it++){
      if((it->second)->Type=="PO")
         cout<<" "<<(it->second)->GateID;
   }
   cout << endl;
}

void
CirMgr::printFloatGates() const
{
   bool b1=false,b2=false;
   map<int,CirGate*>::const_iterator it;
   for(it=AllGate.begin();it!=AllGate.end();it++){
      if((it->second)->faninL!=NULL&&(it->second)->faninR!=NULL){
         if((it->second)->faninL->Type=="UNDEF"||(it->second)->faninR->Type=="UNDEF"){
            if(b1==false){
               b1=true;
               cout<<"Gates with floating fanin(s):";
            }
            cout<<" "<<(it->second)->GateID;
         }
      }
      else if((it->second)->faninL!=NULL&&(it->second)->Type=="PO"){
         if((it->second)->faninL->Type=="UNDEF"){
            if(b1==false){
               b1=true;
               cout<<"Gates with floating fanin(s):";
            }
            cout<<" "<<(it->second)->GateID;
         }
      }
   }
   if(b1==true)
      cout<<endl;
   for(it=AllGate.begin();it!=AllGate.end();it++){
      if((it->second)->Type=="AIG"||(it->second)->Type=="PI"){
         if(((it->second)->fanout).size()==0){
            if(b2==false){
               b2=true;
               cout<<"Gates defined but not used  :";
            }
            cout<<" "<<(it->second)->GateID;
         }
      }
   }
   cout<<endl;
}

void
CirMgr::writeAag(ostream& outfile) 
{
   outfile<<"aag ";
   for(int i=0;i<4;i++)
      outfile<<a[i]<<' ';
   outfile<<ANum<<endl;
   for(int i=1;i<1+a[1]+a[3];i++)
      outfile<<input[i]<<endl;
   for(int i=0;i<sortGate.size();i++)
      outfile<<sortGate[i]<<endl;
   for(int i=0;i<Symb.size();i++)
      outfile<<Symb[i]<<endl;
}

void CirMgr::dfsTraversal(CirGate* c){
   string str;
   if(c->faninL&&!c->faninL->isGlobalRef()){
      dfsTraversal(c->faninL);
   }
   if(c->faninR&&!c->faninR->isGlobalRef()){
      dfsTraversal(c->faninR);
   }
   c->setToGlobalRef();
   if(c->Type=="CONST")
      Netlist.push_back("CONST0");
   else if(c->Type=="PI"){
      str="PI  ";
      str+=int2str(c->GateID);
      if(c->Symbol.size()!=0){
         str+=" (";
         str+=c->Symbol;
         str+=")";
      }
      Netlist.push_back(str);
   }
   else if(c->Type=="AIG"){
      ANum++;
      sortGate.push_back(input[c->LineNo-1]);
      str="AIG ";
      str+=(int2str(c->GateID));
      str+=' ';
      if(c->faninL->Type=="UNDEF")
         str+='*';
      if(c->invertL)
         str+="!";
      str+=(int2str(c->faninL->GateID));
      str+=' ';
      if(c->faninR->Type=="UNDEF")
         str+='*';
      if(c->invertR)
         str+="!";
      str+=(int2str(c->faninR->GateID));
      Netlist.push_back(str);
   }  
   if(c->fanout[0]->Type=="PO"&&c->fanout[0]->isGlobalRef()){
      str="PO  ";
      str+=(int2str(c->fanout[0]->GateID));
      str+=' ';
      if(c->Type=="UNDEF")
         str+='*';
      if(c->fanout[0]->invertL)
         str+='!';
      str+=(int2str(c->GateID));
      if(c->fanout[0]->Symbol.size()!=0){
         str+=" (";
         str+=c->fanout[0]->Symbol;
         str+=")";
      }
      Netlist.push_back(str);
   }
}





