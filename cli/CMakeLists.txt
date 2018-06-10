cmake_minimum_required(VERSION 3.1)

add_executable(agram
  agram
  xdgwl
  ../csrc/anagrams
  ../csrc/anagram_target
  ../csrc/lettercounts
  ../csrc/wordlist_source
)
include_directories(../csrc)

set_property(TARGET agram PROPERTY CXX_STANDARD 14)
set_property(TARGET agram PROPERTY CXX_STANDARD_REQUIRED ON)
