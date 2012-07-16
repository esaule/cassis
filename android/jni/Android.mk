LOCAL_PATH := $(call my-dir)

include jni/png.mk
include jni/pixman.mk
include jni/cairo.mk

# include $(CLEAR_VARS)
# LOCAL_MODULE := cairo
# LOCAL_SRC_FILES := include/libcairo.so
# include $(PREBUILT_SHARED_LIBRARY)

# include $(CLEAR_VARS)
# LOCAL_MODULE := pixman
# LOCAL_SRC_FILES := include/libpixman.so
# include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

#CAIRO_ANDROID=$(HOME)/dev/android-cairo/
CAIRO_ANDROID=./

CAIRO_COMPILE=-I$(CAIRO_ANDROID)/jni/pixman/pixman -I$(CAIRO_ANDROID)/jni/cairo/src -I$(CAIRO_ANDROID)/jni/cairo-extra -I$(CAIRO_ANDROID)/jni/pixman-extra -Wno-missing-field-initializers -D CAIRO_HAS_PNG_FUNCTIONS=1

CAIRO_LINK=-L$(CAIRO_ANDROID)/libs/$(TARGET_ARCH_ABI) -lpixman -lcairo

VERSION_NUMBER=\"`git describe --tags`-`git log --pretty=format:'%h' -n 1``if git diff --exit-code > /dev/null ; then echo -n ; else echo \-dirty; fi`\"

LOCAL_MODULE    := cassis
LOCAL_CFLAGS    := -O2 -I. $(CAIRO_COMPILE) -DANDROID=1  -DVERSION_NUMBER=$(VERSION_NUMBER)
LOCAL_CPPFLAGS    := -O2 --std=gnu++0x -I. $(CAIRO_COMPILE) -fexceptions -DNOSTRING -DUSE_MYMAP  -DVERSION_NUMBER=$(VERSION_NUMBER)
LOCAL_LDLIBS    := -lm -llog -landroid -lz
LOCAL_SRC_FILES := cassis.cpp IA.cpp GameState.cpp SpriteCollection_cairo.cpp
LOCAL_STATIC_LIBRARIES := android_native_app_glue # libcairo libpixman
LOCAL_SHARED_LIBRARIES := cairo pixman png

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
