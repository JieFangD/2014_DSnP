/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir simulation functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <sstream>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Keep "CirMgr::randimSim()" and "CirMgr::fileSim()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
void 
CirMgr::doSimulation()
{
   int i=0;
   size_t j,_in[(pattern.size()+31)/32];
   //FECGroup.clear();
   map<int,CirGate*>::iterator it;
   for(it=AllGate.begin();it!=AllGate.end(),i<a[1];it++){   //pattern to PI 
      if(it->second!=0&&(it->second)->Type=="PI"){
         (it->second)->value=new size_t [(pattern.size()+31)/32];
         for(j=0;j<(pattern.size()+31)/32;j++){
            (it->second)->value[j]=0;
            _in[j]=0;
         }
         for(j=0;j<pattern.size();j++)
            _in[j/32]+=((pattern[j][i]-'0')<<(j%32));
         for(j=0;j<(pattern.size()+31)/32;j++){
            (it->second)->value[j]=(it->second)->value[j]|_in[j];
            _in[j]=0;
         }
         i++;
      }
   }
   if(!havedoSim)
      _sim.init(AndGateNum*2);
   doSim=true;
   makeNetlist();
   doSim=false;
   for(patternID=0;patternID<(pattern.size()+31)/32;patternID++){
      if(!havedoSim)
         _sim.getFEC(FECGroup);
      else if(havedoSim){
         vector< vector<FECtype> > tmp;
         //tmp.clear();
         for(size_t i=0;i<FECGroup.size();i++){
            _sim.init(FECGroup[i].size()*5);
            for(size_t j=0;j<FECGroup[i].size();j++){
               if(FECGroup[i][j].first()==FECGroup[i][j].second->value[patternID-1])
                  FECGroup[i][j].first=FECGroup[i][j].second->value[patternID];
               else if(FECGroup[i][j].first()==~(FECGroup[i][j].second->value[patternID-1]))
                  FECGroup[i][j].first=~(FECGroup[i][j].second->value[patternID]);
               _sim.insert(FECGroup[i][j]);
            }  
            //cout<<FECGroup[i][j].first()<<"   ";}
            //cout<<"!!!"<<endl;//_sim.print();
            _sim.getFEC(tmp);
         }
         //FECGroup.clear();
         FECGroup=tmp;
      }  
      havedoSim=true;
      /*for(size_t i=0;i<FECGroup.size();i++){
         for(size_t j=0;j<FECGroup[i].size();j++)
            cout<<FECGroup[i][j].second->GateID<<"   ";
         cout<<FECGroup[i].size()<<endl;*/

        // if(FECGroup[i].size()==1){
        //    FECGroup.erase(FECGroup.begin()+i);
        //    i--;
        // }
      
   }
   if(dosimlog){
      if(_simLog!=0)
         doSimLog(_simLog);
      _simLog=0;
      dosimlog=false;
   }
     // cout<<FECGroup[i][j].first<<"   "<<FECGroup[i].size()<<"   "<<endl;
   
   //fraig();
   
   //_sim.print();
}
/************************************************/
/*   Public member functions about Simulation   */
/************************************************/
void
CirMgr::randomSim()
{
   RandomNumGen _r;
   pattern.clear();
   string str;
   int num=0;
   while(num*num*num*num*num<a[1])
      num++;
   if(num==1)
      num++;
   for(int i=0;i<((num-1)*(num-1)*6-4)*32;i++){
      str="";
      for(int j=0;j<a[1];j++)
         str+=(_r(2)+'0');
      pattern.push_back(str);
      //cout<<pattern[i]<<endl;
   }
   doSimulation();
   cout<<pattern.size()<<" patterns simulated."<<endl;
   //pattern.resize(0);
}

void
CirMgr::fileSim(ifstream& patternFile)
{
   string str;
   bool suc=true;
   pattern.clear();
   while(getline(patternFile,str)){
      while(str.size()>0&&str[0]==' ')
         str=str.substr(1);
      suc=true;
      if(str.size()!=0&&str.size()!=a[1]){
         cout<<"\nError: Pattern("<<str<<") length("<<str.size()<<") does not match the number of inputs("<<a[1]<<") in a circuit!!"<<endl
             <<"0 patterns simulated."<<endl;
         suc=false;
         break;
      }
      else if(str.size()!=0&&str.size()==a[1]){
         for(size_t i =0;i<str.size();i++){
            if(str[i]!='1'&&str[i]!='0'){
               cout<<"\nError: Pattern("<<str<<") contains a non-0/1 character('"<<str[i]<<"')."<<endl
                   <<"0 patterns simulated."<<endl;
               suc=false;
               break;
            }
         }
         if(!suc)
            break;
      }
      if(str.size()!=0)
         pattern.push_back(str);
   }
   if(suc){
      doSimulation();
      cout<<pattern.size()<<" patterns simulated."<<endl;
   }
   //for(size_t i=0;i<pattern.size();i++)
     // cout<<pattern[i]<<endl;
}

/*************************************************/
/*   Private member functions about Simulation   */
/*************************************************/

