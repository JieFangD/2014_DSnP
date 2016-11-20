/****************************************************************************
  FileName     [ cmdReader.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command line reader member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <cstring>
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar checkChar(char, istream&);


//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------
void
CmdParser::readCmd()
{
   if (_dofile.is_open()) {
      readCmdInt(_dofile);
      _dofile.close();
   }
   else
      readCmdInt(cin);
}

void
CmdParser::readCmdInt(istream& istr)
{
   resetBufAndPrintPrompt();

   while (1) {
      char ch = mygetc(istr);
      ParseChar pch = checkChar(ch, istr);
      if (pch == INPUT_END_KEY) break;
      switch (pch) {
         case LINE_BEGIN_KEY :
         case HOME_KEY       : moveBufPtr(_readBuf); break;
         case LINE_END_KEY   :
         case END_KEY        : moveBufPtr(_readBufEnd); cout<<_readBufEnd;break;
         case BACK_SPACE_KEY : /* TODO */ 
			       if(_readBufPtr==_readBuf)
				  mybeep();
                               else{
                                  cout<<'\b';
			          _readBufPtr--;
			          deleteChar();
			       }
			       break;
         case DELETE_KEY     : deleteChar(); break;
         case NEWLINE_KEY    : if(_tempCmdStored){
			          _history.pop_back();
				  _tempCmdStored=false;
  			       }				
			       addHistory();
                               cout << char(NEWLINE_KEY);
                               resetBufAndPrintPrompt(); 
                               for(int i=0;i<65536;i++)
                                  _readBuf[i]='\0';
			       break;
         case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
         case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
         case ARROW_RIGHT_KEY: /* TODO */ 
                               moveBufPtr(_readBufPtr+1);
			       break;
         case ARROW_LEFT_KEY : /* TODO */ 
			       moveBufPtr(_readBufPtr-1);
			       break;
         case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
         case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
         case TAB_KEY        : /* TODO */ break;
         case INSERT_KEY     : // not yet supported; fall through to UNDEFINE
         case UNDEFINED_KEY  : mybeep(); break;
         default:  // printable character
            insertChar(char(pch)); break;
      }
      #ifdef TA_KB_SETTING
      taTestOnly();
      #endif
   }
}


// This function moves _readBufPtr to the "ptr" pointer
// It is used by left/right arrowkeys, home/end, etc.
//
// Suggested steps:
// 1. Make sure ptr is within [_readBuf, _readBufEnd].
//    If not, make a beep sound and return false. (DON'T MOVE)
// 2. Move the cursor to the left or right, depending on ptr
// 3. Update _readBufPtr accordingly. The content of the _readBuf[] will
//    not be changed
//
// [Note] This function can also be called by other member functions below
//        to move the _readBufPtr to proper position.
bool
CmdParser::moveBufPtr(char* const ptr)
{
   // TODO...
   if((ptr>=_readBuf)&&(ptr<=_readBufEnd)){;
      while(ptr!=_readBufPtr){
         if(ptr<_readBufPtr){
            _readBufPtr--;
	    cout<<'\b';
	 }
	 else if(ptr>_readBufPtr){
	    cout<<*_readBufPtr;
	    _readBufPtr++;
	 }
      }
      return true;
   }   	      
   else{
      mybeep();
      return false;
   }
}


// [Notes]
// 1. Delete the char at _readBufPtr
// 2. mybeep() and return false if at _readBufEnd
// 3. Move the remaining string left for one character
// 4. The cursor should stay at the same position
// 5. Remember to update _readBufEnd accordingly.
// 6. Don't leave the tailing character.
// 7. Call "moveBufPtr(...)" if needed.
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteChar()---
//
// cmd> This is he command
//              ^
//
bool
CmdParser::deleteChar()
{
   // TODO...
   char *nPtr=_readBufPtr;
   if(_readBufPtr!=_readBufEnd){
      while(_readBufPtr!=_readBufEnd){
         *_readBufPtr=*(_readBufPtr+1);
	 _readBufPtr++;
      }
      int i=0;
       _readBufEnd=(_readBufPtr-1);
      _readBufPtr=nPtr;
      while(nPtr!=_readBufEnd){
         cout<<*nPtr;
         nPtr++;
         i++;
      }
      cout<<" \b";
      while(i!=0){
         i--;
         cout<<'\b';
      }   
      return true;
   }
   else
      return false;
}

