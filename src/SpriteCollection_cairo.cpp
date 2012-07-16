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


