#ifndef MYMAP_HPP
#define MYMAP_HPP

#include <utility>

template<typename A>
class myset
{
public:
  typedef int size_t;

protected:
  A* buffer;
  size_t allocsize;
  size_t actsize;

public:
  myset()
    :buffer(NULL), allocsize(0), actsize(0)
  {
  }

  void reserve (size_t size)
  {
    if (size > allocsize)
      {
	A* t = new A[size];
	if (buffer != NULL)
	  delete[] buffer;
	buffer = t;
	allocsize = size;
      }
  }

  const A& operator[] (size_t i) const
  {
    return buffer[i];
  }

  A& operator[] (size_t i)
  {
    return buffer[i];
  }

  size_t size() const
  {
    return actsize;
  }

  void insert (const A& a)
  {
    if (actsize+1> size)
      reserve ((actsize+1)*2);
    
    //assert (actsize+1 > size)
    buffer[actsize] = a;
    actsize ++;
  }

  ~myset()
  {
    if (buffer != NULL)
      delete[] buffer;
  }

};


template<typename A, typename B>
class mymap
{
  myset< std::pair<A,B> > buf;
public:
  typedef int size_t;

  mymap()
  {
  }

  size_t size() const
  {
    return buf.size();
  }
  
  size_t count(const A& a)
  {
    size_t s = buf.size();
    size_t c = 0;
    for (size_t i = 0; i<s; ++i)
      {
	if (buf[i].first == a)
	  ++c;
      }
    return c;
  }
  
  
  B& operator[] (const A&) const
  {
    
  }
};

#endif
