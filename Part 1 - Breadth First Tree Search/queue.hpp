template <class T>
bool ArrayQueue<T>::isEmpty() const
{
  if (m_first == m_last)
    return true;
  else
    return false;
}

template <class T>
const T& ArrayQueue<T>::front() const throw (Oops)
{
  if (!(isEmpty()))
    return m_data[m_first];
  else
    throw Oops("Error = front() empty list");
}

template <class T>
const T& ArrayQueue<T>::back() const throw (Oops)
{
  if (!(isEmpty()))
  {
    if ((m_last-1)<0)
    {
      return m_data[m_max+(m_last-1)];
    }
    else
    {
      return m_data[m_last-1];
    }
  }
  else
    throw Oops("Error = back() empty list");
}

template <class T>
void ArrayQueue<T>::enqueue(const T& x)
{
	if (m_max == 0)
  {
    m_max++;
  }
  if ((m_last+1) % m_max == m_first)
  {
    T*tmp = new T[m_max*2];
    if (!isEmpty())
    {
	    for (int k = 0; k < m_max; k++)
	    {
	      tmp[k] = m_data[(m_first+k)%m_max];
	    }
		}
		delete [] m_data;
    m_data = tmp;
    m_first = 0;
    m_last = m_max - 1;
    m_max = m_max *2;
  }
  m_data[m_last]=x;
  m_last = (m_last + 1) % m_max;
  return;
}

template <class T>
void ArrayQueue<T>::dequeue()
{
  if (!(isEmpty()))
    m_first = (m_first + 1) % m_max;
  return;
}

template <class T>
void ArrayQueue<T>::clear()
{
  m_first = 0;
  m_last = 0;
  m_max = 0;
  delete [] m_data;
  m_data = NULL;
  return;
}

template <class T>
ArrayQueue<T>& ArrayQueue<T>::operator = (const ArrayQueue<T>&rhs)
{
  if (this != &rhs)
  {
  	if (rhs.m_max > m_max)
    {
      delete[] m_data;
    	m_data = new T[rhs.m_max];
    }
    for (int k = 0; k < rhs.m_max; k++)
    {
      m_data[k] = rhs.m_data[k];
    }
    m_first = rhs.m_first;
    m_last = rhs.m_last;
    m_max = rhs.m_max;
  }
  return *this;
}
