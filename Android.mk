LOCAL_PATH:= $(call my-dir)

MINIAF_32 := $(shell cat frameworks/av/media/mediaserver/Android.mk |grep "LOCAL_32_BIT_ONLY[[:space:]]*:=[[:space:]]*" |grep -o "true\|1\|false\|0")

ANDROID_MAJOR :=
ANDROID_MINOR :=
ANDROID_MICRO :=

ifndef ANDROID_MAJOR
include build/core/version_defaults.mk
ifeq ($(strip $(PLATFORM_VERSION)),)
$error(*** Cannot get Android platform version)
endif
ANDROID_MAJOR = $(shell echo $(PLATFORM_VERSION) | cut -d . -f 1)
ANDROID_MINOR = $(shell echo $(PLATFORM_VERSION) | cut -d . -f 2)
ANDROID_MICRO = $(shell echo $(PLATFORM_VERSION) | cut -d . -f 3)
endif

ifeq ($(strip $(ANDROID_MAJOR)),)
$(error *** ANDROID_MAJOR undefined)
endif

ifeq ($(strip $(ANDROID_MINOR)),)
$(error *** ANDROID_MINOR undefined)
endif

ifeq ($(strip $(ANDROID_MICRO)),)
$(warning *** ANDROID_MICRO undefined. Assuming 0)
ANDROID_MICRO = 0
endif

include $(CLEAR_VARS)
LOCAL_SRC_FILES := audioflingerglue.cpp IPrivateAfGlue.cpp IPrivateAfGlueHandler.cpp

ifeq ($(strip $(ANDROID_MAJOR)), 4)
LOCAL_SHARED_LIBRARIES := libc \
                          libdl \
                          libutils \
                          libcutils \
                          libbinder \
                          libmedia
else
LOCAL_SHARED_LIBRARIES := libc \
                          libdl \
                          libutils \
                          libcutils \
                          libbinder \
                          libmedia \
                          libserviceutility
endif

ifeq ($(ANDROID_MAJOR),$(filter $(ANDROID_MAJOR),8 9))
LOCAL_SHARED_LIBRARIES +=  liblog
endif

LOCAL_CPPFLAGS :=-DANDROID_MAJOR=$(ANDROID_MAJOR) -DANDROID_MINOR=$(ANDROID_MINOR) -DANDROID_MICRO=$(ANDROID_MICRO) -Wno-unused-parameter
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libaudioflingerglue
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := IPrivateAfGlue.cpp PrivateAfGlue.cpp IPrivateAfGlueHandler.cpp miniaf.cpp
LOCAL_SHARED_LIBRARIES := libutils \
                          libbinder \
                          libcutils \
                          libaudioutils \
                          libmedia \
                          libhardware

ifeq ($(ANDROID_MAJOR),$(filter $(ANDROID_MAJOR),8 9))
LOCAL_SHARED_LIBRARIES += libaudioclient \
                          liblog
endif

LOCAL_MODULE_TAGS := optional
LOCAL_CPPFLAGS := -DANDROID_MAJOR=$(ANDROID_MAJOR) -DANDROID_MINOR=$(ANDROID_MINOR) -DANDROID_MICRO=$(ANDROID_MICRO) -Wno-unused-parameter
ifneq ($(CM_BUILD),)
LOCAL_CPPFLAGS += -DCM_BUILD
endif
ifneq ($(shell cat frameworks/av/include/media/AudioSystem.h |grep SetACFPreviewParameter),)
LOCAL_CPPFLAGS += -DUSE_SERVICES_VENDOR_EXTENSION
endif
LOCAL_MODULE := miniafservice
ifeq ($(strip $(MINIAF_32)), true)
LOCAL_32_BIT_ONLY := true
endif
include $(BUILD_EXECUTABLE)
