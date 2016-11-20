/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-2014 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};

template <class T>
void swap(T*,T*);

template <class T>
class DList
{
public:
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () {
         _node=_node->_next;
         return *this;
      }
      iterator operator ++ (int) {
         DListNode<T>* tmp=_node;
         _node=_node->_next;
         return tmp;
      }
      iterator& operator -- () {
         _node=_node->_prev;
         return *this;
      }
      iterator operator -- (int) {
         DListNode<T>* tmp=_node;
         _node=_node->_prev;
         return tmp;
      }
      iterator& operator = (const iterator& i) { 
         _node=i._node;
         return *(this);
      }
      bool operator != (const iterator& i) const { 
         if(_node->_prev==(i._node)->_prev&&_node->_next==(i._node)->_next)
            return false;
         return true;
      }
      bool operator == (const iterator& i) const {
         if(_node->_prev==(i._node)->_prev&&_node->_next==(i._node)->_next)
            return true;
         return false;
      }
   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { return _head->_next; }
   iterator end() const { return _head; }
   bool empty() const { 
      if(_head->_next==_head->_prev&&_head->_next==_head)
         return true;
      else
         return false;
   }
   size_t size() const {
      size_t i=0;
      iterator it;
      for(it=begin();it!=end();++it)
         i++;
      return i; 
   }
   void push_back(const T& x) {
      DListNode<T>* pre=_head->_prev;
      DListNode<T>* NewNode=new DListNode<T>(x,pre,_head);
      pre->_next=NewNode;
      _head->_prev=NewNode;
   }
   void pop_front() {
      DListNode<T>* front=_head->_next;
      _head->_next=front->_next;
      (front->_next)->_prev=_head;
   }
   void pop_back() {
      DListNode<T>* back=_head->_prev;
      (back->_prev)->_next=_head;
      _head->_prev=back->_prev;
   }

   // return false if nothing to erase
   bool erase(iterator pos) {
      if(empty())
         return false;
      ((pos._node)->_prev)->_next=(pos._node)->_next;
      ((pos._node)->_next)->_prev=(pos._node)->_prev;
      return true;
   }
   bool erase(const T& x) {
      iterator it,tmp;
      if(empty())
         return false;
      for(it=begin();it!=end();++it){
         if((it._node)->_data==x){
            tmp=it;
            break;
         }
      }
      if(it==end())
         return false;
      else{
         ((tmp._node)->_prev)->_next=(tmp._node)->_next;
         ((tmp._node)->_next)->_prev=(tmp._node)->_prev;
         return true;
      }
   }
   void clear() {
      if(!empty()){
         DListNode<T>* _first=_head->_next;
         while(_first!=_head->_prev){
            DListNode<T>* tmp=_first->_next;
            delete _first;
            _first=tmp;
         }
         delete _first;
         _head->_next=_head->_prev=_head;
      }
   }  // delete all nodes except for the dummy node

   void sort() const {
      iterator i,j,tmp;
      for(i=begin();i!=end();++i){
         tmp=i;
         for(j=i;j!=end();++j){
            if(*j<*tmp)
               tmp=j;
         }
         swap(&(*i),&(*tmp));
      }
   }
private:
   DListNode<T>*  _head;  // = dummy node if list is empty

   // [OPTIONAL TODO] helper functions; called by public member functions
};
   template<class T>
   void swap(T* i,T* j){
      T tmp;
      tmp=*i;
      *i=*j;
      *j=tmp;
   }
#endif // DLIST_H
