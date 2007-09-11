#include <iostream>

#include "Model/Topology.h"
#include "Model/Rule.h"

#include "../h/Prenzl.h"
#include "Registry.h"

#include "net_sf_prenzl_adapter_JNIComputation.h"

using namespace Prenzl;

Registry* registry;

JNIEXPORT int registerRuleAndTopology(Rule* rule, Topology* topology) {
	return registry->registerRuleAndTopology(rule, topology);
}

int getId(JNIEnv * env, jobject computationJ) {
	jclass cls = env->GetObjectClass(computationJ);
	if (cls == 0) {
		cerr <<"# ERROR # Class not found\n";
		return -1;
	}
	jmethodID mid = env->GetMethodID(cls, "getId", "()I");
	if (mid == 0) {
		cerr <<"# ERROR # Method getId not found\n";
		return -1;
	}

	int id = env->CallIntMethod(computationJ, mid);
	return id;
}

JNIEXPORT void JNICALL Java_net_sf_prenzl_adapter_JNIComputation_compute(JNIEnv * env,
		jobject computationJ) {
	int id = getId(env, computationJ);
	if (id>=0) {
		Topology* topo = registry->getTopology(id);
		topo->nextGeneration();
		Rule* ruleL = registry->getRule(id);
		ruleL->computeNext(*topo);
	}
}

JNIEXPORT void JNICALL Java_net_sf_prenzl_adapter_JNIComputation_getCurrent(JNIEnv * env,
		jobject computationJ, jbyteArray bufJ) {
	int id = getId(env, computationJ);
	if (id>=0) {
		Topology* topo = registry->getTopology(id);
		env->ReleaseByteArrayElements(bufJ, (jbyte*)topo->getCurrent(),
				JNI_COMMIT);
	}
}

JNIEXPORT void JNICALL Java_net_sf_prenzl_adapter_JNIComputation_clean(JNIEnv * env,
		jobject computationJ) {
	int id = getId(env, computationJ);
	if (id>=0) {
		registry->remove(id);
	}
}

JNIEXPORT void JNICALL Java_net_sf_prenzl_adapter_JNIComputation_initRegistry(JNIEnv *, jclass) {
	registry = new Registry();
}

JNIEXPORT void JNICALL Java_net_sf_prenzl_adapter_JNIComputation_cleanRegistry(JNIEnv *, jclass) {
	delete registry;
}
