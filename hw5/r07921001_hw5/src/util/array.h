/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <algorithm>
#include <iostream>

using namespace std;

// NO need to implement class ArrayNode
//
template <class T>
class Array
{
public:
   // TODO: decide the initial value for _isSorted
   Array() : _data(0), _size(0), _capacity(0), _isSorted(false) {}
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

      //dereference operator
      const T& operator * () const { return (*_node); }
      T& operator * () { return (*_node); }

      //prefix increment operator ++i --i
      iterator& operator ++ () { 
         ++_node;
         return (*this); 
      }
      iterator& operator -- () { 
         --_node;
         return (*this); 
      }

      //post increment operator i++ i--
      iterator operator ++ (int) { 
         iterator temp = *this;
         ++_node;
         return temp; 
      }
      iterator operator -- (int) { 
         iterator temp = *this;
         --_node;
         return temp; 
      }

      //binary addition operator
      iterator operator + (int i) const { 
         iterator temp = *this;
         temp+=i;
         return temp;
      }

      //binary addition assignment operator
      iterator& operator += (int i) { 
         _node+=i;
         return (*this); 
      }

      //assigment operator
      iterator& operator = (const iterator& i) { 
         _node = i._node;
         return (*this); 
      }

      bool operator != (const iterator& i) const { return ((_node==i._node) ? false:true) ; }
      bool operator == (const iterator& i) const { return ((_node==i._node) ? true:false) ; }

   private:
      T*    _node;
   };

   // TODO: implement these functions
   iterator begin() const { return iterator(_data); }
   iterator end() const { return iterator(_data+_size); }
   bool empty() const { return !_size; }
   size_t size() const { return _size; }

   T& operator [] (size_t i) { return _data[i]; }
   const T& operator [] (size_t i) const { return _data[i]; }

   void push_back(const T& x) { 
      dynamicExpand();
      _data[_size++]=x;
      _isSorted=false;
   }

   void pop_front() { 
      swap(_data[0],_data[_size-1]);
      --_size;
      _isSorted = false;
   }

   void pop_back() { 
      if (!empty())
         --_size;
   }

   bool erase(iterator pos) { 
      if (pos._node<end()._node && pos._node>=begin()._node){
         swap(_data[_size-1], *pos);
         --_size;
         _isSorted = false;
         return true;
      }else return false; 
   }

   bool erase(const T& x) { 
      iterator findResult = find(x);
      if (findResult!=end()) 
         if (erase(findResult))
            return true;
      return false; 
   }

   iterator find(const T& x) { 
      if (!_isSorted){ //O(n)
         iterator counter = begin();
         for (size_t i = 0; i<_size; ++i,++counter)
            if (x == _data[i]) 
               return counter;
         return end(); 
      }
      else{ //binary search O(lgn)
         int index = binarySearch(0,_size-1,x);
         if (index==-1) return end();
         else return iterator(&_data[index]);
      }
   }

   void clear() { _isSorted=false; _size = 0; }

   // [Optional TODO] Feel free to change, but DO NOT change ::sort()
   void sort() const { 
      if (!empty() && !_isSorted){
         ::sort(_data, _data+_size); 
         _isSorted=true; 
      }
   }

   // Nice to have, but not required in this homework...
   void reserve(size_t n) { 
      assert(n>_capacity);
      _capacity = n;
   }
  
   void resize(size_t n) { 
      assert(n>-1);
      assert(_size>=n);
      _size = n;
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   T*            _data;
   size_t        _size;       // number of valid elements
   size_t        _capacity;   // max number of elements
   mutable bool  _isSorted;   // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] Helper functions; called by public member functions
   bool dynamicExpand(){
      if (_size==_capacity) {
         if(_capacity==0) _capacity = 1;
         else _capacity = _capacity*2;
         T* orig = _data;
         _data = new T[_capacity];
         for(size_t i=0; i<_size;i++) _data[i] = orig[i];
         delete[] orig;
         return true;
      }else return false;
   }

   int binarySearch(int l, int r, const T& x) { 
      if (r >= l) { 
         int mid = l + (r - l) / 2; 
         if (_data[mid] == x) return mid; 
         else if (_data[mid] > x) return binarySearch(l, mid - 1, x); 
         else return binarySearch(mid + 1, r, x); 
      } else return -1; //not found
   }
};

#endif // ARRAY_H
