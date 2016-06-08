LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := anagram
LOCAL_SRC_FILES := ../csrc/agram_wc.c ../csrc/anagram.c ../csrc/anagrams.c astr.c ../csrc/is_anagram.c ../csrc/is_within.c jni.c ../csrc/lcwc.c ../csrc/lettercounts.c ../csrc/vector.c ../csrc/wc.c wl_upgrade.c ../csrc/words_from.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../csrc/
LOCAL_CFLAGS := -DPLATFORM='"$(TARGET_ARCH_ABI)"' -DAGRAM_ANDROID=1 -std=iso9899:1999 -Wall -Wextra -Wno-unused-parameter

include $(BUILD_SHARED_LIBRARY)
