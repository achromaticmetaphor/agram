#ifndef ASTR_H
#define ASTR_H

#include <stddef.h>

#include <jni.h>

size_t astrlen(jint const *);
void astrcpy(jint *, const jint *);
jint * astrdup(jint const *);

#endif
