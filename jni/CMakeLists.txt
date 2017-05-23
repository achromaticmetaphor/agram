cmake_minimum_required(VERSION 3.1)

add_library(anagram SHARED ../csrc/agram_wc ../csrc/anagrams ../csrc/lcwc ../csrc/lettercounts ../csrc/wc astr jni wl_upgrade)
include_directories(../csrc)
include_directories(.)
target_compile_definitions(anagram PUBLIC AGRAM_ANDROID=1 PLATFORM="${ANDROID_ABI}")

set_property(TARGET anagram PROPERTY C_STANDARD 99)
set_property(TARGET anagram PROPERTY C_STANDARD_REQUIRED ON)
set_property(TARGET anagram PROPERTY CXX_STANDARD 14)
set_property(TARGET anagram PROPERTY CXX_STANDARD_REQUIRED ON)
