/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
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

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
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

      //dereference operator
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }

      //prefix increment operator ++i --i
      iterator& operator ++ () { 
         _node = _node->_next;
         return (*this); 
      }
      iterator& operator -- () { 
         _node = _node->_prev;
         return (*this); 
      }

      //post increment operator i++ i--
      iterator operator ++ (int) { 
         iterator temp = *this;
         _node = _node->_next;
         return temp; 
      }
      iterator operator -- (int) { 
         iterator temp = *this;
         _node = _node->_prev;
         return temp; 
      }

      //assigment operator
      iterator& operator = (const iterator& i) { 
         _node = i._node;
         return (*this); 
      }

      //DListNode pointer equality
      bool operator != (const iterator& i) const { return ((_node==i._node) ? false:true) ; }
      bool operator == (const iterator& i) const { return ((_node==i._node) ? true:false) ; }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { return iterator(_head->_next); }
   iterator end() const { return iterator(_head); }
   bool empty() const { return (_head->_prev == _head); }

   size_t size() const {  
      size_t counter = 0;
      iterator li = begin();
      for (; li != end(); ++li) ++counter;
      return counter; 
   }

   void push_back(const T& x) { 
      DListNode<T>* temp = new DListNode<T>(x, _head->_prev, _head);
      temp->_prev->_next = temp;
      temp->_next->_prev = temp;
      //cout<<"pushback "<<x<<endl;
      //cout<<"tmp_next "<<temp->_next->_data<<endl;
      //cout<<"tmp_previous "<<temp->_prev->_data<<endl;
      //cout<<"tmp "<<temp->_data<<endl;
   }

   void pop_front() { 
      if (!empty()) removeDListNode(_head->_next);
   }

   void pop_back() { 
      if (!empty()) removeDListNode(_head->_prev);
   }

   // return false if nothing to erase
   bool erase(iterator pos) { 
      iterator li = begin();
      for (; li != end(); ++li){
         if (li==pos) {
            removeDListNode(pos._node);
            return true;
         }
      }
      return false; 
   }

   // return false if nothing to erase
   bool erase(const T& x) { 
      iterator li = begin();
      for (; li != end(); ++li){
         if (li._node->_data==x) {
            removeDListNode(li._node);
            return true;
         }
      }
      return false; 
   }

   iterator find(const T& x) { 
      iterator li = begin();
      for (; li != end(); ++li)
         if (li._node->_data == x) return li;
      return end(); 
   }

   // delete all nodes except for the dummy node
   void clear() { 
      iterator li = begin();
      for (; li != end(); ++li) delete li._node;
      _head->_prev = _head->_next = _head;
   }  

   typedef pair<int, iterator>  IteratorPair;
   void sort() const{ 
      QuickSort();
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
   void removeDListNode( DListNode<T>* removeTar ){
      removeTar->_next->_prev = removeTar->_prev;
      removeTar->_prev->_next = removeTar->_next;
      delete removeTar;
   }

   /* Quick sort method */
   void QuickSort() const{
      QuickSortSubVector(IteratorPair( 0, begin() ), 
                           IteratorPair( size()-1, --end() ));
   }

   // Sort subvector (Quick sort)
   void QuickSortSubVector(IteratorPair low, IteratorPair high) const{
      iterator li = begin();
      if (low.first < high.first){
         IteratorPair q = Partition(low, high);
         IteratorPair q_next = IteratorPair(q); ++(q_next.first); ++(q_next.second);
         QuickSortSubVector(q_next, high);
         QuickSortSubVector(low, q);
      }
   }

   // Partition
   IteratorPair Partition(IteratorPair low, IteratorPair high) const{ 
      T key = T(*(low.second));
      IteratorPair i = IteratorPair(--(low.first), --(low.second));
      IteratorPair j = IteratorPair(++(high.first), ++(high.second));
      while(1){
         while(1) {--(j.first); --(j.second); if(*(j.second) <= key) break;}
         while(1) {++(i.first); ++(i.second); if(*(i.second) >= key) break;}
         if( i.first < j.first ){
            iteratorSwap(i.second._node, j.second._node);
         } else {
            return j;
         }
      }
   }

   //swap two iterator i, j (i<j)
   void iteratorSwap(DListNode<T>* i, DListNode<T>* j)const{
      if (i!=j){
         T  temp = T(i->_data);
         i->_data = j->_data;
         j->_data = temp;
      }
   }

};

#endif // DLIST_H
