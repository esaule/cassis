#ifndef MYSORT_H_
#define MYSORT_H_


template <class Iterator> struct iterator_traits {
  typedef typename Iterator::difference_type difference_type;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::reference reference;
};

template<class T> struct iterator_traits<T*> {
  typedef T* difference_type;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
};

template <class T> struct iterator_traits<const T*> {
  typedef ptrdiff_t difference_type;
  typedef T value_type;
  typedef const T* pointer;
  typedef const T& reference;
};

template <class T> void swap(T& a, T& b)
{
  T tmp = a;
  a = b;
  b = a;
}

template<class iterator, class comparator>
iterator partition(iterator low, iterator high, comparator& comp)
{
  iterator i = low+1;
  iterator j = high;
  
  while( 1 )
    {
      
      while( i<high && comp(*i,*low) )
	i++;
      
      while( comp(*low,*j) )
	j--;
      
      if( comp(*i, *j) )
	swap(*i, *j);
      else
	{
	  swap(*low, *j);
	  return(j);
	}
    }
}

template<class iterator, class comparator>
void quicksort(iterator low, iterator high, comparator& comp )
{
  iterator j;
  
  if( low<high )
    {
      j = partition<iterator, comparator>(low, high, comp);
      quicksort (low, (j-1), comp);
      quicksort ((j+1), high, comp);
    }
}

template<class iterator, class comparator>
void sort(iterator low, iterator high, comparator& comp)
{
  quicksort(low, high, comp);
}

#endif
