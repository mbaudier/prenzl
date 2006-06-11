#include <string.h>
#include <map>

#include "JNI/prenzl/h/Prenzl.h"

#include "Model/CProperties.h"
#include "StdRulesFactories.h"

#include "net_sf_prenzl_stdrules_StdRules.h"

using namespace Prenzl;

static std::map<std::string, Prenzl::RFactory*> factories = createFactories();

JNIEXPORT jint JNICALL Java_net_sf_prenzl_stdrules_StdRules_init
  (JNIEnv * env, jobject, jstring ruleNameJ, jstring topologyNameJ, jint width, jint height, jbyteArray firstGenJ, jbyteArray previousGenJ, jstring propertiesJ){
	std::string ruleName = env->GetStringUTFChars(ruleNameJ, 0);
	std::string propertiesStr = env->GetStringUTFChars(propertiesJ, 0);
	
	//Create rule
	Rule* rule = factories[ruleName]->createRule(CProperties(propertiesStr));
	
	// Create topology
    jbyte *firstGen = env->GetByteArrayElements(firstGenJ, 0);
    jbyte *previousGen = env->GetByteArrayElements(previousGenJ, 0);
	Topology* topo = new Topology(width,height,(unsigned char*)firstGen,(unsigned char*)previousGen);
	jint id = registerRuleAndTopology(rule,topo);
	return id;
  }

JNIEXPORT jobjectArray JNICALL Java_net_sf_prenzl_stdrules_StdRules_listComputations
  (JNIEnv * env, jobject){
    jobjectArray ret;

    ret= (jobjectArray)env->NewObjectArray(
    	 factories.size(),
         env->FindClass("java/lang/String"),
         env->NewStringUTF(""));

	std::map<std::string, Prenzl::RFactory*>::const_iterator itr = factories.begin();
	for(int i = 0; itr != factories.end(); ++itr, ++i) {
        env->SetObjectArrayElement(ret,i,env->NewStringUTF(itr->first.c_str()));
	}
    
    return(ret);
  }
	
	JNIEXPORT jstring JNICALL Java_net_sf_prenzl_stdrules_StdRules_describeRuleProperties
	  (JNIEnv * env, jobject, jstring ruleNameJ){
		std::string ruleName = env->GetStringUTFChars(ruleNameJ, 0);
		return env->NewStringUTF(factories[ruleName]->getDefaultProperties().toString().c_str());
	}

  JNIEXPORT jobjectArray JNICALL Java_net_sf_prenzl_stdrules_StdRules_listTopologies
  (JNIEnv *, jobject){
  	return NULL;
  }
