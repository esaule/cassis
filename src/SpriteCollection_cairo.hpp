#ifndef __SPRITE_COLECTION_H__
#define __SPRITE_COLECTION_H__

#include <assert.h>
#ifndef USE_MYMAP
#include <map>
#else
#include "mymap.hpp"
#endif

#ifdef NOSTRING
#include "mystring.hpp"
#else
#include <string>
#endif

class SpriteCollection
{
#ifdef NOSTRING
  typedef MyString string;
#endif

#ifndef USE_MYMAP
  std::map<string, cairo_surface_t*> storage;
#else
  mymap<string, cairo_surface_t*> storage;
#endif

  const string path;

  bool load(const string& name)
  {
    const bool debug = false;
    //    if (debug)
    //      std::cerr<<"SpriteCollection. loading: "<<name<<std::endl;
    string s=path;
    s += "/";
    s += name;

    cairo_surface_t* sp = NULL;

#ifdef CAIRO_HAS_PNG_FUNCTIONS
    if (sp == NULL) 
      sp = cairo_image_surface_create_from_png(s.c_str());
#endif
    
    if (sp == NULL) return false;

    switch (cairo_surface_status(sp))
      {
      case CAIRO_STATUS_NO_MEMORY :
      case CAIRO_STATUS_FILE_NOT_FOUND :
      case CAIRO_STATUS_READ_ERROR :
	//	std::cerr<<"can not load image \""<<name<<"\""<<std::endl;
	cairo_surface_destroy(sp);
	return false;
	break;
      default:
	storage[name] = sp;
	return true;
	break;
      }
  }
  
public:
  SpriteCollection (const char* p)
    :path(p)
  {
  }

  void print_info()
  {
    //    std::cerr<<"Sprite Collection. size="<<storage.size()<<std::endl;
  }

  cairo_surface_t* getSprite(const string& name)
  {
    if (storage.count(name) != 0)
      {
	assert (storage.count(name) == 1);
	return storage[name];
      }
    else
      {
	if (load(name))
	  return storage[name];
	else
	  return NULL;
      }
  }
  
};


#endif
