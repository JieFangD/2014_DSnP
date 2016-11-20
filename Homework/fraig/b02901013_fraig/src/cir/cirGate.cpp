/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include <bitset>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate()
{
   ostringstream data;
   cout<<"=================================================="<<endl;
   data<<"= "<<Type<<'('<<GateID<<')';
   if(Symbol.size()!=0)
      data<<"\""<<Symbol<<"\"";
   data<<", line "<<LineNo;
   cout<<setw(49)<<left<<data.str()<<right<<'='<<endl;
   data.str("");
   data<<"= FECs: ";
   if(Type=="AIG"){
      for(size_t i=0;i<cirMgr->FECGroup.size();i++){
         if(cirMgr->FECGroup[i][0].first==value[cirMgr->patternID-1]){
            for(size_t j=0;j<cirMgr->FECGroup[i].size();j++){
               if(cirMgr->FECGroup[i][j].second!=this){
                  if(cirMgr->FECGroup[i][j].first()!=cirMgr->FECGroup[i][j].second->value[cirMgr->patternID-1])
                     data<<'!';
                  data<<cirMgr->FECGroup[i][j].second->GateID<<' ';
               }
            }
            break;
         }
      }
   }
   cout<<setw(49)<<left<<data.str()<<right<<'='<<endl;
   data.str("");
   data<<"= Value: ";
   bitset<32> bit(value[(cirMgr->pattern.size()+31)/32-1]);
   string str=bit.to_string();
   for(size_t i=0;i<32;i++){
      data<<str[i];
      if(i!=31&&i%4==3)
         data<<"_";
   }
   cout<<setw(49)<<left<<data.str()<<right<<'='<<endl
       <<"=================================================="<<endl;
}

void
CirGate::reportFanin(int level) 
{
   assert (level >= 0);
   CirGate::setGlobalRef();
   this->dfsTraversalD(this,level,0,false,false);
}

void
CirGate::reportFanout(int level) 
{
   assert (level >= 0);
   CirGate::setGlobalRef();
   this->dfsTraversalU(this,level,0,false,false);
}
void 
CirGate::dfsTraversalU(CirGate* c,int l,int n,bool invert,bool reprint)
{
   for(int i=0;i<2*n;i++)
      cout<<' ';
   if(invert==true)
      cout<<'!';
   if(reprint==true)
      cout<<c->Type<<' '<<c->GateID<<" (*)"<<endl;
   else
      cout<<c->Type<<' '<<c->GateID<<endl;
   if(l>n)
      c->setToGlobalRef();
   if(l>n&&!reprint){
      for(int i=0;i<c->fanout.size();i++){
         if(!c->fanout[i]->isGlobalRef()){
            if(c->fanout[i]->faninL==c)
               invert=c->fanout[i]->invertL;
            else if(c->fanout[i]->faninR==c)
               invert=c->fanout[i]->invertR;
            dfsTraversalU(c->fanout[i],l,n+1,invert,reprint);
         }
         else if(c->fanout[i]->isGlobalRef()){
            if(n<l-1)
               reprint=true;
            if(c->fanout[i]->faninL==c->fanout[i]->faninR)
               invert=c->fanout[i]->invertR;
            else{
               if(c->fanout[i]->faninL==c)
                  invert=c->fanout[i]->invertL;
               else if(c->fanout[i]->faninR==c)
                  invert=c->fanout[i]->invertR;
            }
            dfsTraversalU(c->fanout[i],l,n+1,invert,reprint);
            reprint=false;
         }
      }
   }
}
void 
CirGate::dfsTraversalD(CirGate* c,int l,int n,bool invert,bool reprint)
{
   for(int i=0;i<2*n;i++)
      cout<<' ';
   if(invert==true)
      cout<<'!';
   if(reprint==true)
      cout<<c->Type<<' '<<c->GateID<<" (*)"<<endl;
   else
      cout<<c->Type<<' '<<c->GateID<<endl;
   if(l>n)
      c->setToGlobalRef();
   if(l>n&&!reprint){
      if(c->faninL!=NULL&&!c->faninL->isGlobalRef()){
         invert=c->invertL;
         dfsTraversalD(c->faninL,l,n+1,invert,reprint);
      }
      else if(c->faninL!=NULL&&c->faninL->isGlobalRef()){
         if(n<l-1&&(c->faninL->Type!="PI"&&c->faninL->Type!="CONST"))
            reprint=true;
         invert=c->invertL;
         dfsTraversalD(c->faninL,l,n+1,invert,reprint);
         reprint=false;
      }
      if(c->faninR!=NULL&&!c->faninR->isGlobalRef()){
         invert=c->invertR;
         dfsTraversalD(c->faninR,l,n+1,invert,reprint);
      }
      else if(c->faninR!=NULL&&c->faninR->isGlobalRef()){
         if(n<l-1&&(c->faninR->Type!="PI"&&c->faninR->Type!="CONST"))
            reprint=true;
         invert=c->invertR;
         dfsTraversalD(c->faninR,l,n+1,invert,reprint);
         reprint=false;
      }
   }
}








