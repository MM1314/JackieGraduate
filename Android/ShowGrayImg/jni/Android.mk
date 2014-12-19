LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := skinTest
LOCAL_SRC_FILES := skinTest.cpp

include $(BUILD_SHARED_LIBRARY)
