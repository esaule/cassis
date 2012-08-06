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
  b = tmp;
}

template<class iterator, class lessthancomparator>
iterator partition(iterator low, iterator high, lessthancomparator& comp)
{
  typename iterator_traits<iterator>::value_type num = *low;

  iterator i = low+1;
  iterator j = high;
  
  while( 1 )
    {
      
      while( i<high && comp(*i,num) )
	i++;
      
      while( comp(num,*j) )
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

template<class iterator, class lessthancomparator>
void quicksort(iterator low, iterator high, lessthancomparator& comp )
{
  iterator j;
  
  if( low<high )
    {
      j = partition<iterator, lessthancomparator>(low, high, comp);
      quicksort (low, (j-1), comp);
      quicksort ((j+1), high, comp);
    }
}

///sort the range between low (included) and high (excluded), using the comp as a comparator function. The algorithm is bubblesort
template<class iterator, class lessthancomparator>
void bubblesort(iterator low, iterator high, lessthancomparator& comp )
{
  bool done = false;
  while (!done)
    {
      done = true;
      for (iterator it = low; it+1 != high; ++it)
	{
	  if (comp(*it, *(it+1))) {
	    swap(*it, *(it+1));
	    done = false;
	  }
	}
    }
}

///sort the range between low (included) and high (excluded), using the comp as a comparator function
template<class iterator, class lessthancomparator>
void sort(iterator low, iterator high, lessthancomparator& comp)
{
  bubblesort(low, high, comp);
  //quicksort(low, high-1, comp);
}

#endif
