cmake_minimum_required(VERSION 3.1)

add_executable(agram agram xdgwl ../csrc/agram_wc ../csrc/anagrams ../csrc/lcwc ../csrc/lettercounts ../csrc/wc)
include_directories(../csrc)

set_property(TARGET agram PROPERTY C_STANDARD 99)
set_property(TARGET agram PROPERTY C_STANDARD_REQUIRED ON)
set_property(TARGET agram PROPERTY CXX_STANDARD 14)
set_property(TARGET agram PROPERTY CXX_STANDARD_REQUIRED ON)
