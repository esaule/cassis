#ifndef MYSTRING_H
#define MYSTRING_H

class MyString
{
  typedef int size_t;
  char* str;
  size_t allocsize;
  size_t actsize; //as returned by strlen so in byte and without \0

  MyString& operator= (const MyString&);//not implemented yet

public:
  //pack alloc on copy
  MyString(const MyString& ms)
    :str (new char[ms.actsize+1]),
     allocsize(ms.actsize+1),
     actsize(ms.actsize)
  {
    memcpy(str, ms.str, sizeof(char)*(actsize+1));
  }

  MyString()
    :str(new char[1]), allocsize(1), actsize(0)
  {
    str[0] = '\0';
  }

  MyString(const char* s)
  {
    actsize = strlen(s);
    allocsize = actsize+1;
    memcpy(str, s, sizeof(char)*(actsize+1));
  }

  //make sure a string of actsize size can be stored
  void reserve (size_t size)
  {
    if (size >= this->allocsize)
      {
	char *n = new char[size+1];
	memcpy(n, str, sizeof(char)*(actsize+1));
	delete[] str;
	this->allocsize = size+1;
	str = n;
      }
  }

  void append (const MyString& ms)
  {
    //increase size if necessary
    reserve(ms.actsize + this->actsize);
    
    memcpy(str+actsize, ms.str, sizeof(char)*(ms.actsize+1));
  }

  bool operator== (const MyString& ms) const
  {
    if (ms.actsize != this->actsize)
      return false;

    //assert ms.actsize == this->actsize
    for (size_t i = 0; i< actsize; ++i)
      if (str[i] != ms.str[i])
	return false;

    return true;
  }

  bool operator != (const MyString& ms) const
  {
    return ! this->operator==(ms);
  }

  const MyString& operator+= (const MyString ms)
  {
    this->append(ms);
    return *this;
  }

  ~MyString()
  {
    if (str != NULL)
      delete[] str;
  }

  operator const char *() const
  {
    return str;
  }


  const char * c_str() const
  {
    return str;
  }
};

#endif
