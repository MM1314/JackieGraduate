LOCAL_PATH := $(call my-dir)
APP_STL := gnustl_static

#Android.mk中可以定义多个编译模块，每个编译模块都是以include $(CLEAR_VARS)开始

include $(CLEAR_VARS)
include /home/jackie/WorkOpenAnd/OpenCV-2.4.7.1-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES  := Skin.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -llog -ldl

LOCAL_MODULE     := skin_pic

include $(BUILD_SHARED_LIBRARY)

#SkinPicHandle

include $(CLEAR_VARS)
include ../../sdk/native/jni/OpenCV.mk

LOCAL_MODULE     := skin_pic_handle
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_SRC_FILES  := SkinPicHandle.cpp
LOCAL_LDLIBS     += -llog -ldl

include $(BUILD_SHARED_LIBRARY)

#LKTracker

include $(CLEAR_VARS)
include ../../sdk/native/jni/OpenCV.mk

LOCAL_MODULE     := lk_tracker
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_SRC_FILES  := LKTracker.cpp

LOCAL_LDLIBS     += -llog -ldl

include $(BUILD_SHARED_LIBRARY)

#FerNNClassifier

include $(CLEAR_VARS)
include ../../sdk/native/jni/OpenCV.mk

LOCAL_MODULE     := fernn_classifier
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_SRC_FILES  := FerNNClassifier.cpp

LOCAL_LDLIBS     += -llog -ldl

include $(BUILD_SHARED_LIBRARY)

#tld_utils

include $(CLEAR_VARS)
include ../../sdk/native/jni/OpenCV.mk

LOCAL_MODULE     := tld_utils
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_SRC_FILES  := tld_utils.cpp

LOCAL_LDLIBS     += -llog -ldl

include $(BUILD_SHARED_LIBRARY)


#TLD
include $(CLEAR_VARS)
include ../../sdk/native/jni/OpenCV.mk

LOCAL_MODULE     := tld
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_SRC_FILES  := TLD.cpp

LOCAL_SHARED_LIBRARIES := \
						libfernn_classifier \
						liblk_tracker \
						libtld_utils
LOCAL_LDLIBS     += -llog -ldl

include $(BUILD_SHARED_LIBRARY)


#run_tld

include $(CLEAR_VARS)
include ../../sdk/native/jni/OpenCV.mk

LOCAL_MODULE     := run_tld
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_SRC_FILES  := run_tld.cpp

LOCAL_LDLIBS     += -llog -ldl

include $(BUILD_SHARED_LIBRARY)

#DetectionBasedTracker

include $(CLEAR_VARS)

include ../../sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES  := DetectionBasedTracker_jni.cpp

LOCAL_SHARED_LIBRARIES := \
					libskin_pic
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -llog -ldl

LOCAL_MODULE     := detection_based_tracker
include $(BUILD_SHARED_LIBRARY)