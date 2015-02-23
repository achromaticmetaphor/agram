#ifndef ASTR_H
#define ASTR_H

#include <stddef.h>

#include <jni.h>

size_t astrlen(jint const *);
void uastrcpy(jint *, const jchar *, jsize);
jchar * ustrdup(jchar const *, jsize);

#endif
