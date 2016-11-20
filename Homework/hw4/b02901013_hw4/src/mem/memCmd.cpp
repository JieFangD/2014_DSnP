/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(sizeof(MemTestObj))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
   // TODO
   vector<string> options;
   int num;
   size_t n,s;
   bool flag=false;
   lexOptions(option,options);
   try{
   for(int i=0;i<options.size();i++){
      if(!myStrNCmp("-Array",options[i],2))
         flag=true;
      else if(!myStr2Int(options[i],num))
         return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
   }
   if(options.size()>3||options.size()==0){
      lexOptions(option,options,1);
      return CMD_EXEC_ERROR;
   }
   if(options.size()==1){
      if(flag==true){
         cout<<"Missing option after (-a)!!"<<endl; 
         return CMD_EXEC_DONE;
      }
      else{
	 mtest.newObjs(num);
      }
   }
   else if(options.size()==2){
      if(flag==true){
         cout<<"Missing option after (-a)!!"<<endl; 
         return CMD_EXEC_DONE;
      }
      else{
         lexOptions(option,options,1);
         return CMD_EXEC_ERROR;
      }
   }
   else{
      if(flag==false){
         lexOptions(option,options,1);
         return CMD_EXEC_ERROR;
      }
      if(myStr2Int(options[0],num)){
         n=num;
	 if(myStr2Int(options[2],num)){
	    s=num;
	    if(num==0)
               return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[1]);
	 }   
	 else{
	    cout<<"Extra option!! ("<<options[1]<<")\n"; 
    	    return CMD_EXEC_DONE;
	 }
      }
      else if(myStr2Int(options[1],num)){
	 s=num;
	 if(num==0)
            return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[1]);
	 myStr2Int(options[2],num);
	 n=num;
      }
      mtest.newArrs(n,s);
   }
   }
   catch(bad_alloc){
   }
   return CMD_EXEC_DONE;
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
   // TODO
   vector<string> options;
   int num;
   size_t nn=0;
   bool flag=false,irflag=false;
   lexOptions(option,options);
   for(int i=0;i<options.size();i++){
      if(!myStrNCmp("-Array",options[i],2)){
         flag=true;
         if(options.size()>i+1){
            if(myStrNCmp("-Index",options[i+1],2)&&myStrNCmp("-Random",options[i+1],2))
               return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i+1]);
         }
      }
      else if((!myStrNCmp("-Index",options[i],2))||(!myStrNCmp("-Random",options[i],2))){
	 irflag=true;
         if(options.size()>i+1){
	    if(!myStr2Int(options[i+1],num))
               return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i+1]);
	 }
      }
      else if(!myStr2Int(options[i],num))
         return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
      else if(num<0)
         return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
      else if(myStr2Int(options[i],num)){
	 if(i==0)
	    return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
         if(options.size()>i+1){
            if(!myStrNCmp("-Index",options[i+1],2)||!myStrNCmp("-Random",options[i+1],2))
	       return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
	    else if(myStr2Int(options[i+1],num))
	       return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i+1]);
         }
      }
   }
   if(irflag==false){
      cout<<"Missing option!!"<<endl;
      return CMD_EXEC_DONE;
   }
   if(options.size()==1){
      if(irflag==true){
         cout<<"Missing option after ("<<options[0]<<")!!"<<endl; 
         return CMD_EXEC_DONE;
      }
   }
   else if(options.size()==2){
      if(flag==true&&irflag==true){
         cout<<"Missing option after ("<<options[1]<<"-a)!!"<<endl; 
         return CMD_EXEC_DONE;
      }
      else{
	 myStr2Int(options[1],num);
         if(!myStrNCmp("-Index",options[0],2)){
	    if(num>mtest.getObjListSize()-1||mtest.getObjListSize()==0){
	       cout<<"Size of object list ("<<mtest.getObjListSize()<<") is <= "<<num<<"!!\n"; 
	       return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[1]);
	    }
	    mtest.deleteObj(num);
	 }
	 else{
	    if(mtest.getObjListSize()==0){
	       cout<<"Size of object list is 0!!\n";
	       return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[0]);
	    }
	    for(int i=0;i<num;i++)
	       mtest.deleteObj(rnGen(mtest.getObjListSize())); 
         }
      }
   }
   else{
      if(flag==false){
         lexOptions(option,options,1);
         return CMD_EXEC_ERROR;
      }
      else{
         if(!myStrNCmp("-Array",options[0],2)){
	    myStr2Int(options[2],num);
            if(!myStrNCmp("-Index",options[1],2)){
	       if(num>mtest.getArrListSize()-1||mtest.getArrListSize()==0){
	          cout<<"Size of array list ("<<mtest.getArrListSize()<<") is <= "<<num<<"!!\n"; 
	          return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[2]);
	       }
	       mtest.deleteArr(num);
	    }
	    else{
	       if(mtest.getArrListSize()==0){
	          cout<<"Size of array list is 0!!\n";
	          return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[1]);
	       }
	       nn=mtest.getArrListSize();
               for(int i=0;i<num;i++)
	          mtest.deleteArr(rnGen(nn)); 
	    }
	 }
	 else{
	    myStr2Int(options[1],num);
            if(!myStrNCmp("-Index",options[0],2)){
	       if(num>mtest.getArrListSize()-1||mtest.getArrListSize()==0){
	          cout<<"Size of array list ("<<mtest.getArrListSize()<<") is <= "<<num<<"!!\n"; 
	          return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[1]);
	       }
	       mtest.deleteArr(num);
	    }
	    else{
	       if(mtest.getArrListSize()==0){
	          cout<<"Size of array list is 0!!\n";
	          return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[0]);
	       }
	       nn=mtest.getArrListSize();
	       for(int i=0;i<num;i++)
	          mtest.deleteArr(rnGen(nn)); 
	    }
	 }
      }  
   }   
   return CMD_EXEC_DONE;
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


