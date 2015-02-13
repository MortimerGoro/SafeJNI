MY_LOCAL_PATH := $(call my-dir)

LOCAL_PATH := $(MY_LOCAL_PATH)/../../dist
include $(CLEAR_VARS)
LOCAL_MODULE := safejni
LOCAL_SRC_FILES := \
	$(TARGET_ARCH_ABI)/libsafejni.so
include $(PREBUILT_SHARED_LIBRARY)

LOCAL_PATH := $(MY_LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_LDLIBS := \
	-llog \
	-lc
LOCAL_SHARED_LIBRARIES := \
	safejni
LOCAL_C_INCLUDES := \
	$(MY_LOCAL_PATH)/../../dist
LOCAL_SRC_FILES := \
	main.cpp
LOCAL_CPPFLAGS := \
	-frtti \
	-fexceptions \
	-std=c++11 \
	-D__GXX_EXPERIMENTAL_CXX0X__
LOCAL_MODULE := safejnitest
include $(BUILD_SHARED_LIBRARY)
