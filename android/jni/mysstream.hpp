//This file is part of Cassis.

//Copyright (C) 2012, Erik Saule

//Cassis is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Cassis is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with Cassis.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MYSSTREAM_H
#define MYSSTREAM_H

#include "mystring.hpp"

#include <stdio.h>

class stringstream
{
  MyString s;

public:
  const MyString& str(){return s;}
  void str(const MyString& param){this->s = param;}
  
  void append(const MyString &param)
  {
    this->s.append(param);
  }
};



static stringstream& operator<< (stringstream& ss, const MyString& s)
{
  ss.append(s);
  return ss;
}

static stringstream& operator<< (stringstream& ss, int i)
{
  char buffer[sizeof(int)*3+1];

  sprintf (buffer, "%d", i);
  MyString s (buffer);
  ss.append(s);
  return ss;
}

#endif
