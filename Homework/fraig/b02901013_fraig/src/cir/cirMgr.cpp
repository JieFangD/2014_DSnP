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
#include <bitset>
#include <map>
#include "cirMgr.h"
#include "cirGate.h"
#include "sat.h"
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
   //   cout<<input[i]<<endl;
   if(input.size()==0){
      cerr << "Error: cannot open design \""<<fileName<<"\"!!"<<endl;
      return false;
   }
   handleInput();
   return true;
}

void CirMgr::handleInput(){
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
      newGate->GateID=num/2;
      newGate->Type="PI";
      newGate->LineNo=i+1;
      AllGate[newGate->GateID]=newGate;
      //cout<<newGate->GateID<<endl<<newGate->Type<<endl<<newGate->LineNo<<endl;//
   }
   POID=new int [a[3]];
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
      POID[i-a[1]-1]=a[0]+i-a[1];
      newGate->GateID=a[0]+i-a[1];
      newGate->Type="PO";
      newGate->LineNo=i+1;
      AllGate[newGate->GateID]=newGate;
      newGate->faninL=AllGate[num/2];
      if(num%2)
         newGate->invertL=true;
      (AllGate[num/2]->fanout).push_back(newGate);  
      //cout<<newGate->GateID<<endl<<newGate->Type<<endl<<newGate->LineNo<<endl<<newGate->fanout.size();//
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
               int j=0;
               while(input[i][j]!=' ')
                  j++;
               for(int k=j+1;k<input[i].size();k++)
                  _symb+=input[i][k];
               ((it->second)->Symbol)=_symb;
               Symb.push_back(input[i]);
               break;
            }
         }
      }
      else if(input[i][0]=='o'){
         for(it=AllGate.begin();it!=AllGate.end();it++){
            if((it->second)->Type=="PO"&&((it->second)->Symbol).size()==0){
               int j=0;
               while(input[i][j]!=' ')
                  j++;
               for(int k=j+1;k<input[i].size();k++)
                  _symb+=input[i][k];
               ((it->second)->Symbol)=_symb;
               Symb.push_back(input[i]);
               break;
            }
         }
      }
   }
   makeNetlist();
}
void
CirMgr::makeNetlist(){
   CirGate::setGlobalRef();
   Netlist.clear();
   sortGate.clear();
   AndGateNum=0;
   for(int i=0;i<a[3];i++){
      AllGate[a[0]+1+i]->setToGlobalRef();
      if(!AllGate[a[0]+1+i]->faninL->isGlobalRef())
         dfsTraversal(AllGate[a[0]+1+i]->faninL);//dfsTraversal!!!!!!
      else{
         string str;//handle PO(Netlist)
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
         if(doFraig){
            //Var v=solver.newVar();
            AllGate[a[0]+1+i]->_var=AllGate[a[0]+1+i]->faninL->_var;
         }
         Netlist.push_back(str);
      }
   }
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
CirMgr::doSimLog(ofstream *logFile) {
   if(logFile!=0){
      string str[a[3]];
      for(int i=0;i<a[3];i++){
         str[i]="";
         for(int j=(pattern.size()+31)/32-1;j>=0;j--){
            if(AllGate[POID[i]]->invertL==true){
               bitset<32> bit(~AllGate[POID[i]]->faninL->value[j]);
               str[i]+=bit.to_string();
            }
            else if(AllGate[POID[i]]->invertL==false){
               bitset<32> bit(AllGate[POID[i]]->faninL->value[j]);
               str[i]+=bit.to_string();
            }
         }
      }
      for(size_t j=0;j<pattern.size();j++){
         *logFile<<pattern[j]<<" ";
         for(size_t k=0;k<a[3];k++)
            *logFile<<str[k][((pattern.size()+31)/32)*32-1-j];
         *logFile<<endl;
      }
   }
} 
void
CirMgr::printSummary() const
{
   int ANum=0;
   map<int,CirGate*>::const_iterator it;
   for(it=AllGate.begin();it!=AllGate.end();it++){
      if(it->second!=0&&(it->second)->Type=="AIG")
         ANum++;
   }
   cout<<"Circuit Statistics"<<endl
       <<"=================="<<endl
       <<"  PI"<<setw(12)<<right<<a[1]<<endl
       <<"  PO"<<setw(12)<<right<<a[3]<<endl
       <<"  AIG"<<setw(11)<<right<<ANum<<endl//need to change!!!!!!!!!!!!!!!!!!!!!!!
       <<"------------------"<<endl
       <<"  Total"<<setw(9)<<right<<a[1]+a[3]+ANum<<endl;
}

void
CirMgr::printNetlist() const
{
   cout<<endl;
   for(size_t i=0;i<Netlist.size();i++)
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
      if(it->second!=0&&(it->second)->Type=="PO")
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
      if(it->second!=0&&(it->second)->faninL!=NULL&&(it->second)->faninR!=NULL){
         if((it->second)->faninL->Type=="UNDEF"||(it->second)->faninR->Type=="UNDEF"){
            if(b1==false){
               b1=true;
               cout<<"Gates with floating fanin(s):";
            }
            cout<<" "<<(it->second)->GateID;
         }
      }
      else if(it->second!=0&&(it->second)->faninL!=NULL&&(it->second)->Type=="PO"){
         if((it->second)->faninL->Type=="UNDEF"){
            if(b1==false){
               b1=true;
               cout<<"Gates with floating fanin(s):";
            }
            cout<<" "<<(it->second)->GateID;
         }
      }
   }
   for(it=AllGate.begin();it!=AllGate.end();it++){
      if(it->second!=0&&((it->second)->Type=="AIG"||(it->second)->Type=="PI")){
         if(((it->second)->fanout).size()==0){
            if(b2==false){
               b2=true;
               if(b1==true)
                  cout<<endl;
               cout<<"Gates defined but not used  :";
            }
            cout<<" "<<(it->second)->GateID;
         }
      }
   }
   if(b1!=false||b2!=false)
      cout<<endl;
}

