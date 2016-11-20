/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>

using namespace std;

//---------------------
// Define HashSet class
//---------------------
// To use HashSet ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ()" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//
template <class Data>
class HashSet
{
public:
   HashSet() : _numBuckets(0), _buckets(0) {}
   HashSet(size_t b) : _numBuckets(0), _buckets(0) { init(b); }
   ~HashSet() { reset(); }

   // TODO: implement the HashSet<Data>::iterator
   // o An iterator should be able to go through all the valid Data
   //   in the Hash
   // o Functions to be implemented:
   //   - constructor(s), destructor
   //   - operator '*': return the HashNode
   //   - ++/--iterator, iterator++/--
   //   - operators '=', '==', !="
   //
   class iterator
   {
      friend class HashSet<Data>;

   public:
      iterator(vector<Data>* b= 0): _ptr(b),_num(0) {}
      iterator(const iterator& i) : _ptr(i._ptr),_num(0) {}
      ~iterator() {}

      const Data& operator * () const {
         if(_num<(*_ptr).size())
            return (*_ptr)[_num];
      }
      Data& operator * () {
         if(_num<(*_ptr).size())
            return (*_ptr)[_num];
      }
      iterator& operator ++ () {
         if((*_ptr).size()==0||(*_ptr).size()-1<_num){
            while((*_ptr).size()==0||(*_ptr).size()-1<_num){
               _ptr++;
               _num=0;
            }
         }
         else if((*_ptr).size()-1==_num){
            _ptr++;
            _num=0;
            while((*_ptr).size()==0||(*_ptr).size()-1<_num){
               _ptr++;
               _num=0;
            }
         }
         else
            _num++;
         if((*_ptr)[_num].getName()==""){
            _ptr++;
            _num=0;
         }
         return *this;
      }
      iterator operator ++ (int) {
         iterator tmp=*this;
         if((*_ptr).size()==0||(*_ptr).size()-1<_num){
            while((*_ptr).size()==0||(*_ptr).size()-1<_num){
               _ptr++;
               _num=0;
            }
         }
         else if((*_ptr).size()-1==_num){
            _ptr++;
            _num=0;
            while((*_ptr).size()==0||(*_ptr).size()-1<_num){
               _ptr++;
               _num=0;
            }
         }
         else
            _num++;
         if((*_ptr)[_num].getName()==""){
            _ptr++;
            _num=0;
         }
         return tmp;
      }
      iterator& operator -- () {
         if((*_ptr).size()==0||_num==0){
            while((*_ptr).size()==0||_num==0){
               _ptr--;
               _num=(*_ptr).size();
            }
         }
         else
            _num--;
         if((*_ptr)[_num].getName()==""){
            if((*_ptr).size()==1){
               _ptr--;
               _num=(*_ptr).size();
            }
            else
               _num--;
         }
         return *this;
      }
      iterator operator -- (int) {
         iterator tmp=*this;
         if((*_ptr).size()==0||_num==0){
            while((*_ptr).size()==0||_num==0){
               _ptr--;
               _num=(*_ptr).size();
            }
         }
         else
            _num--;
         if((*_ptr)[_num].getName()==""){
            if((*_ptr).size()==1){
               _ptr--;
               _num=(*_ptr).size();
            }
            else
               _num--;
         }
         return tmp;
      }
      iterator& operator = (const iterator& i) { 
         _ptr=i._ptr;
         _num=i._num;
         return *this;
      }
      bool operator != (const iterator& i) const { 
         if(_ptr==i._ptr&&_num==i._num)
            return false;
         return true;
      }
      bool operator == (const iterator& i) const {
         if(_ptr==i._ptr&&_num==i._num)
            return true;
         return false;
      }
   private:
      size_t            _num;
      vector<Data>*     _ptr;
   };

   void init(size_t b) {
      reset(); _numBuckets = b; _buckets = new vector<Data>[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<Data>& operator [] (size_t i) { return _buckets[i]; }
   const vector<Data>& operator [](size_t i) const { return _buckets[i]; }

   // TODO: implement these functions
   //
   // Point to the first valid data
   iterator begin() const {
      Data t("",0);
      _buckets[_numBuckets-1].push_back(t);
      //cout<<_buckets[_numBuckets-1][0]<<endl;
      for(int i=0;i<_numBuckets;i++){
         if(_buckets[i].size()!=0){
            return _buckets+i;
         }
      }
   }
   // Pass the end
   iterator end() const { return _buckets+_numBuckets; }
   // return true if no valid data
   bool empty() const {
      size_t s=0;
      for(int i=0;i<_numBuckets;i++)
         s+=_buckets[i].size();
      if(s==0)
         return true;
      else
         return false;
   }
   // number of valid data
   size_t size() const {
      size_t s = 0;
      for(int i=0;i<_numBuckets;i++)
         s+=_buckets[i].size();
      return s;
   }
   // check if d is in the hash...
   // if yes, return true;
   // else return false;
   bool check(const Data& d) const { 
      size_t pos=d()%_numBuckets;
      for(int i=0;i<_buckets[pos].size();i++){
         if(_buckets[pos][i]==d)
            return true;
      }
      return false;
   }
   // query if d is in the hash...
   // if yes, replace d with the data in the hash and return true;
   // else return false;
   bool query(Data& d) const {
      size_t pos=d()%_numBuckets;
      for(int i=0;i<_buckets[pos].size();i++){
         if(_buckets[pos][i]==d){
            d=_buckets[pos][i];
            return true;
         }
      }
      return false;
   }
   // update the entry in hash that is equal to d
   // if found, update that entry with d and return true;
   // else insert d into hash as a new entry and return false;
   bool update(const Data& d) {
      size_t pos=d()%_numBuckets;
      for(int i=0;i<_buckets[pos].size();i++){
         if(_buckets[pos][i]==d){
            _buckets[pos][i]=d;
            return true;
         }
      }
      _buckets[bucketNum(d)].push_back(d);
      return false;
   }
   // return true if inserted successfully (i.e. d is not in the hash)
   // return false is d is already in the hash ==> will not insert
   bool insert(const Data& d) { 
      size_t pos=d()%_numBuckets;
      for(int i=0;i<_buckets[pos].size();i++){
         if(_buckets[pos][i]==d)
            return false;
      }
      _buckets[bucketNum(d)].push_back(d);
      return true;
   }

private:
   // Do not add any extra data member
   size_t            _numBuckets;
   vector<Data>*     _buckets;

   size_t bucketNum(const Data& d) const {
      return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H
