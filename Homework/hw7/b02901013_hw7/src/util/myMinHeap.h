/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap() : _data(0) {}
   MinHeap(size_t s) {
      _data.reserve(s);
      for (size_t i = 0; i < s; ++i) _data.push_back(Data());
      sort(_data.begin(), _data.end());
   }
   ~MinHeap() {}

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const { return _data[0]; }
   void insert(const Data& d) {
      size_t s=size()+1;
      _data.push_back(d);
      while(s>1){
         if(_data[s/2-1]<d)
            break;
         _data[s-1]=_data[s/2-1];
         s=s/2;
      }
      _data[s-1]=d;
   }
   void delMin() { delData(0); }
   void delData(size_t i) {
      size_t s=size();
      size_t t=(i+1)*2;
      while(t<=s){
         if(t+1<=s){
            if(_data[t]<_data[t-1])
               ++t;
         }
         if(_data[s-1]<_data[t-1])
            break;
         _data[i]=_data[t-1];
         i=t-1;
         t=2*(i+1);
      }
      _data[t/2-1]=_data[s-1];
      _data.pop_back();
      
   } // remove _data[i]

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
