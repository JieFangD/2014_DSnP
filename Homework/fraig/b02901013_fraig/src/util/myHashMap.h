/****************************************************************************
  FileName     [ myHashMap.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashMap and Cache ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2009-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_MAP_H
#define MY_HASH_MAP_H

#include <vector>

using namespace std;

// TODO: Implement your own HashMap and Cache classes.

//-----------------------
// Define HashMap classes
//-----------------------
// To use HashMap ADT, you should define your own HashKey class.
// It should at least overload the "()" and "==" operators.
//
class HashKey
{
public:
   HashKey(size_t n):_num(n) {};
 
   size_t operator() () const { return _num; }

   void operator = (const size_t num) { _num=num; }
   bool operator == (const HashKey& k) const { 
      if(_num==k._num)
         return true;
      return false;
   }

private:
   size_t _num;
};

template <class HashKey, class HashData>
class HashMap
{
typedef pair<HashKey, HashData> HashNode;

public:
   HashMap() : _numBuckets(0), _buckets(0) {}
   HashMap(size_t b) : _numBuckets(0), _buckets(0) { init(b); }
   ~HashMap() { reset(); }

   // [Optional] TODO: implement the HashMap<HashKey, HashData>::iterator
   // o An iterator should be able to go through all the valid HashNodes
   //   in the HashMap
   // o Functions to be implemented:
   //   - constructor(s), destructor
   //   - operator '*': return the HashNode
   //   - ++/--iterator, iterator++/--
   //   - operators '=', '==', !="
   //
   // (_bId, _bnId) range from (0, 0) to (_numBuckets, 0)
   //

   void init(size_t b) {
      reset(); _numBuckets = b; _buckets = new vector<HashNode>[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<HashNode>& operator [] (size_t i) { return _buckets[i]; }
   const vector<HashNode>& operator [](size_t i) const { return _buckets[i]; }

   // TODO: implement these functions
   //
   // Point to the first valid data
   /*iterator begin() const {
      HashNode h(0,0);
      _buckets[_numBuckets-1].push_back(h);
      for(int i=0;i<_numBuckets;i++){
         if(_buckets[i].size()!=0)
            return _buckets+i;
      }
   }
   // Pass the end
   iterator end() const { return _buckets+numBuckets(); }
   // return true if no valid data
   bool empty() const {
      if(size()==0) 
         return true;
      return false;
   }
   // number of valid data
   size_t size() const {
      size_t s=0;
      iterator it=begin();
      for(;it!=end();it++)
      if((*it).second!=0)
         s++;
      return s;
   }*/
   bool check(HashNode& d) const {
      size_t pos=bucketNum(d.first);
      for(int i=0;i<_buckets[pos].size();i++){
         if(_buckets[pos][i].first==d.first){
            if(_buckets[pos][i].second->faninL==d.second->faninL&&_buckets[pos][i].second->invertL==d.second->invertL&&
               _buckets[pos][i].second->faninR==d.second->faninR&&_buckets[pos][i].second->invertR==d.second->invertR)
               return true;
            else if(_buckets[pos][i].second->faninL==d.second->faninR&&_buckets[pos][i].second->invertL==d.second->invertR&&
                    _buckets[pos][i].second->faninR==d.second->faninL&&_buckets[pos][i].second->invertR==d.second->invertL)
               return true;
         }
      }
      return false;
   }
   bool query(HashNode& d) const {
      size_t pos=bucketNum(d.first);
      for(int i=0;i<_buckets[pos].size();i++){
         if(_buckets[pos][i].first==d.first){
            if(_buckets[pos][i].second->faninL==d.second->faninL&&_buckets[pos][i].second->invertL==d.second->invertL&&
               _buckets[pos][i].second->faninR==d.second->faninR&&_buckets[pos][i].second->invertR==d.second->invertR){
               d=_buckets[pos][i];
               return true;
            }
           else if(_buckets[pos][i].second->faninL==d.second->faninR&&_buckets[pos][i].second->invertL==d.second->invertR&&
                   _buckets[pos][i].second->faninR==d.second->faninL&&_buckets[pos][i].second->invertR==d.second->invertL){
               d=_buckets[pos][i];
               return true;
            }
         }
      }
      return false;
   }
   bool insert(const HashNode& d) { 
      size_t pos=bucketNum(d.first);
      size_t i=_buckets[pos].size();
      while(i>0&&_buckets[pos][i-1].second->GateID>d.second->GateID)
         i--;
      _buckets[pos].insert(_buckets[pos].begin()+i,d);
      return true;
   }
   void getFEC(vector< vector<HashNode> >& Fgroup ){
      for(int i=0;i<_numBuckets;i++){
         if(_buckets[i].size()>1)
            //for(int j=0;j<_buckets[i].size();j++){
            Fgroup.push_back(_buckets[i]);
               /*for(int k=0;k<j;k++){
                  if(_buckets[i][k].first==_buckets[i][j].first){
                     v[j].push_back[_buckets[i]
            }*/
         _buckets[i].clear();
      }
      //return Fgroup;
   }
   /*HashMap resize() {
      for(int i=0;i<_numBuckets;i++){
         if(_buckets[i].size()<2)
            _buckets[i].clear();*/



   void print(){
      for(int i=0;i<_numBuckets;i++){
         for(int j=0;j<_buckets[i].size();j++)
            cout<<_buckets[i][j].first()<<"   ";
         cout<<_buckets[i].size()<<"!!!"<<endl;
      }
   }

