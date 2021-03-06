/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"

using namespace std;

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{
public:
   CirGate() : LineNo(0),faninL(NULL),faninR(NULL),invertL(false),invertR(false),_ref(0),Type("UNDEF") {}
   ~CirGate() {}

   // Basic access methods
   string getTypeStr() const { return Type; }
   int getLineNo() const { return LineNo; }

   // Printing functions
   void printGate() const;
   void reportGate() const;
   void reportFanin(int level) ;
   void reportFanout(int level) ;
   void dfsTraversalD(CirGate*,int,int,bool,bool);
   void dfsTraversalU(CirGate*,int,int,bool,bool);
   bool isGlobalRef() { return (_ref==_gRef); }
   void setToGlobalRef() { _ref=_gRef; }
   static void setGlobalRef() { _gRef++; } 

   int              GateID;
   int              LineNo;
   string           Type;    
   string           Symbol;
   vector<CirGate*> fanout;
   CirGate*         faninL;
   CirGate*         faninR;
   bool             invertL;
   bool             invertR;

private:
   static unsigned  _gRef;
   unsigned         _ref;
};

#endif // CIR_GATE_H
