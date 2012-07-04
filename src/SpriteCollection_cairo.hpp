#ifndef __SPRITE_COLECTION_H__
#define __SPRITE_COLECTION_H__

#include <assert.h>
#include <map>

class SpriteCollection
{
  std::map<std::string, cairo_surface_t*> storage;

  const char* path;

  bool load(const std::string& name)
  {
    const bool debug = false;
    if (debug)
      std::cerr<<"SpriteCollection. loading: "<<name<<std::endl;
    std::string s=path;
    s += "/";
    s += name;

    cairo_surface_t* sp = cairo_image_surface_create_from_png(s.c_str());
    switch (cairo_surface_status(sp))
      {
      case CAIRO_STATUS_NO_MEMORY :
      case CAIRO_STATUS_FILE_NOT_FOUND :
      case CAIRO_STATUS_READ_ERROR :
	std::cerr<<"can not load image \""<<name<<"\""<<std::endl;
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
    std::cerr<<"Sprite Collection. size="<<storage.size()<<std::endl;
  }

  cairo_surface_t* getSprite(const std::string& name)
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
