/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir optimization functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::sweep()" and "CirMgr::optimize()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
void
CirMgr::cutfaninL(CirGate* c){
   if(c->faninL==NULL)
      return;
   else{
      for(size_t i=0;i<((c->faninL)->fanout).size();i++){
         if((c->faninL)->fanout[i]==c){
            (c->faninL)->fanout.erase((c->faninL)->fanout.begin()+i);
            return;
         }
      }
   }
}
void 
CirMgr::cutfaninR(CirGate* c){
   if(c->faninR==NULL)
      return;
   else{
      for(size_t i=0;i<((c->faninR)->fanout).size();i++){
         if((c->faninR)->fanout[i]==c){
            (c->faninR)->fanout.erase((c->faninR)->fanout.begin()+i);
            return;
         }
      }
   }
}
void cutfanout(CirGate* c){
   for(size_t i=0;i<(c->fanout).size();i++){
      if(c->fanout[i]->faninL==c)
         c->fanout[i]->faninL=NULL;
      else
         c->fanout[i]->faninR=NULL;
   }
}
void
CirMgr::setTo0(CirGate* c){
   for(size_t i=0;i<(c->fanout).size();i++){
      if(AllGate[0]==NULL){
         CirGate* newGate=new CirGate;
         newGate->Type="CONST";
         newGate->LineNo=0;
         newGate->GateID=0;
         AllGate[0]=newGate;
      }
      if(c->fanout[i]->faninL==c)
         c->fanout[i]->faninL=AllGate[0];
      else
         c->fanout[i]->faninR=AllGate[0];
      AllGate[0]->fanout.push_back(c->fanout[i]);
   }
}

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
void
CirMgr::sweep()
{
   makeNetlist();
   map<int,CirGate*>::iterator it;
   for(it=AllGate.begin();it!=AllGate.end();++it){
      if(it->second!=0&&!(it->second)->isGlobalRef()&&(it->second)->Type!="PI"&&(it->second)->Type!="CONST"){
         cout<<"Sweeping: "<<(it->second)->Type<<'('<<(it->second)->GateID<<") removed..."<<endl;
         cutfaninL(it->second);
         cutfaninR(it->second);
         cutfanout(it->second);
         delete (it->second);
         it->second=0;
      }
   }
}

void
CirMgr::optimize()
{
   doOpt=true;
   makeNetlist();
   doOpt=false;
   makeNetlist();
}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/
bool
CirMgr::doOptimize(CirGate* c){
   if(c->faninL==c->faninR&&c->invertL==c->invertR){//two same input
      cout<<"Simplifying: "<<c->faninL->GateID<<" merging ";
      if(c->invertL)
         cout<<'!';
      cout<<c->GateID<<"..."<<endl;
      for(size_t i=0;i<c->fanout.size();i++){
         if(c->fanout[i]->faninL==c){
            c->fanout[i]->faninL=c->faninL;
            if(c->invertL==true)
               c->fanout[i]->invertL=!(c->fanout[i]->invertL);
         }
         else{
            c->fanout[i]->faninR=c->faninL;
            if(c->invertL==true)
               c->fanout[i]->invertR=!(c->fanout[i]->invertR);
         }
         (c->faninL->fanout).push_back(c->fanout[i]);
      }
      cutfaninL(c);
      cutfaninR(c);
      return true;
   }
   else if(c->faninL==c->faninR&&c->invertL!=c->invertR){
      cout<<"Simplifying: 0 merging "<<c->GateID<<"..."<<endl;
      setTo0(c);
      cutfaninL(c);
      cutfaninR(c);
      return true;
   }  
   else if((c->faninL)->Type=="CONST"){//handle aig's faninL=0 or 1
      if(c->invertL==false){
         cout<<"Simplifying: 0 merging "<<c->GateID<<"..."<<endl;
         setTo0(c);  
      }
      else{
         cout<<"Simplifying: "<<c->faninR->GateID<<" merging ";
         if(c->invertR)
            cout<<'!';
         cout<<c->GateID<<"..."<<endl;
         for(size_t i=0;i<(c->fanout).size();i++){
            if(c->fanout[i]->faninL==c){
               c->fanout[i]->faninL=c->faninR;
               if(c->invertR==true)
                  c->fanout[i]->invertL=!(c->fanout[i]->invertL);
            }
            else{
               c->fanout[i]->faninR=c->faninR;
               if(c->invertR==true)
                  c->fanout[i]->invertR=!(c->fanout[i]->invertR);
            }
            c->faninR->fanout.push_back(c->fanout[i]);
         }
      }
      cutfaninL(c);
      cutfaninR(c);
      return true;
   }
   else if((c->faninR)->Type=="CONST"){//handle aig's faninR=0 or 1
      if(c->invertR==false){
         cout<<"Simplifying: 0 merging "<<c->GateID<<"..."<<endl;
         setTo0(c);
      }
      else{
         cout<<"Simplifying: "<<c->faninL->GateID<<" merging ";
         if(c->invertL)
            cout<<'!';
         cout<<c->GateID<<"..."<<endl;
         for(size_t i=0;i<(c->fanout).size();i++){
            if(c->fanout[i]->faninL==c){
               c->fanout[i]->faninL=c->faninL;
               if(c->invertL==true)
                  c->fanout[i]->invertL=!(c->fanout[i]->invertL);
            }
            else{
               c->fanout[i]->faninR=c->faninL;
               if(c->invertL==true)
                  c->fanout[i]->invertL=!(c->fanout[i]->invertL);
            }
            c->faninL->fanout.push_back(c->fanout[i]);
         }
      }
      cutfaninL(c);
      cutfaninR(c);
      return true;
   }
   return false;
}





