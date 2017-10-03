#include <iostream>
using namespace std;

template <class T>
const LinkedList<T>& LinkedList<T> :: operator = (const LinkedList<T>& rhs)
{
	if (this != &rhs)
	{
		clear();
		LinkedList <T>* p = this;
		const LinkedList <T>* q = &rhs;
	  while (q->m_next != NULL)
		{
		  p -> m_data = q -> m_data;
		  p->m_next = new LinkedList<T>;
	    p = p -> m_next;
	    q = q -> m_next;
	  }
	}
  return *this;
}

template <class T>
int LinkedList<T> :: size() const
{
  int counter = 0;
	LinkedList<T>* p = m_next;
	while (p != NULL)
	{
		counter++;
		p = p->m_next;
	}
	return (counter);
}

template <class T>
bool LinkedList<T> :: isEmpty() const
{
	bool isEmpty = false;
	if (m_next == NULL)
	{
		isEmpty = true;
	}
	return (isEmpty);
}

template <class T>
LinkedList<T>* LinkedList<T> :: getFirstPtr()
{
	LinkedList<T>* p = this;
	if (p != NULL)
	{
		return p;
	}
	else
	{
		cout<<"!!-- ERROR : PANIC!! in LINKEDLIST.getFirstPtr() - empty list"<<endl;
		return NULL;
	}
}

template <class T>
void LinkedList<T> :: clear()
{
  if(m_next != NULL)
  {
    m_next -> clear();
   
    delete m_next;
    m_next = NULL; 
  }
}

template <class T>
void LinkedList<T> :: insert_front(const T& x)
{
  insert (x, this);
}

template <class T>
void LinkedList<T> :: insert(const T& x, LinkedList<T>* pos)
{
	if (pos != NULL)
	{
		LinkedList<T>* p = new LinkedList<T>;
	  p->m_data = pos->m_data;
	  p->m_next = pos->m_next;
	  pos->m_data = x;
	  pos->m_next = p;
  }
  else
  {
		cout<<"!!-- ERROR : PANIC!! in LINKEDLIST.insert()";
		cout<<" -  invalid index"<<endl;
	}
}


template <class T>
LinkedList<T>* LinkedList<T> :: findIndex(const int index)
{
  LinkedList<T>* p = this;

  while(p->m_data.index != index && p->m_next != NULL)
  {
    p = p->m_next;
  }

  return p;
}

template <class T>
void LinkedList<T> :: reverse()
{
	LinkedList<T>* tmp = new LinkedList;
	LinkedList<T>* p = this;
	while (p->m_next != NULL)
	{
		tmp -> insert_front(p->m_data);
		p = p-> m_next;
	}
	*this = *tmp;
	tmp->clear();
	delete tmp;
	return;
}
