LOCAL_PATH := $(call my-dir)

include jni/pixman.mk
include jni/cairo.mk
include $(CLEAR_VARS)

LOCAL_MODULE    := cassis
LOCAL_CFLAGS    := -O2 -I. -Ijni/pixman/pixman -Ijni/cairo/src -Ijni/cairo-extra -Ijni/pixman-extra -Wno-missing-field-initializers
LOCAL_CPPFLAGS    := -O2 --std=gnu++0x -I. -Ijni/pixman/pixman -Ijni/cairo/src -Ijni/cairo-extra -Ijni/pixman-extra -Wno-missing-field-initializers -fexceptions
LOCAL_LDLIBS    := -lm -llog -landroid
LOCAL_SRC_FILES := cassis.cpp IA.cpp GameState.cpp
LOCAL_STATIC_LIBRARIES := android_native_app_glue libcairo libpixman

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
