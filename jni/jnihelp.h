#include <stddef.h>

#include <jni.h>

jobjectArray arr2jarr(JNIEnv *, const jint * *, size_t);
const jint * * jarr2arr(JNIEnv *, jobjectArray, size_t *);
void free_jarr(JNIEnv *, jobjectArray, const jint * *);
