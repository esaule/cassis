#include <stdlib.h>
#include "SpriteCollection_cairo.hpp"

#ifdef ANDROID

#include <android/asset_manager.h>

AAssetManager* SpriteCollection::am = NULL;


cairo_status_t android_png_cairo_wrapper_read (void *closure,
					       unsigned char *data,
					       unsigned int length)
{
  AAsset* as = (AAsset*) closure;

  while (length > 0)
    {
      int res = AAsset_read(as, data, length);
      if (res <= 0)
	return CAIRO_STATUS_READ_ERROR;
    
      length -= res;
    }
  
  return CAIRO_STATUS_SUCCESS;
}
#endif

SpriteCollection SpriteCollection::sc("../assets/");


