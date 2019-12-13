/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
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
// "operator ==" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//
template <class Data>
class HashSet
{
public:
   HashSet(size_t b = 0) : _numBuckets(0), _buckets(0) { if (b != 0) init(b); }
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
      iterator(bool begin, vector<Data>* b, size_t n):_buckets(b),_numBuckets(n),_hash_idx(0),_pos(0){
         if (begin){
            for (;_hash_idx<_numBuckets;++_hash_idx)
               if (_buckets[_hash_idx].size()) break;
         }
         else _hash_idx = _numBuckets;
      }
      ~iterator() {}
      // dereference
      const Data& operator * () const { return _buckets[_hash_idx][_pos]; }
      Data& operator * () { return _buckets[_hash_idx][_pos]; }
      // ++i --i
      iterator& operator ++ () { 
         if(_hash_idx == _numBuckets) return (*this); //end in end out
         ++_pos;
         for (;_hash_idx<_numBuckets;++_hash_idx){ //search in buckets
            if (!_buckets[_hash_idx].empty())
               for (; _pos<_buckets[_hash_idx].size(); ++_pos)//search in slots
                  return (*this); 
            _pos = 0; // next bucket pos start from 0
         }
         return (*this); 
      }
      iterator& operator -- () { 
         size_t hash_idx_now = _hash_idx;
         size_t pos_now = _pos;
         --pos_now;
         for (;hash_idx_now>=0;--hash_idx_now){ //search in buckets
            if (!_buckets[hash_idx_now].empty())
               for (; pos_now>=0; --pos_now){//search in slots
                  _pos = pos_now;
                  _hash_idx = hash_idx_now;
                  return (*this); 
               }
            if(hash_idx_now>0) pos_now = _buckets[hash_idx_now-1].size()-1; // next bucket pos start from slot_size -1
         }
         return (*this); //begin in begin out
      }
      // i++ i--
      iterator operator ++ (int) {
         iterator temp = *this;
         ++(*this);
         return temp;
      } 
      iterator operator -- (int) {
         iterator temp = *this;
         --(*this);
         return temp;
      } 
      //asign
      iterator& operator = (const iterator& i) { 
         _hash_idx = i._hash_idx;
         _pos = i._pos;
         return (*this); 
      }
      //equal
      bool operator == (const iterator& i) const { 
         return ((_hash_idx==i._hash_idx)&&(_pos==i._pos))? true:false; 
      }
      bool operator != (const iterator& i) const {
         return !(*this == i); 
      }

   private:
      size_t            _numBuckets;
      size_t            _hash_idx;     //buckets id
      size_t            _pos;          //slots id
      vector<Data>*     _buckets;
   };

   void init(size_t b) { _numBuckets = b; _buckets = new vector<Data>[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   void clear() {
      for (size_t i = 0; i < _numBuckets; ++i) _buckets[i].clear();
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<Data>& operator [] (size_t i) { return _buckets[i]; }
   const vector<Data>& operator [](size_t i) const { return _buckets[i]; }

   // TODO: implement these functions
   //
   // Point to the first valid data
   iterator begin() const { return iterator(true, _buckets, _numBuckets); }
   // Pass the end
   iterator end() const { return iterator(false, _buckets, _numBuckets); }
   // return true if no valid data
   bool empty() const { return begin()==end(); }
   // number of valid data
   size_t size() const { 
      size_t counter = 0;
      iterator iter = begin();
      for (;iter!=end();++iter) ++counter;
      return counter;
   }

   // check if d is in the hash...
   // if yes, return true;
   // else return false;
   bool check(const Data& d) const { 
      size_t hash_idx = bucketNum(d);
      for (size_t pos = 0; pos<_buckets[hash_idx].size(); ++pos)
         if (_buckets[hash_idx][pos] == d) return true;
      return false; 
   }

   // query if d is in the hash...
   // if yes, replace d with the data in the hash and return true;
   // else return false;
   bool query(Data& d) const { 
      size_t hash_idx = bucketNum(d);
      for (size_t pos = 0; pos<_buckets[hash_idx].size(); ++pos)
         if (_buckets[hash_idx][pos] == d) {
            d = _buckets[hash_idx][pos];
            return true;
         }
      return false; 
   }

   // update the entry in hash that is equal to d (i.e. == return true)
   // if found, update that entry with d and return true;
   // else insert d into hash as a new entry and return false;
   bool update(const Data& d) { 
      size_t hash_idx = bucketNum(d);
      for (size_t pos = 0; pos<_buckets[hash_idx].size(); ++pos)
         if (_buckets[hash_idx][pos] == d) {
            _buckets[hash_idx].erase(_buckets[hash_idx].begin()+pos);
            _buckets[hash_idx].push_back(d);
            return true;
         }
      _buckets[hash_idx].push_back(d);
      return false; 
   }

   // return true if inserted successfully (i.e. d is not in the hash)
   // return false is d is already in the hash ==> will not insert
   bool insert(const Data& d) { 
      size_t hash_idx = bucketNum(d);
      for (size_t pos = 0; pos<_buckets[hash_idx].size(); ++pos)
         if (_buckets[hash_idx][pos] == d) return false;
      _buckets[hash_idx].push_back(d);
      return true; 
   }

   // return true if removed successfully (i.e. d is in the hash)
   // return fasle otherwise (i.e. nothing is removed)
   bool remove(const Data& d) {
      size_t hash_idx = bucketNum(d);
      for (size_t pos = 0; pos<_buckets[hash_idx].size(); ++pos)
         if (_buckets[hash_idx][pos] == d) {
            _buckets[hash_idx].erase(_buckets[hash_idx].begin()+pos);
            return true;
         }
      return false; 
   }

private:
   // Do not add any extra data member
   size_t            _numBuckets;
   vector<Data>*     _buckets;

   size_t bucketNum(const Data& d) const {
      return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H
