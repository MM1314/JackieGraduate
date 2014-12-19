#LOCAL_PATH := $(call my-dir)

#include $(CLEAR_VARS)

#include C:\Users\yinjingyu\Desktop\JackieMao\AndroidworkSpace\OpenCV-2.4.9-android-sdk\OpenCV-2.4.9-android-sdk\sdk\native\jni\OpenCV.mk

#LOCAL_MODULE    := GrayPic
#LOCAL_SRC_FILES := GrayPic.cpp

#生成动态库
#include $(BUILD_SHARED_LIBRARY)


LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

OpenCV_INSTALL_MODULES:=on
OPENCV_CAMERA_MODULES:=off

#通过静态链接的方式连接OpenCV的so
OPENCV_LIB_TYPE:=STATIC

ifeq ("$(wildcard $(OPENCV_MK_PATH))","")
include  C:\Users\yinjingyu\Desktop\JackieMao\AndroidworkSpace\OpenCV-2.4.9-android-sdk\OpenCV-2.4.9-android-sdk\sdk\native\jni\OpenCV.mk
else
include $(OPENCV_MK_PATH)
endif

LOCAL_MODULE    := GrayPic
LOCAL_SRC_FILES := GrayPic.cpp \
					HandReco/SkinTester.cpp \
					HandReco/FingerTest.cpp

LOCAL_LIBS += -lm -llog

include $(BUILD_SHARED_LIBRARY)