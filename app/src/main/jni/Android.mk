LOCAL_PATH := $(call my-dir)

####################################
# libnfc
####################################
include $(CLEAR_VARS)
LOCAL_MODULE    := androidNfc
LOCAL_SRC_FILES := libnfc/usbhost.c libnfc/libnfc.c libnfc/otg.c libnfc/serial.c

#LOCAL_LDLIBS	:= -llog
LOCAL_C_INCLUDES += $(LOCAL_PATH)/bionic/libc/kernel/common
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include

include $(BUILD_STATIC_LIBRARY)

#####################################
#	libnetreader
#####################################
include $(CLEAR_VARS)
LOCAL_MODULE    := androidNR
LOCAL_SRC_FILES := libnetcardreader/libnetcardreader.cpp libnetcardreader/netprotocol.cpp libnetcardreader/nfcprotocol.cpp libnetcardreader/readcard.cpp libnetcardreader/sm4.cpp libnetcardreader/verify_client.cpp

#LOCAL_LDLIBS	:= -llog
#LOCAL_CFLAGS += -I./include
#LOCAL_C_INCLUDES += bionic/libc/kernel/common

include $(BUILD_STATIC_LIBRARY)


#-------------------------------------------------------------------------#
#   Test CallBackLib
#-------------------------------------------------------------------------#
#####################################################
# build  SelfCallbackNfc.so for self nfc
#####################################################
include $(CLEAR_VARS)

LOCAL_MODULE    := SelfCallbackNfc
LOCAL_SRC_FILES := com_lz_nfc_jni_SelfCallbackNfc.cpp CallBackHandler.cpp javareadcard.cpp
#LOCAL_CFLAGS += -fpermissive


LOCAL_LDLIBS	:= -llog

LOCAL_C_INCLUDES += $(LOCAL_PATH)/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libnetcardreader
LOCAL_STATIC_LIBRARIES  := androidNR
LOCAL_STATIC_LIBRARIES  += androidNfc

include $(BUILD_SHARED_LIBRARY)


include $(LOCAL_PATH)/perbuild/Android.mk