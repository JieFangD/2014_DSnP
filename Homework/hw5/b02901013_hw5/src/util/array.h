/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <algorithm>

using namespace std;

// NO need to implement class ArrayNode
//
template <class T>
class Array
{
public:
   Array() : _data(0), _size(0), _capacity(0) {}
   ~Array() { delete []_data; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class Array;

   public:
      iterator(T* n= 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return *_node; }
      T& operator * () { return *_node; }
      iterator& operator ++ () {
         _node++;         
         return (*this);
      }
      iterator operator ++ (int) {
         T* tmp=_node;
         _node++;
         return tmp;
      }
      iterator& operator -- () {
         _node--;
         return (*this);
      }
      iterator operator -- (int) {
         T* tmp=_node;
         _node--;
         return tmp;
      }
      iterator operator + (int i) const { return _node+i; }
      iterator& operator += (int i) { return _node+=i; }
      iterator& operator = (const iterator& i) {
         _node=i._node;
         return (*this);
      }
      bool operator != (const iterator& i) const { 
         if(_node==i._node)
            return false;
         return true;
      }
      bool operator == (const iterator& i) const { 
         if(_node==i._node)
            return true;
         return false;
      }
   private:
      T*    _node;
   };

   // TODO: implement these functions
   iterator begin() const { 
      if(_size==0)
         return 0;
      return _data;
   }
   iterator end() const { 
      if(_size==0)
         return 0;
      return _data+_size;
   }
   bool empty() const { 
      if(_size==0)
         return true;
      return false;
   }
   size_t size() const { return _size; }
   T& operator [] (size_t i) { return _data[i]; }
   const T& operator [] (size_t i) const { return _data[i]; }
   void push_back(const T& x) {
      _size++;
      if(_size>=_capacity){
         _capacity=2*(_size);
         T* ptr=new T [_capacity];
         for(size_t i=0;i<_size-1;i++)
            ptr[i]=*(_data+i);
         _data=ptr;
      }
      _data[_size-1]=x;
   }
   void pop_front() {
      if(_size>0){
         _size--;
         for(size_t i=0;i<_size;i++)
            _data[i]=_data[i+1];
      }
   }
   void pop_back() {
      if(_size>0)
         _size--;
   }
   bool erase(iterator pos) { 
      if(empty())
         return false;
      while(pos!=end()){
         *pos=*(pos+1);
         pos++;
      }
      _size--;
      return true;
   }
   bool erase(const T& x) {
      if(empty())
         return false;
      T* it=_data;
      size_t i;
      for(i=0;i<_size;i++){
         if(*(it+i)==x)
            break;
      }
      if(i==_size)
         return false;
      else{
         erase(it+i);
         return true;
      }
   }

   void clear() { _size=0; }

   // This is done. DO NOT change this one.
   void sort() const { if (!empty()) ::sort(_data, _data+_size); }

   // Nice to have, but not required in this homework...
   // void reserve(size_t n) { ... }
   // void resize(size_t n) { ... }

private:
   T*           _data;
   size_t       _size;       // number of valid elements
   size_t       _capacity;   // max number of elements

   // [OPTIONAL TODO] Helper functions; called by public member functions
};

#endif // ARRAY_H
