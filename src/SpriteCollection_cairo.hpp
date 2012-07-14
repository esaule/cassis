#ifndef __SPRITE_COLECTION_H__
#define __SPRITE_COLECTION_H__

#include "cairo.h"

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

#ifdef ANDROID
#include <android/asset_manager.h>
#include <android/log.h>

cairo_status_t android_png_cairo_wrapper_read (void *closure,
					       unsigned char *data,
					       unsigned int length);

#endif



class SpriteCollection
{
#ifdef NOSTRING
  typedef MyString string;
#else
  typedef std::string string;
#endif


#ifndef USE_MYMAP
  std::map<string, cairo_surface_t*> storage;
#else
  mymap<string, cairo_surface_t*> storage;
#endif

  const string path;

#ifdef ANDROID
  cairo_surface_t* android_cairo_image_surface_create_from_png(const char* path)
  {
    // LOGI("opening directory");
    // AAssetDir* ad = AAssetManager_openDir (am, "");
    // const char* filename;
    // while ((filename = AAssetDir_getNextFileName(ad)) != NULL)
    //   {
    // 	LOGI("%s", filename);
    //   }
    // AAssetDir_close(ad);
    // LOGI("done");

    //    LOGI("android_cairo_image_surface_create_from_png(\"%s\")", path);

    if (am == NULL)
      {
	//LOGI("am == NULL");
	return NULL;
      }

    AAsset* as = AAssetManager_open (am, path, AASSET_MODE_STREAMING);

    if (as == NULL)
      {
	//	LOGI("as == NULL");
	return NULL;
      }

    cairo_surface_t* sur = cairo_image_surface_create_from_png_stream (android_png_cairo_wrapper_read, as);

    AAsset_close(as);
    
    if (sur == NULL)
      {
	//LOGI("sur == NULL");
	return NULL;
      }

    if (cairo_surface_status(sur) == CAIRO_STATUS_NO_MEMORY ||
	cairo_surface_status(sur) == CAIRO_STATUS_READ_ERROR)
      {
	//LOGI("cairo_surface_status() error");
	cairo_surface_destroy(sur);
	return NULL;
      }

    return sur;
  }  
#endif

  bool load(const string& name)
  {
    //LOGI("load(\"%s\")", name.c_str());

    const bool debug = false;
    //    if (debug)
    //      std::cerr<<"SpriteCollection. loading: "<<name<<std::endl;
    string s=path;
    s += "/";
    s += name;

    cairo_surface_t* sp = NULL;

#ifdef CAIRO_HAS_PNG_FUNCTIONS
    if (sp == NULL) 
      {
	sp = cairo_image_surface_create_from_png(s.c_str());

	switch (cairo_surface_status(sp))
	  {
	  case CAIRO_STATUS_NO_MEMORY :
	  case CAIRO_STATUS_FILE_NOT_FOUND :
	  case CAIRO_STATUS_READ_ERROR :
	    //std::cerr<<"can not load image \""<<name<<"\""<<std::endl;
	    cairo_surface_destroy(sp);
	    sp = NULL;
	  }
      }
#endif
    
#ifdef ANDROID
    if (sp == NULL)
      {
	sp = android_cairo_image_surface_create_from_png(name.c_str());

	switch (cairo_surface_status(sp))
	  {
	  case CAIRO_STATUS_NO_MEMORY :
	  case CAIRO_STATUS_FILE_NOT_FOUND :
	  case CAIRO_STATUS_READ_ERROR :
	    //std::cerr<<"can not load image \""<<name<<"\""<<std::endl;
	    cairo_surface_destroy(sp);
	    sp = NULL;
	  }
      }
#endif
    
    if (sp == NULL) return false;

    switch (cairo_surface_status(sp))
      {
      case CAIRO_STATUS_NO_MEMORY :
      case CAIRO_STATUS_FILE_NOT_FOUND :
      case CAIRO_STATUS_READ_ERROR :
	//std::cerr<<"can not load image \""<<name<<"\""<<std::endl;
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


#ifdef ANDROID
  static AAssetManager* am;
#endif

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
	  {
	    return NULL;
	  }
      }
  }


  static SpriteCollection sc;
};


#endif
