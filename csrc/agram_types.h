#ifndef AGRAM_TYPES_H
#define AGRAM_TYPES_H

#if AGRAM_ANDROID
  #define AGRAM_MMAP 1
  #include <jni.h>
  typedef jint agram_cpt;
  typedef jchar agram_dchar;
  typedef jint agram_size;
#else
  #include <cstddef>
  typedef wchar_t agram_cpt;
  typedef char agram_dchar;
  typedef size_t agram_size;
#endif

#endif
