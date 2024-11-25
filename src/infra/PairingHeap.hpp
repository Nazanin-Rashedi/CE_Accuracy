#ifndef H_PairingHeap
#define H_PairingHeap
//----------------------------------------------------------------------------
#include "Pool.hpp"
#include <functional>
//----------------------------------------------------------------------------
/// A maximum priority queue implemented via a pairing heap
template <typename T,typename Compare = std::less<T> > class PairingHeap
{
   private:
   /// A heap entry
   struct HeapEntry {
      /// Pointers to other entries
      HeapEntry* previous,*next,*child;
      /// The data element
      T data;
   };
   /// The heap
   StructPool<HeapEntry> heap;
   /// A pointer to the root
   HeapEntry* root;
   /// The comparison functions
   Compare cmp;
   /// The number of contained elements
   unsigned count;

   /// Flatten the heap. This destroys the heap property, use only for cleanup!
   static void flattenForCleanup(HeapEntry* h);

   /// Copy heap entries recursively
   void copyFromHeap(HeapEntry* node);
   /// Merge two heaps
   HeapEntry* merge(HeapEntry* a,HeapEntry* b);
   /// Remove a heap node. Returns its children as new heap. This does not free the entry or unlink it!
   HeapEntry* removePrimitive(HeapEntry* root);
   /// Removes a heap node from the heap but does not free it
   void removeEntry(HeapEntry* root);
   /// Unlink a subheap. entry must not be the root
   void unlink(HeapEntry* entry);

   public:
   /// Type of the contained objects
   typedef T value_type;
   /// Type of the underlying sizes
   typedef unsigned size_type;
   /// An pointer to a specific heap entry
   class point_iterator {
      private:
      /// The enty
      HeapEntry* entry;
      friend class PairingHeap;

      public:
      /// Constructor
      point_iterator() : entry(0) {}
      /// Invalid?
      bool operator!() const { return !entry; }
      /// Dereference it
      const T& operator*() const { return entry->data; }
   };

   /// Constructor
   PairingHeap();
   /// Constructor
   PairingHeap(const Compare& cmp);
   /// Constructor
   PairingHeap(const value_type* first,const value_type* last);
   /// Constructor
   PairingHeap(const value_type* first,const value_type* last,const Compare& cmp);
   /// Copy-Constructor
   PairingHeap(const PairingHeap& other);
   /// Destructor
   ~PairingHeap();

   /// Assignment
   PairingHeap& operator=(const PairingHeap& other);

   /// Is the heap empty?
   bool empty() const { return !root; }
   /// Returns the number of elements contained in the heap
   size_type size() const { return count; }
   /// Returns the top element of the heap
   const value_type& top() const { return root->data; }
   /// Inserts a new element into the heap
   point_iterator push(const value_type& x);
   /// Removes the top element from the heap
   void pop();

   /// Remove an element from the heap
   void erase(point_iterator pos);
   /// Removes all elements from the heap
   void clear();
   /// Modify an element in the heap
   void modify(point_iterator pos,const T& newValue);

   /** A side heap. A stash allows for reading some or all entries of
     * the main heap an re-adding them cheapily
     */
   class stash
   {
      private:
      /// The heap
      PairingHeap& heap;
      /// The root of the stash heap
      HeapEntry* root;
      /// The bottom of the stash heap
      HeapEntry* bottom;
      /// The count
      unsigned count;

      stash(const stash&);
      void operator=(const stash&);

      public:
      /// Create a stash storage
      explicit stash(PairingHeap& heap);
      /// Destructor
      ~stash();

      /// Pop the top heap entry onto the stash
      void pop_to_stash();
      /// Merge the stash back into the heap
      void push_to_heap();
      /// Clear the stash, deleting all entries stored on it
      void clear();
   };
};
//----------------------------------------------------------------------------
template <typename T,typename Compare> PairingHeap<T,Compare>::PairingHeap()
   : heap(), root(0), cmp(), count(0)
   // Constructor
{
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> PairingHeap<T,Compare>::PairingHeap(const Compare& cmp)
   : heap(), root(0), cmp(cmp), count(0)
   // Constructor
{
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> PairingHeap<T,Compare>::PairingHeap(const value_type* first,const value_type* last)
   : heap(), root(0), cmp(), count(0)
   // Constructor
{
   for (const value_type* iter=first;iter!=last;++iter)
      push(*iter);
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> PairingHeap<T,Compare>::PairingHeap(const value_type* first,const value_type* last,const Compare& cmp)
   : heap(), root(0), cmp(cmp), count(0)
   // Constructor
{
   for (const value_type* iter=first;iter!=last;++iter)
      push(*iter);
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> PairingHeap<T,Compare>::PairingHeap(const PairingHeap& other)
   : heap(), root(0), cmp(), count(0)
   // Constructor
{
   operator=(other);
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> PairingHeap<T,Compare>::~PairingHeap()
   // Destructor
{
   clear();
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> void PairingHeap<T,Compare>::copyFromHeap(HeapEntry* node)
   // Copy heap entries recursively
{
   for (;node;node=node->next) {
      copyFromHeap(node->child);
      push(node->data);
   }
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> PairingHeap<T,Compare>& PairingHeap<T,Compare>::operator=(const PairingHeap& other)
   // Assignment
{
   if (this!=&other) {
      clear();
      copyFromHeap(other.root);
   }
   return *this;
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> void PairingHeap<T,Compare>::flattenForCleanup(HeapEntry* h)
   // Flatten the heap. This destroys the heap property, use only for cleanup!
{
   // Transform all children into sibblings
   for (HeapEntry* iter=h;iter;iter=iter->next) {
      while (iter->child) {
         HeapEntry* n=iter->child->next;
         iter->child->next=iter->next;
         iter->next=iter->child;
         iter->child=n;
      }
   }
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> typename PairingHeap<T,Compare>::HeapEntry* PairingHeap<T,Compare>::merge(HeapEntry* a,HeapEntry* b)
   // Merge two heaps
{
   if (cmp(a->data,b->data)) {
      // Store a as first child of b
      if (b->child)
         b->child->previous=a;
      a->next=b->child;
      a->previous=b;
      b->child=a;

      return b;
   } else {
      // Store b as first child of a
      if (a->child)
         a->child->previous=b;
      b->next=a->child;
      b->previous=a;
      a->child=b;

      return a;
   }
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> typename PairingHeap<T,Compare>::HeapEntry* PairingHeap<T,Compare>::removePrimitive(HeapEntry* root)
   // Remove a heap node. This does not free the entry or unlink it!
{
   // Root without children?
   if (!root->child) {
      root=0;
   } else if (!root->child->next) { // Only one child?
      root=root->child;
      root->previous=0;
   } else { // No, two pass construction
      // First pass: forward
      HeapEntry* iter=root->child,*last=0;
      while (iter) {
         HeapEntry* heap1=iter;
         HeapEntry* heap2=iter->next;
         // Last entry?
         if (!heap2) {
            heap1->previous=last;
            last=heap1;
            break;
         }
         // Merge the heaps
         iter=heap2->next;
         heap1->previous=0; heap1->next=0;
         heap2->previous=0; heap2->next=0;
         HeapEntry* newHeap=merge(heap1,heap2);

         // Link them
         newHeap->previous=last;
         last=newHeap;
      }
      // Second pass: backward
      while (last->previous) {
         HeapEntry* heap1=last;
         HeapEntry* heap2=last->previous;
         HeapEntry* iter=heap2->previous;

         // Merge with the last entry
         heap1->previous=0; heap2->previous=0;
         last=merge(heap1,heap2);

         // And link again
         last->previous=iter;
      }
      root=last;
   }
   return root;
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> typename PairingHeap<T,Compare>::point_iterator PairingHeap<T,Compare>::push(const value_type& x)
   // Inserts a new element into the heap
{
   // Create a new heap
   HeapEntry* newHeap=heap.alloc();
   newHeap->previous=0; newHeap->next=0; newHeap->child=0;
   new (&(newHeap->data)) T(x);
   ++count;

   // And merge with the existing one
   if (root)
      root=merge(root,newHeap); else
      root=newHeap;

   // Return the new heap entry
   point_iterator result;
   result.entry=newHeap;
   return result;
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> void PairingHeap<T,Compare>::pop()
   // Removes the top element from the heap
{
   // Remove the tree node
   HeapEntry* top=root;
   root=removePrimitive(root);

   // Free the entry
   top->data.~T();
   heap.free(top);
   --count;
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> void PairingHeap<T,Compare>::unlink(HeapEntry* entry)
   // Unlink a subheap. entry must not be the root
{
   if (entry->previous->child==entry) {
      // A child
      entry->previous->child=entry->next;
   } else {
      // A sibbling
      entry->previous->next=entry->next;
   }
   if (entry->next)
      entry->next->previous=entry->previous;
   entry->next=0;
   entry->previous=0;
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> void PairingHeap<T,Compare>::removeEntry(HeapEntry* entry)
   // Removes a heap node from the heap but does not free it
{
   // Trying to remove the root node?
   if (entry==root) {
      root=removePrimitive(root);
   } else {
      // No, unlink it
      unlink(entry);

      // Merge the children if any
      HeapEntry* child=removePrimitive(entry);
      if (child)
         root=merge(root,child);
   }
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> void PairingHeap<T,Compare>::erase(point_iterator iter)
   // Removes an element from the heap
{
   HeapEntry* entry=iter.entry;

   // Remove from heap
   removeEntry(entry);

   // Free the entry
   entry->data.~T();
   heap.free(entry);
   --count;
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> void PairingHeap<T,Compare>::clear()
   // Removes all elements from the heap
{
   // Flatten the heap
   flattenForCleanup(root);

   // Call the destructors
   for (HeapEntry* iter=root;iter;iter=iter->next)
      iter->data.~T();

   // And release the heap
   heap.freeAll();
   count=0;
   root=0;
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> void PairingHeap<T,Compare>::modify(point_iterator iter,const T& newValue)
   // Modify an element in the heap
{
   HeapEntry* entry=iter.entry;

   // Is this an increment?
   if (!cmp(newValue,entry->data)) {
      entry->data=newValue;
      // Did we change a non-root value?
      if (entry!=root) {
         unlink(entry);
         root=merge(root,entry);
      }
   } else {
      // No. we must really remove and reinsert
      removeEntry(entry);
      entry->data=newValue;
      if (root)
         root=merge(root,entry); else
         root=entry;
   }
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> PairingHeap<T,Compare>::stash::stash(PairingHeap& heap)
   : heap(heap),root(0),bottom(0),count(0)
   // Create a stash storage
{
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> PairingHeap<T,Compare>::stash::~stash()
   // Destructor
{
   clear();
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> void PairingHeap<T,Compare>::stash::pop_to_stash()
   // Pop the top heap entry onto the stash
{
   // Remove the root node
   HeapEntry* top=heap.root;
   heap.root=heap.removePrimitive(heap.root);
   top->child=0;

   // First object in the stash?
   if (!root) {
      root=bottom=top;
   } else if (bottom&&(!heap.cmp(bottom->data,top->data))) {
      bottom->child=top;
      top->previous=bottom;
      bottom=top;
   } else {
      root=heap.merge(root,top);
      bottom=0;
   }

   // Update the counters
   ++count;
   --heap.count;
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> void PairingHeap<T,Compare>::stash::push_to_heap()
   // Merge the stash back into the heap
{
   if (root) {
      // Merge with the heap
      if (heap.root)
         heap.root=heap.merge(root,heap.root); else
         heap.root=root;
      heap.count+=count;

      // Reset the pointers
      root=0;
      bottom=0;
      count=0;
   }
}
//----------------------------------------------------------------------------
template <typename T,typename Compare> void PairingHeap<T,Compare>::stash::clear()
   // Clear the stash, deleting all entries stored on it
{
   // Flatten the stash
   flattenForCleanup(root);

   // Call the destructors and release the entries
   for (HeapEntry* iter=root,*next;iter;iter=next) {
      next=iter->next;
      iter->data.~T();
      heap.heap.free(iter);
   }

   // Reset the pointers
   root=0;
   bottom=0;
   count=0;
}
//----------------------------------------------------------------------------
#endif
