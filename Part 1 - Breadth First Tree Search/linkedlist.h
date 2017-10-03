#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

template <class T>
class LinkedList
{
public:
  T m_data;                  // Data to be stored
  LinkedList<T>* m_next;     // Pointer to the next element in the list
	
    // Default constructor
    // Post: next pointer set to NULL
  LinkedList() : m_next(NULL) {}

    // Auxiliary constructor, construct from parameters
    // Post: data and next pointer set to parameters
  LinkedList(const T& x, LinkedList<T>* p) 
             : m_data(x), m_next(p) {}

    //Destructor
  ~LinkedList() {}

    // Performs a deep copy of the data from rhs into this linked list
    // Parameters: rhs is linked list to be copied
    // Returns: *this
    // Post: this list contains same data values (in the same order)
    //     as are in rhs; any memory previously used by this list has been
    //     deallocated.
  const LinkedList<T>&  operator= (const LinkedList<T>& rhs);

  

    // Accessor function for the current # data values in the list
    // Returns: current size of the list
  int size() const;
    
    // Determines whether the list is empty
    // Returns: 'true' if the list is empty; 'false' otherwise
  bool isEmpty() const;

    // Get a pointer to the first element's node
    // Returns: pointer to the first node in the list; 
    //     returns NULL if the list is empty
  LinkedList<T>* getFirstPtr();

    // Accessor function for a node in the list
    // Returns: pointer to the node at the i'th position in the list; 
    //     returns NULL if no such element exists.
  LinkedList<T>* getAtPtr(int i);

    // Empties the list
    // Turn the list into an empty list.
    // Post: ALL dynamically allocated memory for elements is deallocated
  void clear();

    // Puts the data x in the front of the list 
    // Parameters: x is data value to inserted
    // Post: x is the first element of the list
  void insert_front(const T& x);

    // Puts the data value x at the position pointed by pos
    // Parameters: x is data value to inserted
    //     pos pointer to the position to insert x at.
    // Pre: pos is a pointer in this list.
    // Post: x is inserted at the position pointed by pos
  void insert(const T& x, LinkedList<T>* pos);
  
  	// Determines whether animal with index is in the list
    // Parameters: index is data value to be found
    // Returns: a pointer to the position of the animal with the index in the list; 
    //     otherwise, NULL
  LinkedList<T>* findIndex(const int index);  
  
    // Purpose: reverses the elements from the list
    // Post: the list is now in reverse order
  void reverse();
};


#include "linkedlist.hpp"

#endif
