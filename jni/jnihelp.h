#include <stddef.h>

#include <jni.h>

jobjectArray arr2jarr(JNIEnv *, const char * *, size_t);
const char * * jarr2arr(JNIEnv *, jobjectArray, size_t *);
void free_jarr(JNIEnv *, jobjectArray, const char * *);
