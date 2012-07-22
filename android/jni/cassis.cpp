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

#include <android_native_app_glue.h>

#include <stdbool.h>
#include <errno.h>
#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cairo/src/cairo.h"

#include "cassis_cairo.hpp"
#include "cairo_menu_selector.hpp"

#define  LOG_TAG    "cassis"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

bool app_has_focus = false;
int  tick          = 0;

static void draw_frame(ANativeWindow_Buffer *buffer, CairoGraphicController* cd);
static void handle_app_command(struct android_app* app, int32_t cmd);
static int32_t handle_input(struct android_app* app, AInputEvent* event);



static bool pressed;

CairoGraphicController* cd;


void android_main(struct android_app* app) {
    // Make sure glue isn't stripped.
    app_dummy();

    pressed = false;
    cd = NULL;

    app->userData = NULL;
    app->onAppCmd = handle_app_command;
    app->onInputEvent = handle_input;

    //LOGI("asset: %p", app->activity->assetManager);
    SpriteCollection::am = app->activity->assetManager;


    //CassisDisplay cd2(0);
    CairoMenuSelector cms;
    cd = &cms;

    if (app->savedState != NULL) {
      // We are starting with a previous saved state; restore from it.
      try
	{
	  cms.deserialize((char*)app->savedState);
	}
      catch (DeserializeException de)
	{
	  LOGE("Can not deserialize state! Quitting");
	  return;
	}
    }

    while (1) {
        // Read all pending events. If app_has_focus is true, then we are going 
        // to read any events that are ready then render the screen. If we don't
        // have focus, we are going to block and spin around the poll loop until
        // we get focus again, preventing us from doing a bunch of rendering 
        // when the app isn't even visible.
        int ident;
        int events;
        struct android_poll_source* source;

        while ((ident=ALooper_pollAll(app_has_focus ? 0 : -1, NULL, &events, (void**)&source)) >= 0) {
            // Process this event.
            if (source != NULL) {
                source->process(app, source);
            }

            // Check if we are exiting.
            if (app->destroyRequested != 0) {
                LOGI("Engine thread destroy requested!");
                return;
            }
        }


        /* Now that we've delt with input, draw stuff */
        if (app->window != NULL) {
            ++tick;
	    //            LOGI("Rendering frame %d", tick);
            ANativeWindow_Buffer buffer;
            if (ANativeWindow_lock(app->window, &buffer, NULL) < 0) {
                LOGW("Unable to lock window buffer");
                continue;
            }

            draw_frame(&buffer, cd);

            ANativeWindow_unlockAndPost(app->window);
        }
    }
}


static void handle_app_command(struct android_app* app, int32_t cmd) {
  /* app->userData is available here */
  
  char * buf = NULL;

  switch (cmd) {
  case APP_CMD_SAVE_STATE:
    //To save the state: write the size of the save in
    //app->savedStateSize and point to a buffer of that size with
    //app->savedState. Saving the state is important because all
    //change of application cause a save. Changing orientation causes
    //a save as well.
    app->savedStateSize = cd->serializesize();
    buf = (char*) malloc(app->savedStateSize);
    cd->serialize((char*)buf);
    app->savedState = (void*)buf;
    break;
  case APP_CMD_INIT_WINDOW:
    app_has_focus=true;
    break;
  case APP_CMD_LOST_FOCUS:
    app_has_focus=false;
    break;
  case APP_CMD_GAINED_FOCUS:
    app_has_focus=true;
    break;
  }
}


static int32_t handle_input(struct android_app* app, AInputEvent* event) {
    /* app->userData is available here */

    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        app_has_focus = true;

	LOGI("Motion Event, pressed = %d", (pressed?1:0));
	
	for (int i=0; i< AMotionEvent_getPointerCount(event); ++i)
	  {
	    int action = AMotionEvent_getAction(event);
	    LOGI("Motion Event, action = %d", action);
	    if ((action & AMOTION_EVENT_ACTION_DOWN)
		||(action & AMOTION_EVENT_ACTION_POINTER_DOWN))
	      {
		LOGI("In");
		if (!pressed)
		  {
		    pressed = true;
		    float px = AMotionEvent_getX(event, i);
		    float py = AMotionEvent_getY(event, i);
		    cd->clickat(px,py);
		  }
	      }
	    else
	      pressed = false;

	  }
        return 1;
    } else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
        LOGI("Key event: action=%d keyCode=%d metaState=0x%x",
                AKeyEvent_getAction(event),
                AKeyEvent_getKeyCode(event),
                AKeyEvent_getMetaState(event));
    }

    return 0;
}



static void draw_frame(ANativeWindow_Buffer *buffer, CairoGraphicController* cd)
{

    int pixel_size = 0;
    cairo_surface_t *surface = NULL;

    /* Setup our cairo surface to render directly to the native window buffer */
    if (buffer->format == WINDOW_FORMAT_RGB_565) {
        pixel_size = 2;
        surface = cairo_image_surface_create_for_data((unsigned char*)buffer->bits, CAIRO_FORMAT_RGB16_565,
						      buffer->width, buffer->height, buffer->stride*pixel_size);
    } else if (buffer->format == WINDOW_FORMAT_RGBA_8888 || buffer->format == WINDOW_FORMAT_RGBX_8888) {
        pixel_size = 4;
        surface = cairo_image_surface_create_for_data((unsigned char*)buffer->bits, CAIRO_FORMAT_RGB24,
						      buffer->width, buffer->height, buffer->stride*pixel_size);
    } else {
        LOGE("Unsupported buffer format: %d", buffer->format);
        return;
    }

    cairo_t         *cr      = cairo_create(surface);

    /* clear the screen */
    memset(buffer->bits, 0, buffer->stride*pixel_size*buffer->height);

    /* Normalize our canvas size to make our lives easier */
    //    cairo_scale(cr, buffer->width, buffer->height);


    cd->setSizeX (buffer->width);
    cd->setSizeY (buffer->height);

    cd->render(cr);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}
