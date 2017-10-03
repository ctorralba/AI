#ifndef ABSTRACTQUEUE_H
#define ABSTRACTQUEUE_H

#include <string>

using namespace std;

/* ---------------  Class 'Oops' ---------------
Thrown when an error is encountered.
Member 'm_msg' stores an error message.
*/
class Oops
{
	string m_errormsg;
public:
	Oops(const string &msg) : m_errormsg(msg) {}
	const string& getMsg() const
	{
		return m_errormsg;
	}
};

template < typename T >
class AbstractQueue
{
public:
  // Purpose: Checks if a queue is empty
  // Returns: 'true' if the queue is empty else
  //     'false'
  virtual bool isEmpty() const = 0;


  // Purpose: looks at the front of the queue
  // Returns: a reference to the element currently in front of the queue
  virtual const T& front() const throw (Oops) = 0;


  // Purpose: looks at the back of the queue
  // Returns: a reference to the element currently in the back of the queue
  virtual const T& back() const throw (Oops) = 0;


  // Purpose: enqueue an element into the queue
  // Parameters: x is the item to add to the queue
  // Post: x is now the element at the end of the queue,
  virtual void enqueue(const T& x) = 0;


  // Purpose: dequeues
  // Post: the element formerly at the front of the queue has
  //     been removed.
  // Note: Dequeueing an empty queue results in an empty queue.
  virtual void dequeue() = 0;


  // Purpose: clears the queue
  // Post: the queue is now empty
  virtual void clear() = 0;


  // destructor
  virtual ~AbstractQueue() {}

};

#endif
