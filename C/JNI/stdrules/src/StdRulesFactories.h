#pragma once
#include <string>
#include <map>
#include <string>

#include "Model/CProperties.h"
#include "Rules/stdrules/PhaseCheulou.h"
#include "Rules/stdrules/OlNotBin.h"
#include "Rules/stdrules/NicolasDeStael.h"
#include "Rules/stdrules/Lsd.h"
#include "Rules/stdrules/GameOfLife.h"
#include "Rules/stdrules/ChannelPropagation.h"
#include "Rules/stdrules/Blur.h"
#include "Rules/stdrules/Turner.h"
#include "Rules/stdrules/Hydra.h"
#include "Rules/stdrules/ColorHydra.h"

namespace Prenzl {

	class RFactory {
	public:	
		virtual std::string getName() = 0;
		virtual Rule * createRule(const CProperties& prop) = 0;
		virtual CProperties getDefaultProperties() {
			return CProperties();
		}
	};	
	
	class PhaseCheulouFact : public RFactory {
	public:
		std::string getName() { return "Phase Cheulou";}
		Rule * createRule(const CProperties& prop) {
			return new PhaseCheulou(
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
		CProperties getDefaultProperties() {
			return CProperties(
				"pc.R.R=0\npc.R.G=0\npc.R.B=-90\npc.G.R=90\npc.G.G=0\npc.G.B=0\npc.B.R=0\npc.B.G=90\npc.B.B=0"
			);
		}		
	};
	
	class OlNotBinFact : public RFactory {
	public:
		std::string getName() { return "OlNotBin";}
		Rule * createRule(const CProperties& prop) {
			return new OlNotBin();
		}		
	};
	
	class NicolasDeStaelFact : public RFactory {
	public:
		std::string getName() { return "Nicolas De Stael";}
		Rule * createRule(const CProperties& prop) {
			return new NicolasDeStael(
				prop.getValueAsInt("nds.volatility"),
				prop.getValueAsInt("nds.sign_bit"),
				prop.getValueAsInt("nds.dir_bit")
			);
		}		
		CProperties getDefaultProperties() {
			return CProperties( "nds.volatility=2\nnds.sign_bit=6\nnds.dir_bit=3\n" );
		}		
	};	

	class LsdFact : public RFactory {
	public:
		std::string getName() { return "LSD";}
		Rule * createRule(const CProperties& prop) {
			return new Lsd(
				prop.getValueAsInt("lsd.B.delta"),
				prop.getValueAsInt("lsd.G.delta"),
				prop.getValueAsInt("lsd.R.delta"),
				prop.getValueAsInt("lsd.B.dx"),
				prop.getValueAsInt("lsd.B.dy"),
				prop.getValueAsInt("lsd.G.dx"),
				prop.getValueAsInt("lsd.G.dy"),
				prop.getValueAsInt("lsd.R.dx"),
				prop.getValueAsInt("lsd.R.dy")
			);
		}	
		CProperties getDefaultProperties() {		
			return CProperties(
				"lsd.R.dx=1\nlsd.R.dy=-1\nlsd.R.delta=1\n"
				"lsd.G.dx=1\nlsd.G.dy=1\nlsd.G.delta=1\n"
				"lsd.B.dx=-1\nlsd.B.dy=0\nlsd.B.delta=2\n"
			);
		}			
	};	
	
	class GameOfLifeFact : public RFactory {
	public:
		std::string getName() { return "Game Of Life";}
		Rule * createRule(const CProperties& prop) {
			return new GameOfLife();
		}		
	};	
	
	class ChannelPropagationFact : public RFactory {
	public:
		std::string getName() { return "Channel Propagation";}
		Rule * createRule(const CProperties& prop) {
			return new ChannelPropagation(
				prop.getValueAsInt("cp.B"),
				prop.getValueAsInt("cp.G"),
				prop.getValueAsInt("cp.R"),
				prop.getValueAsInt("cp.colorInteraction")
			);
		}
		CProperties getDefaultProperties() {		
			return CProperties("cp.B=30\ncp.G=30\ncp.R=30\ncp.colorInteraction=4\n");
		}				
	};	
	
	class BlurFact : public RFactory {
	public:
		std::string getName() { return "Blur";}
		Rule * createRule(const CProperties& prop) {
			return new Blur(prop.getValueAsInt("blur.disturbanceShift"));
		}	
		CProperties getDefaultProperties() {		
			return CProperties("blur.disturbanceShift=12\n");
		}			
	};		
	
	class TurnerFact : public RFactory {
	public:
		std::string getName() { return "Turner";}
		Rule * createRule(const CProperties& prop) {
			return new Turner(
				prop.getValueAsInt("turner.volatility"),
				prop.getValueAsInt("turner.sign_bit"),
				prop.getValueAsInt("turner.dir_bit")
			);
		}		
		CProperties getDefaultProperties() {
			return CProperties( "turner.volatility=2\nturner.sign_bit=5\nturner.dir_bit=6\n" );
		}			
	};
		
	class HydraFact : public RFactory {
	public:
		std::string getName() { return "Hydra";}
		Rule * createRule(const CProperties& prop) {
			return new Hydra(
				prop.getValueAsInt("hydra.bodyLowerLimit"),
				prop.getValueAsInt("hydra.headLowerLimit")
			);
		}	
		CProperties getDefaultProperties() {
			return CProperties( "hydra.bodyLowerLimit=100\nhydra.headLowerLimit=200\n" );
		}			
	};	
	
	class ColorHydraFact : public RFactory {
	public:
		std::string getName() { return "Color Hydra";}
		Rule * createRule(const CProperties& prop) {
			return new ColorHydra(
				prop.getValueAsInt("chydra.bodyLowerLimit"),
				prop.getValueAsInt("chydra.headLowerLimit")
			);
		}		
		CProperties getDefaultProperties() {
			return CProperties( "chydra.bodyLowerLimit=300\nchydra.headLowerLimit=550\n" );
		}			
	};		
	
	void registerFactory(RFactory * factory, std::map<std::string, RFactory *>& factories) {
		factories[factory->getName()] = factory;
	}
	
	std::map<std::string, RFactory *> createFactories() {
		std::cout << "Initialise Factories" << std::endl;
		std::map<std::string, RFactory *> factories;
		registerFactory( new PhaseCheulouFact(), factories);
		registerFactory( new OlNotBinFact(), factories);
		registerFactory( new NicolasDeStaelFact(), factories);
		registerFactory( new LsdFact(), factories);
		registerFactory( new GameOfLifeFact(), factories);
		registerFactory( new ChannelPropagationFact(), factories);
		registerFactory( new BlurFact(), factories);
		registerFactory( new TurnerFact(), factories);
		registerFactory( new HydraFact(), factories);
		registerFactory( new ColorHydraFact(), factories);
		std::cout << factories.size() << " Factories created" << std::endl;
		return factories;
	}	
}
