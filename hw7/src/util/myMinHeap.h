/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}

   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }
   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const { return _data[0]; }

   void insert(const Data& d) { 
      _data.push_back(d); 
      size_t now_idx = size()-1;
      while (now_idx>0){
         size_t parant_idx = now_idx/2;
         if (d.getLoad()>=_data[parant_idx].getLoad()) break;
         _data[now_idx] = _data[parant_idx];
         now_idx = parant_idx;
      }
      _data[now_idx] = d;
   }

   void delMin() { delData(0); }

   void delData(size_t i) { 
      swap(_data[i],_data[size()-1]);
      _data.pop_back();
      MinHeapify(i);
   }

   void MinHeapify(size_t root){
      int left_idx = root*2+1;
      int right_idx = root*2+2;
      int smallest_idx = root;
      if ((left_idx<size())&&(_data[left_idx] < _data[smallest_idx])) smallest_idx = left_idx;
      if ((right_idx<size())&&(_data[right_idx] < _data[smallest_idx])) smallest_idx = right_idx;
      if (smallest_idx != root) {
         swap(_data[root], _data[smallest_idx]);
         MinHeapify(smallest_idx);
      }
   } 

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
