#ifndef LIST_HPP
#define LIST_HPP
/* List.hpp
 *
 * doubly-linked, double-ended list with Iterator interface
 * EECS 280 List/Editor Project
 */

#include <iostream>
#include <iterator> //std::bidirectional_iterator_tag
#include <cassert>  //assert


template <typename T>
class List {
  //OVERVIEW: a doubly-linked, double-ended list with Iterator interface
public:

  // Add Default constructor
  List() : first(nullptr), last(nullptr), num_elements(0){}

  // Destructor
  ~List() { clear(); }

  // Copy constructor
  List(const List &other) : first(nullptr), last(nullptr), num_elements(0) {
    copy_all(other);
  }

  // Assignment operator
  List &operator=(const List &other) {
    if(this != &other) {
      clear();
      copy_all(other);
    }
    return *this;
  }

  //EFFECTS:  returns true if the list is empty
  bool empty() const{
    return num_elements == 0;
  }

  //EFFECTS: returns the number of elements in this List
  //HINT:    Traversing a list is really slow. Instead, keep track of the size
  //         with a private member variable. That's how std::list does it.
  int size() const{
    return num_elements;
  }

  //REQUIRES: list is not empty
  //EFFECTS: Returns the first element in the list by reference
  T & front(){
    return first->datum;
  }

  //REQUIRES: list is not empty
  //EFFECTS: Returns the last element in the list by reference
  T & back(){
    return last->datum;
  }

  //EFFECTS:  inserts datum into the front of the list
  void push_front(const T &datum){
    Node *newNode = new Node();
    newNode->datum = datum;
    newNode->prev = nullptr;
    newNode->next = first;

    if(empty()){
      last = newNode;
    } else {
      first->prev = newNode;
    }

    first = newNode;
    num_elements++;
  }

  //EFFECTS:  inserts datum into the back of the list
  void push_back(const T &datum){
    Node *newNode = new Node();
    newNode->datum = datum;
    newNode->next = nullptr;
    newNode->prev = last;

    if(empty()){
      // if the list is empty its also the first node
      first = newNode;
    } else {
      last->next = newNode;
    }

    last = newNode;
    num_elements++;
  }

  //REQUIRES: list is not empty
  //MODIFIES: invalidates all iterators to the removed element
  //EFFECTS:  removes the item at the front of the list
  void pop_front(){
    Node *placeHolder = first;
    first = first->next;

    // check if theres just one element
    if(first) {
      first->prev = nullptr;
    } else {
      last = nullptr;
    }
    
    delete placeHolder;
    num_elements--;
  }

  //REQUIRES: list is not empty
  //MODIFIES: invalidates all iterators to the removed element
  //EFFECTS:  removes the item at the back of the list
  void pop_back(){
    Node *placeHolder = last;
    last = last->prev;

    // check if theres just one element
    if(last) {
      last->next = nullptr;
    } else {
      first = nullptr;
    }
    
    delete placeHolder;
    num_elements--;
  }

  //MODIFIES: invalidates all iterators to the removed elements
  //EFFECTS:  removes all items from the list
  void clear(){
    while (!empty()) {
        pop_front();
    }
  }

  // You should add in a default constructor, destructor, copy constructor,
  // and overloaded assignment operator, if appropriate. If these operations
  // will work correctly without defining these, you should omit them. A user
  // of the class must be able to create, copy, assign, and destroy Lists.

private:
  //a private type
  struct Node {
    Node *next;
    Node *prev;
    T datum;
  };

  //REQUIRES: list is empty
  //EFFECTS:  copies all nodes from other to this
  void copy_all(const List<T> &other){
    for (Node *n = other.first; n != nullptr; n = n->next) {
        push_back(n->datum);
    }
  }

  Node *first;   // points to first Node in list, or nullptr if list is empty
  Node *last;    // points to last Node in list, or nullptr if list is empty
  int num_elements;

public:
  ////////////////////////////////////////
  class Iterator {
  public:
    //OVERVIEW: Iterator interface to List

    // Add a default constructor here. The default constructor must set both
    // pointer members to null pointers.
    Iterator() : list_ptr(nullptr), node_ptr(nullptr) {}
    // Add custom implementations of the destructor, copy constructor, and
    // overloaded assignment operator, if appropriate. If these operations
    // will work correctly without defining these, you should omit them. A user
    // of the class must be able to copy, assign, and destroy Iterators.


    // Your iterator should implement the following public operators:
    // *, ++ (both prefix and postfix), == and !=.
    Iterator &operator++(){
      assert(node_ptr);
      node_ptr = node_ptr -> next;
      return *this;
    }

    Iterator operator++(int dummy) {  // i++
      Iterator copy = *this;
      operator++();
      return copy;
    }

    bool operator==(const List::Iterator &other) const {
      return node_ptr == other.node_ptr && list_ptr == other.list_ptr;
    }

    bool operator!=(const List::Iterator &other) const {
      return !(*this == other);
    }

