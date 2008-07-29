#pragma once

#include <jni.h>

#include "Model/Topology.h"
#include "Model/Rule.h"

#include "Registry.h"

namespace Prenzl {

	class RegistryUtil {
	private:
		Registry* registry;

	public:
		RegistryUtil() {
			registry = new Registry();
		}

		~RegistryUtil() {
			delete registry;
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

		int registerRuleAndTopology(Rule* rule, Topology* topology) {
			return registry->registerRuleAndTopology(rule, topology);
		}

		void compute(JNIEnv * env, jobject computationJ) {
			int id = getId(env, computationJ);
			if (id>=0) {
				Topology* topo = registry->getTopology(id);
				topo->nextGeneration();
				Rule* ruleL = registry->getRule(id);
				ruleL->computeNext(*topo);
			}
		}

		void getCurrent(JNIEnv * env, jobject computationJ, jbyteArray bufJ) {
			int id = getId(env, computationJ);
			if (id>=0) {
				Topology* topo = registry->getTopology(id);
				env->ReleaseByteArrayElements(bufJ, (jbyte*)topo->getCurrent(),
						JNI_COMMIT);
			}
		}

		void cleanUp(JNIEnv * env, jobject computationJ) {
			int id = getId(env, computationJ);
			if (id>=0) {
				registry->remove(id);
			}
		}

	};

}
