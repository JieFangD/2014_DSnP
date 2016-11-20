/****************************************************************************
  FileName     [ calcGNum.cpp ]
  PackageName  [ calc ]
  Synopsis     [ Define member functions for class GNum ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <ctype.h>
#include <cassert>
#include "util.h"
#include "calcGNum.h"

// TODO: Initialize the static data members of class GNum
//       (Note: let default _base = 10)
// TODO: Define the member functions of class GNum
int site;
char cnum[65536]={'0'};
char numtochar(int);
int GNum::_base=10;
CalcMap GNum::_varMap;
bool GNum::getVarVal(const string& s, GNum& n) {
   CalcMap::iterator it;
   it=_varMap.find(s);      
   if(it==_varMap.end())
      return false;
   else{
      n=_varMap[s];
      return true;
   }
}
bool selfStr2Int(const string& str, int& num,int base)
{
   num = 0; 
   size_t i = 0;
   int sign = 1; 
   if (str[0] == '-') { sign = -1; i = 1; }
   bool valid = false;
   for (; i < str.size(); ++i) {
      if (48<=int(str[i])&&int(str[i])<=57) {
         num *= base;
         num += int(str[i] - '0');
         valid = true;
      }
      else if(97<=int(str[i])&&int(str[i])<=122){
         num*=base;
         num+=int(str[i]-'a'+10);
	  valid=true;
      }
      else
         return false;
   }
   num *= sign;
   return valid;
}

   
bool GNum::getStrVal(const string& s, GNum& n) {   
   int num=0,num1=0;
   if(isValidVarName(s)){
      if(getVarVal(s,n))
         return true;   
   }
   if(s[0]=='#'){
      for(unsigned i=1;i<s.size();i++){
         selfStr2Int(s.substr(i,1),num1,10);
         if(num1>=n._base||(-1*num1)>=n._base)
 	    return false;
      }
      if(selfStr2Int(s.substr(1),num,n._base)){
	  n._num=num;
         return true;
      }
   }
   return false;       	 
}
void Change(int num,int base){
   int in=0;
   for(int i=0;i<65536;i++)
      cnum[i]='0';
   while((num/base)>0){
      in=num%base;
      cnum[site]=numtochar(in);
      num/=base;
      site++;
   }
   cnum[site]=numtochar(num);
}     
char numtochar(int i){
   switch(i){
      case 0:
         return '0';
      case 1:
         return '1';
      case 2:
         return '2';
      case 3:
         return '3';
      case 4:
         return '4';
      case 5:
         return '5';
      case 6:
         return '6';
      case 7:
         return '7';
      case 8:
         return '8';
      case 9:
         return '9';
      case 10:
         return 'a';
      case 11:
         return 'b';
      case 12:
         return 'c';
      case 13:
         return 'd';
      case 14:
         return 'e';
      case 15:
         return 'f';
      case 16:
         return 'g';
      case 17:
         return 'h';
      case 18:
         return 'i';
      case 19:
         return 'j';
      case 20:
         return 'k';
      case 21:
         return 'l';
      case 22:
         return 'm';
      case 23:
         return 'n';
      case 24:
         return 'o';
      case 25:
         return 'p';
      case 26:
         return 'q';
      case 27:
         return 'r';
      case 28:
         return 's';
      case 29:
         return 't';
      case 30:
         return 'u';
      case 31:
         return 'v';
      case 32:
         return 'w';
      case 33:
         return 'x';
      case 34:
         return 'y';
      case 35:
         return 'z';
   }
}
ostream& operator << (ostream& os, const GNum& n) {
   os<<'#';
   int x=n._num;
   if(n._num<0){
      cout<<'-';
      x*=-1;
   }
   Change(x,n._base);
   for(int i=site;i>=0;i--)
      cout<<cnum[i];
   site=0;
   return os;
}

