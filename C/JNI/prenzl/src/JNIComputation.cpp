#include <stdio.h>
#include <iostream.h>
#include <string.h>

#include "Model/Topology.h"
#include "Model/Rule.h"

#include "../h/Prenzl.h"
#include "Registry.h"

#include "net_sf_prenzl_adapter_JNIComputation.h"

using namespace Prenzl;

Registry* registry;

JNIEXPORT int registerRuleAndTopology(Rule* rule, Topology* topology){
	return registry->registerRuleAndTopology(rule,topology);
}

int getId(JNIEnv * env, jobject computationJ){
    jclass cls = env->GetObjectClass(computationJ);
	if (cls == 0){
		cerr <<"# ERROR # Class not found\n";
		return -1;
	}
    jmethodID mid = env->GetMethodID(cls, "getId", "()I");
	if (mid == 0){
		cerr <<"# ERROR # Method getId not found\n";
		return -1;
	}
	
	int id = env->CallIntMethod(computationJ, mid);
	return id;
}
/*
JNIEXPORT jint JNICALL Java_net_sf_prenzl_adapter_JNIComputation_init
  (JNIEnv * env, jobject computationJ,jstring ruleNameJ, jint width, jint height, jbyteArray firstGenJ)
  {
	const char* ruleName = env->GetStringUTFChars(ruleNameJ, 0);
	//Create rule
	Rule* rule = ruleFactory->createRuleFromName(ruleName);
	// Create topology
	Topology* topology = new Topology(width,height);
	jint id = registry->registerRuleAndTopology(rule,topology);

	Topology* topo = registry->getTopology(id);
    jbyte *firstGen = env->GetByteArrayElements(firstGenJ, 0);
	for(int i= 0;i<width;i++){
		for(int j= 0;j<height;j++){
			topo->setCurrent(i,j,Topology::BLUE,firstGen[(i+j*width)*3]);
			topo->setCurrent(i,j,Topology::GREEN,firstGen[(i+j*width)*3+1]);
			topo->setCurrent(i,j,Topology::RED,firstGen[(i+j*width)*3+2]);
		}
	}
	return id;
  }
*/
JNIEXPORT void JNICALL Java_net_sf_prenzl_adapter_JNIComputation_compute
  (JNIEnv * env, jobject computationJ)
  {
	int id = getId(env,computationJ);
	if(id>=0){
	  	Topology* topo = registry->getTopology(id);
		topo->nextGeneration();
		Rule* ruleL = registry->getRule(id);
		ruleL->computeNext(*topo);  	
	}
  }

JNIEXPORT void JNICALL Java_net_sf_prenzl_adapter_JNIComputation_getCurrent
  (JNIEnv * env, jobject computationJ, jbyteArray bufJ)
  {
	int id = getId(env,computationJ);
	if(id>=0){
	  	Topology* topo = registry->getTopology(id);
	    env->ReleaseByteArrayElements(bufJ, (jbyte*)topo->getCurrent(), JNI_COMMIT);
	}
  }
  
  JNIEXPORT void JNICALL Java_net_sf_prenzl_adapter_JNIComputation_clean
  (JNIEnv * env, jobject computationJ){
	int id = getId(env,computationJ);
	if(id>=0){
	  	registry->remove(id);
	}
  }
  

JNIEXPORT void JNICALL Java_net_sf_prenzl_adapter_JNIComputation_initRegistry
  (JNIEnv *, jclass)
  {
  	registry = new Registry();
  }

JNIEXPORT void JNICALL Java_net_sf_prenzl_adapter_JNIComputation_cleanRegistry
  (JNIEnv *, jclass){
  	delete registry;
  }
/*
JNIEXPORT jobjectArray JNICALL Java_net_sf_prenzl_adapter_JNIComputation_listComputations
  (JNIEnv * env, jclass){
    jobjectArray ret;
    int i;

	//RuleFactory* ruleFactory = new RuleFactory();
	const int size = 2;
    const char* message[size] = {RULE_PHASE_CHELOU,RULE_OL_NOT_BIN};

    ret= (jobjectArray)env->NewObjectArray(
    	size,
         env->FindClass("java/lang/String"),
         env->NewStringUTF(""));

    for(i=0;i<size;i++) {
        env->SetObjectArrayElement(
		ret,i,env->NewStringUTF(message[i]));
    }
    
    //delete ruleFactory;
    return(ret);
  }
*/
