LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := anagram
LOCAL_SRC_FILES := agram_wc.c anagram.c anagrams.c astr.c is_anagram.c is_within.c jni.c lcwc.c lettercounts.c vector.c wc.c wl_upgrade.c words_from.c
LOCAL_CFLAGS := -DPLATFORM='"$(TARGET_ARCH_ABI)"' -DAGRAM_ANDROID=1 -std=iso9899:1999 -Wall -Wextra -Wno-unused-parameter

include $(BUILD_SHARED_LIBRARY)
