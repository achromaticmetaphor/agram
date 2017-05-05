cmake_minimum_required(VERSION 3.1)

add_executable(agram agram xdgwl ../csrc/agram_wc ../csrc/anagram ../csrc/anagrams ../csrc/is_anagram ../csrc/is_within ../csrc/lcwc ../csrc/lettercounts ../csrc/vector ../csrc/wc ../csrc/words_from)
include_directories(../csrc)

set_property(TARGET agram PROPERTY C_STANDARD 99)
set_property(TARGET agram PROPERTY C_STANDARD_REQUIRED ON)
