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
 * Method:    initJNIComputation
 * Signature: (Ljava/lang/String;Ljava/lang/String;II[B[BLjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_sf_prenzl_stdrules_StdRules_initJNIComputation
  (JNIEnv *, jobject, jstring, jstring, jint, jint, jbyteArray, jbyteArray, jstring);

/*
 * Class:     net_sf_prenzl_stdrules_StdRules
 * Method:    listJNIRules
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_net_sf_prenzl_stdrules_StdRules_listJNIRules
  (JNIEnv *, jobject);

/*
 * Class:     net_sf_prenzl_stdrules_StdRules
 * Method:    listJNITopologies
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_net_sf_prenzl_stdrules_StdRules_listJNITopologies
  (JNIEnv *, jobject);

/*
 * Class:     net_sf_prenzl_stdrules_StdRules
 * Method:    describeJNIRuleProperties
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_sf_prenzl_stdrules_StdRules_describeJNIRuleProperties
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif
