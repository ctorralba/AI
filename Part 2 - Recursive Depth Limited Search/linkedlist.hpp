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
LinkedList<T> :: LinkedList (const LinkedList<T>& rhs)
{
  m_next = new LinkedList<T>;
  *this = rhs;
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
		cout<<"!!-- ERROR: LINKEDLIST.getFirstPtr() - empty list"<<endl;
		return NULL;
	}
}

template <class T>
LinkedList<T>* LinkedList<T> :: getLastPtr()
{
  if (!(isEmpty()))
  {
  	LinkedList<T>*tmp = this;
  	while (tmp -> m_next != NULL)
  	{
  		if(tmp->m_next->m_next == NULL)
  		  {
  		    return tmp;
  		  }
  		else
  		 {
  		   tmp = tmp -> m_next;
  	   }
  	}
  	return tmp;
  }
  else
  {
    cout<<"!!-- ERROR: LINKEDLIST.getLastPtr() - empty list"<<endl;
		return NULL;
	}
}

template <class T>
LinkedList<T>* LinkedList<T> :: getAtPtr(int i)
{
	if (i<size() && i>=0)
	{
		if (m_next != NULL)
		{
		  LinkedList* p = this;
			for (int k = 0; k < i; k++)
		  {
		  	p = p -> m_next;
		  }
			if (p != NULL)
			{
				return p;
			}
			else
			{
				return NULL;
			}
	  }
	  else
	  {
	  	cout<<"!!-- ERROR: LINKEDLIST.getAtPtr()";
			cout<<" - empty list"<<endl;
	  	return NULL;
	  }
  }
  else
   {
	  	cout<<"!!-- ERROR: LINKEDLIST.getAtPtr()";
			cout<<" -  invalid index"<<endl;
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
		cout<<"!!-- ERROR: LINKEDLIST.insert()";
		cout<<" -  invalid index"<<endl;
	}
}



template <class T>
void LinkedList<T> :: remove(LinkedList<T>* pos)
{
	if (pos != NULL)
	{
		LinkedList<T>*tmp = pos -> m_next;
		pos->m_data = tmp->m_data;
		pos->m_next = tmp->m_next;
		delete tmp;
  }
	else
  {
	 	cout<<"!!-- ERROR : LINKEDLIST.remove()";
		cout<<" -  invalid index"<<endl;
  }
}

template <class T>
bool LinkedList<T> :: operator == (const LinkedList<T>& rhs) const
{	
	const LinkedList<T>*p = this;
	const LinkedList<T>*q = &rhs;
	
	while ((p->m_next != NULL && p->m_next->m_next != NULL)&&
	       (q->m_next != NULL && q->m_next->m_next != NULL))
	{
		p = p->m_next;
		q = q->m_next;
		if (p->m_data != q->m_data)
		{
			return false;
		}
  }
		return true;
}

template <class T>
LinkedList<T>* LinkedList<T> :: find(const T& x)
{
  LinkedList<T>* p = this;

  while(p->m_data != x && p->m_next != NULL)
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

template <class T>
void LinkedList<T> :: append(const LinkedList<T>& xlist)
{
  LinkedList<T>* p = getLastPtr();
  const LinkedList<T>* ptr = &xlist;

  delete p->m_next;
  p->m_next = new LinkedList<T>;
  p = p->m_next;

  while(ptr->m_next != NULL)
  {
    p->m_next = new LinkedList<T>;
    p->m_data = ptr->m_data;
    ptr = ptr->m_next;
    p = p->m_next;
  }
}
