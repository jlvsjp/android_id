LOCAL_PATH := $(call my-dir)

include  $(CLEAR_VARS)
LOCAL_MODULE    := wltdecode
LOCAL_SRC_FILES := libwltdecode.so
include $(PREBUILT_SHARED_LIBRARY)

