#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "exception.hpp"

class DeserializeException : public BaseException
{
public:
  DeserializeException(){}
  virtual ~DeserializeException(){}
};

class Serializable
{
public:
  typedef int size_t;
  
  ///returns the amount of memory needed to save the state of the object
  virtual size_t serializesize() const = 0;
  ///Save the state of the object in the buffer given as a parameter.
  ///The size of the buffer is assumed to be >= serializesize();
  virtual void serialize(char* c) const = 0;

  ///Restore the state based on the buffer given as a parameter. If
  ///the buffer is incoherent, send a DeserializeException.
  virtual void deserialize(const char* c) = 0;

};

#endif
