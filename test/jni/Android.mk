MY_LOCAL_PATH := $(call my-dir)

LOCAL_PATH := $(MY_LOCAL_PATH)/../../bin
include $(CLEAR_VARS)
LOCAL_MODULE := safejni
LOCAL_SRC_FILES := \
	$(TARGET_ARCH_ABI)/libsafejni.a
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_PATH := $(MY_LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_LDLIBS := \
	-llog \
	-lc
LOCAL_STATIC_LIBRARIES := \
	safejni
LOCAL_C_INCLUDES := \
	$(MY_LOCAL_PATH)/../../bin
LOCAL_SRC_FILES := \
	main.cpp
LOCAL_CPPFLAGS := \
	-frtti \
	-fexceptions \
	-std=c++11 \
	-D__GXX_EXPERIMENTAL_CXX0X__
LOCAL_MODULE := safejnitest
include $(BUILD_SHARED_LIBRARY)
