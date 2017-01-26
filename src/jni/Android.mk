LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	../safejni.cpp
LOCAL_CPPFLAGS := \
	-frtti \
	-fexceptions \
	-std=c++11 \
	-D__GXX_EXPERIMENTAL_CXX0X__
LOCAL_LDLIBS := -llog -latomic
LOCAL_MODULE := safejni
include $(BUILD_SHARED_LIBRARY)
