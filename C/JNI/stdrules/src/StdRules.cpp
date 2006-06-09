#include <string.h>

#include "JNI/prenzl/h/Prenzl.h"

#include "Model/CProperties.h"

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
	const char* propertiesStr = env->GetStringUTFChars(propertiesJ, 0);
	//Create rule
	Rule* rule = NULL;
	if(strcmp(ruleName,RULE_PHASE_CHELOU)==0){
		CProperties prop(propertiesStr);
		rule = new PhaseCheulou(
			prop.getValueAsInt("pc.B.G"),
			prop.getValueAsInt("pc.B.R"),
			prop.getValueAsInt("pc.G.R"),
			prop.getValueAsInt("pc.G.B"),
			prop.getValueAsInt("pc.R.B"),
			prop.getValueAsInt("pc.R.G"),
			prop.getValueAsInt("pc.B.B"),
			prop.getValueAsInt("pc.G.G"),
			prop.getValueAsInt("pc.R.R")
		);
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
	
	JNIEXPORT jstring JNICALL Java_net_sf_prenzl_stdrules_StdRules_describeRuleProperties
	  (JNIEnv * env, jobject, jstring ruleNameJ){
		const char* ruleName = env->GetStringUTFChars(ruleNameJ, 0);
		if(strcmp(ruleName,RULE_PHASE_CHELOU)==0){
			return env->NewStringUTF(
			"pc.R.R=0\npc.R.G=0\npc.R.B=-90\npc.G.R=90\npc.G.G=0\npc.G.B=0\npc.B.R=0\npc.B.G=90\npc.B.B=0");
		}
		else if(strcmp(ruleName,RULE_LSD)==0){
			return env->NewStringUTF("lsd.R.dx=90\nlsd.R.dy=0\nlsd.R.delta=20");
		}
		return env->NewStringUTF("");
	}

  JNIEXPORT jobjectArray JNICALL Java_net_sf_prenzl_stdrules_StdRules_listTopologies
  (JNIEnv *, jobject){
  	return NULL;
  }
