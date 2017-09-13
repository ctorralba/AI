#include "abstractqueue.h"
#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
using namespace std;

template <class T>
class ArrayQueue:public AbstractQueue<T>
{
  private:
    int m_first;	//index of first element of the queue
    int m_last;		//index to last element of the queue
    int m_max;		//the container space of the queue
    
  public:
  	T*m_data;		//data for the queue
  	//Default Constructor
  	//Post: m_first, m_last, m_max set to 0. m_data set to Null.
    ArrayQueue():m_first(0),m_last(0),m_max(0),m_data(NULL){};
    
    // Determines whether the queue is empty
    // Returns: 'true' if the queue is empty; 'false' otherwise
    bool isEmpty() const;
    
    // Returns the element that is in front of the queue
    // If error throw oops
    const T& front() const throw (Oops);
    
    //Returns the element that is in the back of queue
    //error throws oops
    const T& back() const throw (Oops);
    
    //Parameters: the element that is being stored in the queue.
    //Post: element is added to the back of the queue.
    void enqueue(const T& x);
    
    //Post: Dequeue's the queue. The first element of the queue is popped off.
    void dequeue();
    
    //Post: Clears the queue.
    //		Dynamically allocated data is deallocated.
    void clear();
    ArrayQueue<T>& operator = (const ArrayQueue<T>& rhs);
    int getFirst() const {return m_first;}
    int getLast() const {return m_last;}
    int getMax() const  {return m_max;}  
};

#include "queue.hpp"
#endif
