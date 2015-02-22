LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := anagram
LOCAL_SRC_FILES := agram_wc.c anagram.c anagrams.c astr.c is_anagram.c is_within.c jnihelp.c lcwc.c lettercounts.c wc.c word.c words_from.c

include $(BUILD_SHARED_LIBRARY)