// 1. Insert character 'ch' at _readBufPtr
// 2. Move the remaining string right for one character
// 3. The cursor should move right for one position afterwards
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling insertChar('k') ---
//
// cmd> This is kthe command
//               ^
//
void
CmdParser::insertChar(char ch, int repeat)
{
   // TODO...
   if(_readBufPtr==_readBufEnd){
      *_readBufPtr=ch;
      cout<<*_readBufPtr;
      _readBufPtr++;
      _readBufEnd++;
   }
   else if(_readBufPtr!=_readBufEnd){
      char *nPtr=_readBufEnd;
      nPtr++;
      *nPtr=*_readBufEnd;
      while(_readBufEnd!=_readBufPtr){
         *_readBufEnd=*(_readBufEnd-1);
     	 _readBufEnd--;
      }
      *(_readBufEnd+1)=*_readBufPtr;
      _readBufEnd=nPtr;
      *_readBufPtr=ch;
      cout<<*_readBufPtr;
      _readBufPtr++;
      char *sPtr=_readBufPtr;
      int i=0;
      while(sPtr!=_readBufEnd){
         i++;
	 cout<<*sPtr;
	 sPtr++;
      }
      while(i!=0){
	 i--;
	 cout<<'\b';
      }	
   }	
}

// 1. Delete the line that is currently shown on the screen
// 2. Reset _readBufPtr and _readBufEnd to _readBuf
// 3. Make sure *_readBufEnd = 0
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteLine() ---
//
// cmd>
//      ^
//
void
CmdParser::deleteLine()
{
   // TODO...
   while(_readBufPtr!=_readBufEnd){
      cout<<*_readBufPtr;
      _readBufPtr++;
   }
   while(_readBufPtr!=_readBuf){
      cout<<" \b\b";
      _readBufPtr--;
   }
   cout<<" \b";
   _readBufPtr=_readBufEnd=_readBuf;
   *_readBufPtr=0;
}


// This functions moves _historyIdx to index and display _history[index]
// on the screen.
//
// Need to consider:
// If moving up... (i.e. index < _historyIdx)
// 1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
// 2. If at bottom, temporarily record _readBuf to history.
//    (Do not remove spaces, and set _tempCmdStored to "true")
// 3. If index < 0, let index = 0.
//
// If moving down... (i.e. index > _historyIdx)
// 1. If already at bottom, beep and do nothing
// 2. If index >= _history.size(), let index = _history.size() - 1.
//
// Assign _historyIdx to index at the end.
//
// [Note] index should not = _historyIdx
//i
void
CmdParser::moveToHistory(int index)
{
   // TODO...
   if(index<_historyIdx){
      if(_historyIdx==0)
	 mybeep();
      else{
	 if(_historyIdx==_history.size()-1&&_tempCmdStored==true){
	    _history.pop_back();
	    addHistory();
	 }   
         else if(_historyIdx==_history.size()){
            _tempCmdStored=true;
	    addHistory();
         }	
	 if(index<0){
	    index=0;
	    _historyIdx=0;
            for(int i=0;i<65536;i++)
               _readBuf[i]='\0';
            retrieveHistory();
         }
         else{
            _historyIdx=index;
            for(int i=0;i<65536;i++)
               _readBuf[i]='\0';
	    retrieveHistory();
         }
      }
   }
   else if(index>_historyIdx){ 
      if(_historyIdx==_history.size()-1||_historyIdx==_history.size())
	 mybeep();
      else if(index>=_history.size()){
	 index=_history.size()-1;
         _historyIdx=index;
         for(int i=0;i<65536;i++)
            _readBuf[i]='\0';
	 retrieveHistory();
      }
      else{
	 _historyIdx=index;
         for(int i=0;i<65536;i++)
            _readBuf[i]='\0';
         retrieveHistory();
      }
   }  
}


// This function adds the string in _readBuf to the _history.
// The size of _history may or may not change. Depending on whether 
// there is a temp history string.
//
// 1. Remove ' ' at the beginning and end of _readBuf
// 2. If not a null string, add string to _history.
//    Be sure you are adding to the right entry of _history.
// 3. If it is a null string, don't add anything to _history.
// 4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,
//    and reset _tempCmdStored to false
// 5. Reset _historyIdx to _history.size() // for future insertion
// 6. Reset _readBufPtr and _readBufEnd to _readBuf
// 7. Make sure *_readBufEnd = 0 ==> _readBuf becomes null string
//
void
CmdParser::addHistory()
{
   // TODO...
   char input[65536];
   for(int y=0;y<65536;y++)
      input[y]='\0';
   int i=0,j=_readBufEnd-_readBuf-1,k=0;
   if(_tempCmdStored==false){
      while(_readBuf[i]==' ')
         i++;
      while(_readBuf[j]==' '){
         j--;
         _readBufEnd--;
      }
      for(k=0;k<=j-i;k++){
         input[k]=_readBuf[i+k];
      }
      if(j-i>=0)
         _history.push_back(input);
      _historyIdx=_history.size();
   }
   else if(_tempCmdStored==true){
      _history.push_back(_readBuf);
   }
   for(int i=0;i<65536;i++)
      _readBuf[i]='\0';
}


// 1. Replace current line with _history[_historyIdx] on the screen
// 2. Set _readBufPtr and _readBufEnd to end of line
//
// [Note] Do not change _history.size().
//
void
CmdParser::retrieveHistory()
{
   deleteLine();
   strcpy(_readBuf, _history[_historyIdx].c_str());
   cout << _readBuf;
   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}