    T &operator*() const {
      return node_ptr -> datum;
    }
    // Equality comparisons must satisfy the following rules:
    // - two default-constructed iterators must compare equal
    // - a default-constructed iterator must compare unequal to an
    //   iterator obtained from a list, even if it is the end iterator
    // - two iterators to the same location in the same list must
    //   compare equal
    // - two iterators to different locations in the same list must
    //   compare unequal
    // - comparing iterators obtained from different lists results in
    //   undefined behavior
    //   - Your implementation can handle this however you like, but
    //     don't assume any particular behavior in your test cases --
    //     comparing iterators from different lists essentially
    //     violates the REQURES clause.
    // Note: comparing both the list and node pointers should be
    // sufficient to meet these requirements.



    // Type aliases required to work with STL algorithms. Do not modify these.
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    // This operator will be used to test your code. Do not modify it.
    // REQUIRES: Iterator is decrementable. All iterators associated with a
    //           list are decrementable, including end iterators, except for
    //           begin iterators (those equal to begin() on the list)
    // EFFECTS:  moves this Iterator to point to the previous element
    //           and returns a reference to this Iterator
    Iterator& operator--() { // prefix -- (e.g. --it)
      assert(list_ptr);
      assert(*this != list_ptr->begin());
      if (node_ptr) {
        node_ptr = node_ptr->prev;
      } else { // decrementing an end Iterator moves it to the last element
        node_ptr = list_ptr->last;
      }
      return *this;
    }

    // This operator will be used to test your code. Do not modify it.
    // REQUIRES: Iterator is decrementable. All iterators associated with a
    //           list are decrementable, including end iterators, except for
    //           begin iterators (those equal to begin() on the list)
    // EFFECTS:  moves this Iterator to point to the previous element
    //           and returns a copy of the original Iterator
    Iterator operator--(int /*dummy*/) { // postfix -- (e.g. it--)
      Iterator copy = *this;
      operator--();
      return copy;
    }

    // REQUIRES: Iterator is dereferenceable
    // EFFECTS: returns the underlying element by pointer
    // NOTE: T must be a class type to use the syntax it->. If T has a
    //       member variable f, then it->f accesses f on the
    //       underlying T element.
    T* operator->() const {
      return &operator*();
    }

  private:
    const List *list_ptr; //pointer to the List associated with this Iterator
    Node *node_ptr; //current Iterator position is a List node
    // add any additional necessary member variables here


    // add any friend declarations here
    friend class List;


    // construct an Iterator at a specific position in the given List
    Iterator(const List *lp, Node *np) : list_ptr(lp), node_ptr(np){}

  };//List::Iterator
  ////////////////////////////////////////

  // return an Iterator pointing to the first element
  Iterator begin() const {
    return Iterator(this, first);  // "this" is the List*, first is the first Node*
  }

  Iterator end() const {
      return Iterator(this, nullptr);  // nullptr = past-the-end sentinel
  }

  //REQUIRES: i is a valid, dereferenceable iterator associated with this list
  //MODIFIES: invalidates all iterators to the removed element
  //EFFECTS: Removes a single element from the list container.
  //         Returns An iterator pointing to the element that followed the
  //         element erased by the function call
  Iterator erase(Iterator i){
    Node *toDelete = i.node_ptr;
    Node *next = toDelete->next;


    if(toDelete->prev){ // make sure it is not the 1st element
      // replace the last elements pointer with the next element
      toDelete->prev->next = toDelete-> next; 
    } else {
      first = toDelete->next;
    }

    if(toDelete->next){ // make sure it's not the last element
      toDelete->next->prev = toDelete->prev;
    } else{
      last = toDelete->prev;
    }

    // delete the element with no pointers
    delete toDelete;
    --num_elements;
    return Iterator(this, next);
  }

  //REQUIRES: i is a valid iterator associated with this list
  //EFFECTS: Inserts datum before the element at the specified position.
  //         Returns an iterator to the the newly inserted element.
  Iterator insert(Iterator i, const T &datum){
    Node *newNode = new Node();
    newNode->datum = datum;
    newNode->next = i.node_ptr; 
  
    // cases for if at the end, beginning, or middle
    if(i.node_ptr == nullptr){
      newNode->prev = last;
      // if empty list
      if(last) last->next = newNode;
      else first = newNode;
      last = newNode;
    } else if (i.node_ptr->prev == nullptr){
      newNode->prev = nullptr;
      i.node_ptr->prev = newNode;
      first = newNode;
    } else {
      newNode->prev = i.node_ptr->prev;
      i.node_ptr->prev->next = newNode;
      i.node_ptr->prev = newNode;
    }
    ++num_elements;
    return Iterator(this, newNode);
  }

};


////////////////////////////////////////////////////////////////////////////////
// Add your member function implementations below or in the class above
// (your choice). Do not change the public interface of List, although you
// may add the Big Three if needed.  Do add the public member functions for
// Iterator.


#endif // Do not remove this. Write all your code above this line.
