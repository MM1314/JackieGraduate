LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#OPENCV_CAMERA_MODULES:=off
#OPENCV_INSTALL_MODULES:=off
#OPENCV_LIB_TYPE:=SHARED
include ../../sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES  := DetectionBasedTracker_jni.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS +=  -llog -ldl

LOCAL_MODULE     := detection_based_tracker

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
include ../../sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES  := com_example_myhand_HandUtils.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS +=  -llog -ldl

LOCAL_MODULE     := hand_utils

include $(BUILD_SHARED_LIBRARY)