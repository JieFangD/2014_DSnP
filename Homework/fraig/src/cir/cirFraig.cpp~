/****************************************************************************
  FileName     [ cirFraig.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir FRAIG functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "sat.h"
#include "myHashMap.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::strash()" and "CirMgr::fraig()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
void 
CirMgr::merge(CirGate* c,CirGate* r,string str,bool invert){
   cout<<str<<c->GateID<<" merging ";
   if(invert)
      cout<<'!';   
   cout<<r->GateID<<"..."<<endl;
   cutfaninL(r);
   cutfaninR(r);
   for(int i=0;i<(r->fanout).size();i++){
      if(r->fanout[i]->faninL==r)
         r->fanout[i]->faninL=c;
      else
         r->fanout[i]->faninR=c;
      c->fanout.push_back(r->fanout[i]);
   }
   map<int,CirGate*>::iterator it=AllGate.find(r->GateID);
   delete r;
   it->second=0;
}

void 
CirMgr::doStrash(CirGate* c){
   size_t _num=0,_numL=0,_numR=0;
   if(c->invertL)
      _numL=(c->faninL)->GateID+1;
   else
      _numL=(c->faninL)->GateID;
   if(c->invertR)
      _numR=(c->faninR)->GateID+1;
   else
      _numR=(c->faninR)->GateID;
   _num=_numL*_numR;
   HashKey _k(_num);
   pair<HashKey,CirGate*> _p(_k,c);
   if(_h.query(_p))
      merge(_p.second,c,"Strashing: ",false);
   else
      _h.insert(_p);
}

/*******************************************/
/*   Public member functions about fraig   */
/*******************************************/
void
CirMgr::strash()
{
   _h.init(AndGateNum*4);
   doStr=true;
   makeNetlist();
   doStr=false;
   makeNetlist();
}

void
CirMgr::sat(size_t pos)
{
   vector<FECtype> newFEC;
   newFEC.clear();
   bool result;
   for(size_t i=1;i<FECGroup[pos].size();i++){
      Var newV=solver.newVar();
      if(FECGroup[pos][0].first()==FECGroup[pos][0].second->value[patternID-1])
         solver.addXorCNF(newV,AllGate[FECGroup[pos][0].second->GateID]->_var,true,AllGate[FECGroup[pos][i].second->GateID]->_var,true);
      else if(FECGroup[pos][0].first()==~(FECGroup[pos][0].second->value[patternID-1]))
         solver.addXorCNF(newV,AllGate[FECGroup[pos][0].second->GateID]->_var,true,AllGate[FECGroup[pos][i].second->GateID]->_var,false);
      solver.assumeRelease();
      if(FECGroup[pos][i].first()==FECGroup[pos][i].second->value[patternID-1])
         solver.assumeProperty(newV,true);
      else if(FECGroup[pos][i].first()==~(FECGroup[pos][i].second->value[patternID-1]))
         solver.assumeProperty(newV,false);
      result=solver.assumpSolve();
      if(result){
         newFEC.push_back(FECGroup[pos][i]);
         FECGroup[pos].erase(FECGroup[pos].begin()+i);
         i--;
      }
   }
   if(newFEC.size()>1)
      FECGroup.push_back(newFEC);
}
   
   
void
CirMgr::fraig()
{
   solver.initialize();
   doFraig=true;
   AIGID.clear();
   makeNetlist();
   doFraig=false;
   
   //for(size_t i=0;i<FECGroup.size();i++)
   //   sat(i);
   for(size_t i=0;i<FECGroup.size();i++){
      if(FECGroup[i].size()<2){
         FECGroup.erase(FECGroup.begin()+i);
         i--;
      }
   }
   for(size_t num=0;num<AIGID.size();num++){
      for(size_t i=0;i<FECGroup.size();i++){
         if(FECGroup[i][0].first()==AllGate[AIGID[num]]->value[patternID-1]||FECGroup[i][0].first()==~AllGate[AIGID[num]]->value[patternID-1]){
            int j;
            size_t s;
            for(j=0;j<num;j++){
               for(s=0;s<(pattern.size()+31)/32;s++){
                  if(AllGate[AIGID[j]]->value[s]!=AllGate[AIGID[num]]->value[s]&&AllGate[AIGID[j]]->value[s]!=~AllGate[AIGID[num]]->value[s])
                     break;
               }
               if(s==(pattern.size()+31)/32)
                  break;
            }
            if(s==(pattern.size()+31)/32&&j!=num){
               if(AllGate[AIGID[j]]->value[patternID-1]==AllGate[AIGID[num]]->value[patternID-1])
                  merge(AllGate[AIGID[j]],AllGate[AIGID[num]],"Fraig: ",false);
               else if(AllGate[AIGID[j]]->value[patternID-1]==~AllGate[AIGID[num]]->value[patternID-1])
                  merge(AllGate[AIGID[j]],AllGate[AIGID[num]],"Fraig: ",true);
               AIGID.erase(AIGID.begin()+num);
               num--;
            }
            else if(s==(pattern.size()+31)/32&&j==num){
               sat(i);
               for(size_t k=0;k<FECGroup.size();k++){
                   if(FECGroup[k][0].first()==~AllGate[AIGID[num]]->value[patternID-1])
                      FECGroup.erase(FECGroup.begin()+k);
               }
            }
            break;
         }
      }
   }
   FECGroup.clear();
   //cout<<"Updating by UNSAT... Total #FEC Group = "<<FECGroup.size()<<endl;
   makeNetlist();
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/

