#include <string.h>

#include "JNI/prenzl/h/Prenzl.h"

#include "Rules/stdrules/PhaseCheulou.h"
#include "Rules/stdrules/OlNotBin.h"
#include "Rules/stdrules/NicolasDeStael.h"
#include "Rules/stdrules/Lsd.h"
#include "Rules/stdrules/GameOfLife.h"
#include "Rules/stdrules/ChannelPropagation.h"
#include "Rules/stdrules/Blur.h"

#include "net_sf_prenzl_stdrules_StdRules.h"

using namespace Prenzl;

const char* RULE_PHASE_CHELOU = "PhaseCheulou";
const char* RULE_OL_NOT_BIN = "OlNotBin";
const char* RULE_NICOLAS_DE_STAEL = "NicolasDeStael";
const char* RULE_LSD = "LSD";
const char* RULE_GAME_OF_LIFE = "GameOfLife";
const char* RULE_CHANNEL_PROPAGATION = "ChannelPropagation";
const char* RULE_BLUR = "Blur";

JNIEXPORT jint JNICALL Java_net_sf_prenzl_stdrules_StdRules_init
  (JNIEnv * env, jobject, jstring ruleNameJ, jstring topologyNameJ, jint width, jint height, jbyteArray firstGenJ, jbyteArray previousGenJ, jstring propertiesJ){
	const char* ruleName = env->GetStringUTFChars(ruleNameJ, 0);
	//Create rule
	Rule* rule = NULL;
	if(strcmp(ruleName,RULE_PHASE_CHELOU)==0){
		rule = new PhaseCheulou();
	}
	else if(strcmp(ruleName,RULE_OL_NOT_BIN)==0){
		rule = new OlNotBin();
	}
	else if(strcmp(ruleName,RULE_NICOLAS_DE_STAEL)==0){
		rule = new NicolasDeStael();
	}
	else if(strcmp(ruleName,RULE_LSD)==0){
		rule = new Lsd();
	}
	else if(strcmp(ruleName,RULE_GAME_OF_LIFE)==0){
		rule = new GameOfLife();
	}
	else if(strcmp(ruleName,RULE_CHANNEL_PROPAGATION)==0){
		rule = new ChannelPropagation();
	}
	else if(strcmp(ruleName,RULE_BLUR)==0){
		rule = new Blur();
	}
	
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
    int i;

	const int size = 7;
    const char* message[size] = {
    	RULE_PHASE_CHELOU,
    	RULE_OL_NOT_BIN,
    	RULE_NICOLAS_DE_STAEL,
    	RULE_LSD,
    	RULE_GAME_OF_LIFE,
    	RULE_CHANNEL_PROPAGATION,
    	RULE_BLUR
    };

    ret= (jobjectArray)env->NewObjectArray(
    	size,
         env->FindClass("java/lang/String"),
         env->NewStringUTF(""));

    for(i=0;i<size;i++) {
        env->SetObjectArrayElement(
		ret,i,env->NewStringUTF(message[i]));
    }
    return(ret);
  }

  JNIEXPORT jobjectArray JNICALL Java_net_sf_prenzl_stdrules_StdRules_listTopologies
  (JNIEnv *, jobject){
  	return NULL;
  }