private:
   // Do not add any extra data member
   size_t                   _numBuckets;
   vector<HashNode>*        _buckets;

   size_t bucketNum(const HashKey& k) const {
      return (k() % _numBuckets); }

};



   /*class iterator
   {
      friend class HashMap<HashKey, HashData>;

   public:
      iterator(vector<HashNode>* n= 0): _ptr(n),_num(0) {}
      iterator(const iterator& i) : _ptr(i._ptr),_num(0) {}
      ~iterator() {}

      const HashNode& operator * () const {
         if(_num<(*_ptr).size())
            return (*_ptr)[_num];
      }
      HashNode& operator * () {
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
         if((*_ptr)[_num].second==0){
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
         if((*_ptr)[_num].second==0){
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
         if((*_ptr)[_num].second==0){
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
         if((*_ptr)[_num].second==0){
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
      vector<HashNode>* _ptr;
      size_t            _num;
   };*/

//---------------------
// Define Cache classes
//---------------------
// To use Cache ADT, you should define your own HashKey class.
// It should at least overload the "()" and "==" operators.
//
/*class CacheKey
{
public:
   CacheKey(size_t n):_num(n) {};
   
   size_t operator() () const { return _num; }
  
   bool operator == (const CacheKey& k) const { 
      if(_num==k._num)
         return true;
      return false;
   }

private:
   size_t _num;
}; 
 
template <class CacheKey, class CacheData>
class Cache
{
typedef pair<CacheKey, CacheData> CacheNode;

public:
   Cache() : _size(0), _cache(0) {}
   Cache(size_t s) : _size(0), _cache(0) { init(s); }
   ~Cache() { reset(); }

   // NO NEED to implement Cache::iterator class

   // TODO: implement these functions
   //
   // Initialize _cache with size s
   void init(size_t s) {
      reset(); _size = s;} //_cache = new CacheNode[s]; }
   void reset() {
      _size = 0;
      if (_cache) { delete [] _cache; _cache = 0; }
   }
   size_t size() const { return _size; }

   CacheNode& operator [] (size_t i) { return _cache[i]; }
   const CacheNode& operator [](size_t i) const { return _cache[i]; }

   // return false if cache miss
   bool read(const CacheKey& k, CacheData& d) const { 
      if(_cache[k()%_size].first==k)
         return true;
      return false;
   }
   // If k is already in the Cache, overwrite the CacheData
   void write(const CacheKey& k, const CacheData& d) {
      //CacheNode _c(k,d);
      //_cache[k()%_size]=_c;	
   }

private:
   // Do not add any extra data member
   size_t         _size;
   CacheNode*     _cache;
};*/


#endif // MY_HASH_H
