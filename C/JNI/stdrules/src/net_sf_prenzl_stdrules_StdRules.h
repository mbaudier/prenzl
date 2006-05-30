/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class net_sf_prenzl_stdrules_StdRules */

#ifndef _Included_net_sf_prenzl_stdrules_StdRules
#define _Included_net_sf_prenzl_stdrules_StdRules
#ifdef __cplusplus
extern "C" {
#endif
#undef net_sf_prenzl_stdrules_StdRules_TYPE_JNI
#define net_sf_prenzl_stdrules_StdRules_TYPE_JNI 1L
#undef net_sf_prenzl_stdrules_StdRules_TYPE_JAVA
#define net_sf_prenzl_stdrules_StdRules_TYPE_JAVA 2L
/*
 * Class:     net_sf_prenzl_stdrules_StdRules
 * Method:    init
 * Signature: (Ljava/lang/String;Ljava/lang/String;II[B[BLjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_sf_prenzl_stdrules_StdRules_init
  (JNIEnv *, jobject, jstring, jstring, jint, jint, jbyteArray, jbyteArray, jstring);

/*
 * Class:     net_sf_prenzl_stdrules_StdRules
 * Method:    listComputations
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_net_sf_prenzl_stdrules_StdRules_listComputations
  (JNIEnv *, jobject);

/*
 * Class:     net_sf_prenzl_stdrules_StdRules
 * Method:    listTopologies
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_net_sf_prenzl_stdrules_StdRules_listTopologies
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
