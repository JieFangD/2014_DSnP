/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include  "sat.h"

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
typedef pair<HashKey,CirGate*> FECtype;

public:
   CirMgr(): AndGateNum(0),doOpt(false),doStr(false),doSim(false),dosimlog(false),doFraig(false),havedoSim(false),const_inFEC(false),patternID(0) {solver.initialize();}
   ~CirMgr() {}

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) { 
      if(AllGate.find(gid)==AllGate.end())
         return 0;
      return AllGate[gid]; 
   }
   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit optimization
   void sweep();
   void cutfaninL(CirGate*);
   void cutfaninR(CirGate*);
   void optimize();
   bool doOptimize(CirGate*);
   void setTo0(CirGate*);

   // Member functions about simulation
   void randomSim();
   void fileSim(ifstream&);
   void doSimulation();
   void setSimLog(ofstream *logFile){ 
      if(logFile!=0){
         _simLog=logFile;
         dosimlog=true;
      }
   }
   void doSimLog(ofstream*);

   // Member functions about fraig
   void strash();
   void doStrash(CirGate*);
   void merge(CirGate*,CirGate*,string,bool);
   void printFEC() const;
   void fraig();
   void sat(size_t);
   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) ;
   void handleInput();
   void makeNetlist();
   void dfsTraversal(CirGate*);
   void printFECPairs() const;

   vector< vector<FECtype> >   FECGroup;
   vector<string>              pattern;
   int                         patternID;

private:
   vector<string>              input;
   vector<string>              sortGate;  //dfs AndGate priority
   vector<string>              Netlist;
   vector<string>              Symb;
   map<int,CirGate*>           AllGate;
   int                         a[5];      //M,I,L,O,A
   int*                        POID;
   vector<int>                 AIGID;
   int                         AndGateNum;
   bool                        doOpt;
   bool                        doStr;
   bool                        doSim;
   bool                        dosimlog;
   bool                        doFraig;
   bool                        havedoSim;
   bool                        const_inFEC;
   HashMap<HashKey,CirGate*>   _h;
   HashMap<HashKey,CirGate*>   _sim;
   SatSolver                   solver;   
   ofstream*                   _simLog;

};

#endif // CIR_MGR_H
