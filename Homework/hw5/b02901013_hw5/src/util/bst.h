/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
   friend class BSTree<T>;
   friend class BSTree<T>::iterator;
 
   BSTreeNode(const T& d,BSTreeNode<T>* l=0,BSTreeNode<T>* r=0,BSTreeNode<T>* p=0):
      _data(d),_left(l),_right(r),_parent(p) {}

   T                 _data;
   BSTreeNode<T>*    _left;
   BSTreeNode<T>*    _right;
   BSTreeNode<T>*    _parent;
};


template <class T>
class BSTree
{
   // TODO: design your own class!!

public:
   BSTree() {
      _root=new BSTreeNode<T> (T());
      _max =new BSTreeNode<T> (T());
      _root->_left=_root->_right=_max->_right=_max->_parent=NULL;
      _max->_left=_root;
      _root->_parent=_max;
      _size=0;
   }
   ~BSTree() { clear(); delete _root; delete _max; }
   
   class iterator 
   {
      friend class BSTree;
   
   public:
      iterator(BSTreeNode<T>* n=0): _node(n) { }
      iterator(const iterator& i) : _node(i._node) { }
      ~iterator() {}

   const T& operator * () const {return _node->_data; }
      T& operator *() { return _node->_data; }
      iterator& operator ++ () {
         BSTreeNode<T>* tmp=_node;
         if(_node->_right==NULL){   
            if(_node->_right==NULL&&(_node->_parent)->_parent!=NULL){
               _node=_node->_parent;
               while(_node->_right==tmp&&(_node->_parent)->_parent!=NULL){
                  tmp=_node;
                  _node=_node->_parent;
               }
            }
            if((_node->_parent)->_parent==NULL){
               if(_node->_right==tmp){
                  _node=_node->_parent;
                  return *this;
               }
               if(_node->_left==NULL&&_node->_right==NULL){
                  _node=_node->_parent;
                  return *this;
               }
               if(tmp==_node){
                  _node=_node->_parent;
                  return *this;
               }
            }
            return *this;
         }
         else{
            _node=_node->_right;
            while(_node->_left)
               _node=_node->_left;
            return *this;
         }
      }
      iterator operator ++ (int) {
         BSTreeNode<T>* tmp=_node;
         BSTreeNode<T>* rtmp=_node;
         if(_node->_right==NULL){   
            if(_node->_right==NULL&&(_node->_parent)->_parent!=NULL){
               _node=_node->_parent;
               while(_node->_right==tmp&&(_node->_parent)->_parent!=NULL){
                  tmp=_node;
                  _node=_node->_parent;
               }
            }
            if((_node->_parent)->_parent==NULL){
               if(_node->_right==tmp){
                  _node=_node->_parent;
                  return rtmp;
               }
               if(_node->_left==NULL&&_node->_right==NULL){
                  _node=_node->_parent;
                  return rtmp;
               }
               if(tmp==_node){
                  _node=_node->_parent;
                  return rtmp;
               }
            }
            return rtmp;
         }
         else{
            _node=_node->_right;
            while(_node->_left)
               _node=_node->_left;
            return rtmp;
         }
      }
      iterator& operator -- () {
         BSTreeNode<T>* tmp=_node;
         if(_node->_parent==NULL){
            _node=_node->_left;
            while(_node->_right)
               _node=_node->_right;
            return *this;
         }
         if(_node->_left==NULL){   
            if(_node->_left==NULL&&(_node->_parent)->_parent!=NULL){
               _node=_node->_parent;
               while(_node->_left==tmp&&(_node->_parent)->_parent!=NULL){
                  tmp=_node;
                  _node=_node->_parent;
               }
            }
            if((_node->_parent)->_parent==NULL){
               if(_node->_left==tmp){
                  while(_node->_left)
                     _node=_node->_left;
                  return *this;
               }
               if(_node->_left==NULL&&_node->_right==NULL){
                  while(_node->_left)
                     _node=_node->_left;
                  return *this;
               }
               if(tmp==_node){
                  while(_node->_left)
                     _node=_node->_left;
                  return *this;
               }
            }
            return *this;
         }
         else{
            _node=_node->_left;
            while(_node->_right)
               _node=_node->_right;
            return *this;
         }
      }
      iterator operator -- (int) {
         BSTreeNode<T>* tmp=_node;
         BSTreeNode<T>* rtmp=_node;
         if(_node->_parent==NULL){
            _node=_node->_left;
            while(_node->_right)
               _node=_node->_right;
            return rtmp;
         }
         if(_node->_left==NULL){   
            if(_node->_left==NULL&&(_node->_parent)->_parent!=NULL){
               _node=_node->_parent;
               while(_node->_left==tmp&&(_node->_parent)->_parent!=NULL){
                  tmp=_node;
                  _node=_node->_parent;
               }
            }
            if((_node->_parent)->_parent==NULL){
               if(_node->_left==tmp){
                  while(_node->_left)
                     _node=_node->_left;
                  return rtmp;
               }
               if(_node->_left==NULL&&_node->_right==NULL){
                  while(_node->_left)
                     _node=_node->_left;
                  return rtmp;
               }
               if(tmp==_node){
                  while(_node->_left)
                     _node=_node->_left;
                  return rtmp;
               }
            }
            return rtmp;
         }
         else{
            _node=_node->_left;
            while(_node->_right)
               _node=_node->_right;
            return rtmp;
         }
      }
      iterator& operator = (const iterator& i) { 
         _node=i._node;
         return *(this);
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
      BSTreeNode<T>* _node;
   };
   iterator begin() const {
      BSTreeNode<T> * tmp=_root;
      if(_size==0){
         tmp=NULL;
         return tmp;
      }
      else if(_size==1)
         return tmp;
      else{
         while(tmp->_left)
            tmp=tmp->_left;
         return tmp;  
      }
   }
   iterator end() const {
      BSTreeNode<T>* tmp=_root;
      if(_size==0){
         tmp=NULL;
         return tmp;
      }
      return _max;
   }
   bool empty() const { 
      if(_size==0)
         return true;
      else
         return false;
   }
   size_t size() const {
      return _size; 
   }
   void pop_front() {
      if(_size==1){
         _root->_left=_root->_right=NULL;
         _size--;
      }
      else if(_size>1){
         BSTreeNode<T>* tmp=_root;
         while(tmp->_left)
            tmp=tmp->_left;
         if(tmp->_parent!=_max){
            if(tmp->_right)
               (tmp->_right)->_parent=tmp->_parent;
            (tmp->_parent)->_left=tmp->_right;
         }
         else{
            _root=_root->_right;
            _root->_parent=_max;
            _max->_left=_root;
         }
         _size--;
      }
   }
   void pop_back() {
      if(_size==1){
         _root->_left=_root->_right=NULL;
         _size--;
      }
      else if(_size>1){
         BSTreeNode<T>* tmp=_root;
         while(tmp->_right)
            tmp=tmp->_right;
         if(tmp->_parent!=_max){
            if(tmp->_left)
               (tmp->_left)->_parent=tmp->_parent;
            (tmp->_parent)->_right=tmp->_left;
         }
         else{
            _root=_root->_left;
            _root->_parent=_max;
            _max->_left=_root;
         }
         _size--;
      }
   }
   void insert(const T& x) {
      if(_size==0){
         _root->_data=x;
         _max->_data=x;
      }
      else{
         if(_max->_data<x)
            _max->_data=x;
         BSTreeNode<T>* newNode=new BSTreeNode<T> (T());
         newNode->_data=x;
         newNode->_left=newNode->_right=NULL;
         BSTreeNode<T>* tmp=_root;
         while(tmp){
            if(x<tmp->_data){
               if(tmp->_left)
                  tmp=tmp->_left;
               else{
                  tmp->_left=newNode;
                  newNode->_parent=tmp;
                  break;
               }
            }
            else{
               if(tmp->_right)
                  tmp=tmp->_right;
               else{
                  tmp->_right=newNode;
                  newNode->_parent=tmp;
                  break;
               }
            }
         }
      }
      _size++;
   }
   bool erase(const T& x) {
      if(empty())
         return false;
      else{
         iterator it;
         for(it=begin();it!=end();it++){
            if((it._node)->_data==x)
               break;
         }
         if(it==end())
            return false;
         else
            erase(it);
         return true;
      }
   }
   bool erase(iterator pos) {
      if(empty())
         return false;
      if(pos==end())
         return false;
      else{
         if((pos._node)->_left==NULL&&(pos._node)->_right==NULL){
            if((pos._node)->_parent!=_max){
               if(((pos._node)->_parent)->_left==pos._node)
                  ((pos._node)->_parent)->_left=NULL;
               else
                  ((pos._node)->_parent)->_right=NULL;
            }
         }
         else if((pos._node)->_left!=NULL&&(pos._node)->_right==NULL){
            ((pos._node)->_left)->_parent=(pos._node)->_parent;
            if((pos._node)->_parent==_max){
               _root=(pos._node)->_left;
               _root->_parent=_max;
               _max->_left=_root;
            }
            else if(((pos._node)->_parent)->_left==pos._node)
               ((pos._node)->_parent)->_left=(pos._node)->_left;
            else
               ((pos._node)->_parent)->_right=(pos._node)->_left;
         }
         else if((pos._node)->_left==NULL&&(pos._node)->_right!=NULL){
            ((pos._node)->_right)->_parent=(pos._node)->_parent;
            if((pos._node)->_parent==_max){
               _root=(pos._node)->_right;
               _root->_parent=_max;
               _max->_left=_root;
            }
            else if(((pos._node)->_parent)->_left==pos._node)
               ((pos._node)->_parent)->_left=(pos._node)->_right;
            else
               ((pos._node)->_parent)->_right=(pos._node)->_right;
         }
         else if((pos._node)->_left!=NULL&&(pos._node)->_right!=NULL){
            BSTreeNode<T>* tmp=(pos._node)->_right;
            while(tmp->_left)
               tmp=tmp->_left;
            if(tmp==(pos._node)->_right){
               tmp->_left=(pos._node)->_left;
               ((pos._node)->_left)->_parent=tmp;
               tmp->_parent=(pos._node)->_parent;
               if((pos._node)->_parent!=_max){
                  if(((pos._node)->_parent)->_left==pos._node)
                     ((pos._node)->_parent)->_left=tmp;
                  else
                     ((pos._node)->_parent)->_right=tmp;
               }
            }
            else{
               ((pos._node)->_left)->_parent=tmp;
               if(tmp->_right){
                  (tmp->_parent)->_left=tmp->_right;
                  (tmp->_right)->_parent=tmp->_parent;
               }
               else
                  (tmp->_parent)->_left=NULL;
               ((pos._node)->_right)->_parent=tmp;
               tmp->_right=(pos._node)->_right;
               tmp->_left=(pos._node)->_left;
               tmp->_parent=(pos._node)->_parent;
               if((pos._node)->_parent!=_max){
                  if(((pos._node)->_parent)->_left==pos._node)
                     ((pos._node)->_parent)->_left=tmp;
                  else
                     ((pos._node)->_parent)->_right=tmp;
               }
            }
            if(((pos._node)->_parent)==_max){
               _root=tmp;
               _root->_parent=_max;
               _max->_left=_root;
            }
         }
         _size--;
         return true;
      }
   }
   void sort() {}
   void print() {}
   void clear() {
      if(_size>1){
         if(_root->_left!=NULL){
            deleteNode(_root->_left);
            _root->_left=NULL;
         }
         if(_root->_right!=NULL){
            deleteNode(_root->_right);
            _root->_right=NULL;
         }
      }
      _size=0;
   }
   void deleteNode(BSTreeNode<T>* n){
      if(n->_left==NULL&&n->_right==NULL)
         delete n;
      else if(n->_left!=NULL&&n->_right==NULL)
         deleteNode(n->_left);
      else if(n->_left==NULL&&n->_right!=NULL)
         deleteNode(n->_right);
      else{
         deleteNode(n->_left);
         deleteNode(n->_right);
      }
   }
private:
   size_t            _size;
   BSTreeNode<T>*    _root;
   BSTreeNode<T>*    _max;
};

#endif // BST_H
