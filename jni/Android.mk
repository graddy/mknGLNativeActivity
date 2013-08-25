# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

#libpng‚ÌƒŠƒ“ƒN
include $(CLEAR_VARS)
LOCAL_MODULE    := libpng-prebuilt
LOCAL_SRC_FILES := lib/libpng.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := mknGlnative-activity

LOCAL_SRC_FILES := png_loader.cpp
LOCAL_SRC_FILES += main.cpp
LOCAL_SRC_FILES += testcodes.cpp
LOCAL_SRC_FILES += clsCharactors.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv1_CM -lz
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_STATIC_LIBRARIES += libpng-prebuilt
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
