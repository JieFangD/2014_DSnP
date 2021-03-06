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

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
   CirMgr(): foundInput(false),ANum(0) {}
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

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) ;
   bool handleError(vector<string>);
   bool handleInput();
   bool legalOutput(CirGate*);
   void dfsTraversal(CirGate*);

private:
   vector<string>    input;
   vector<string>    sortGate;
   vector<string>    Netlist;
   vector<string>    Symb;
   bool              foundInput;
   map<int,CirGate*> AllGate;
   int               a[5];      //M,I,L,O,A
   int               ANum;
};

#endif // CIR_MGR_H