void
CirMgr::printFECPairs() const
{
   size_t t=0;
   for(size_t i=0;i<FECGroup.size();i++){
      if(FECGroup[i][0].first()==FECGroup[i][0].second->value[patternID-1]){
         cout<<"["<<t<<"] ";
         for(size_t j=0;j<FECGroup[i].size();j++){
            if(FECGroup[i][j].first()!=FECGroup[i][j].second->value[patternID-1])
               cout<<'!';
            cout<<FECGroup[i][j].second->GateID<<' ';
         }
         cout<<endl;
         t++;
      }
   }
}

void
CirMgr::writeAag(ostream& outfile) 
{
   outfile<<"aag ";
   for(int i=0;i<4;i++)
      outfile<<a[i]<<' ';
   outfile<<AndGateNum<<endl;
   for(int i=1;i<1+a[1];i++)
      outfile<<input[i]<<endl;
   for(int i=0;i<a[3];i++){
      size_t j=0;
      if(AllGate[a[0]+1+i]->invertL)
         j=1;
      outfile<<2*(AllGate[a[0]+1+i]->faninL->GateID)+j<<endl;
   }
   for(size_t i=0;i<sortGate.size();i++)
      outfile<<sortGate[i]<<endl;
   for(size_t i=0;i<Symb.size();i++)
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
   if(c->Type=="CONST"){
      if(doSim){
         c->value=new size_t [(pattern.size()+31)/32];
         for(int i=0;i<(pattern.size()+31)/32;i++)
            c->value[i]=0;
         if(!const_inFEC){
            HashKey _k(0),_ik(~0);
            pair<HashKey,CirGate*> _p(_k,c),_ip(_ik,c);
            _sim.insert(_p);     
            _sim.insert(_ip);
            const_inFEC=true;
         }
      }
      if(doFraig){
         AIGID.push_back(c->GateID);
         Var v=solver.newVar();
         c->_var=v;
         solver.assertProperty(v,false);
      }
      Netlist.push_back("CONST0");
   }
   else if(c->Type=="PI"){
      str="PI  ";
      str+=int2str(c->GateID);
      if(c->Symbol.size()!=0){
         str+=" (";
         str+=c->Symbol;
         str+=")";
      }
      if(doFraig){
         Var v=solver.newVar();
         c->_var=v;
      }
      Netlist.push_back(str);
   }
   else if(c->Type=="AIG"){
      if(doOpt){
         if(doOptimize(c)){
            map<int,CirGate*>::iterator it=AllGate.find(c->GateID);
            delete c;
            it->second=0;
         }
      }
      else if(doStr){
         doStrash(c);
      }
      else{
         AndGateNum++;
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
      if(doSim){
         c->value=new size_t [(pattern.size()+31)/32];
         for(int i=0;i<(pattern.size()+31)/32;i++){
            if(c->invertL==true&&c->invertR==true)
               c->value[i]=(~(c->faninL->value[i]))&(~(c->faninR->value[i]));
            else if(c->invertL==true&&c->invertR==false)
               c->value[i]=(~(c->faninL->value[i]))&(c->faninR->value[i]);
            else if(c->invertL==false&&c->invertR==true)
               c->value[i]=(c->faninL->value[i])&(~(c->faninR->value[i]));
            else if(c->invertL==false&&c->invertR==false)
               c->value[i]=(c->faninL->value[i])&(c->faninR->value[i]);
         }
         if(!havedoSim){
            HashKey _k(c->value[0]),_ik(~c->value[0]);
            pair<HashKey,CirGate*> _p(_k,c),_ip(_ik,c);
            _sim.insert(_p);
            _sim.insert(_ip);
         }
      }
      if(doFraig){
         AIGID.push_back(c->GateID);
         Var v=solver.newVar();
         c->_var=v;
         solver.addAigCNF(c->_var,c->faninL->_var,c->invertL,c->faninR->_var,c->invertR);
      }
   }  
   if(doOpt==false&&doStr==false&&c->fanout[0]->Type=="PO"&&c->fanout[0]->isGlobalRef()){
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
      if(doFraig){
         c->fanout[0]->_var=c->_var;
      }
      Netlist.push_back(str);
   }
}





