#ifndef EXCEP_H
#define EXCEP_H

class BaseException
{
public:
  virtual ~BaseException(){}
};

class NotImplementedYetException : public BaseException
{
public:
  virtual ~NotImplementedYetException(){}
};


#endif
