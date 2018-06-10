#ifndef AGRAM_TYPES_H
#define AGRAM_TYPES_H

#if AGRAM_ANDROID
#include <jni.h>
typedef jint agram_codepoint;
typedef jchar agram_display_char;
typedef jint agram_size;
#else
#include <cstddef>
typedef wchar_t agram_codepoint;
typedef char agram_display_char;
typedef size_t agram_size;
#endif

#endif
