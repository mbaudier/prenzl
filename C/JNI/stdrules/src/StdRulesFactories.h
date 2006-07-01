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
#include "Rules/stdrules/Braque.h"
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
				prop.getValueAsInt("BG"),
				prop.getValueAsInt("BR"),
				prop.getValueAsInt("GR"),
				prop.getValueAsInt("GB"),
				prop.getValueAsInt("RB"),
				prop.getValueAsInt("RG"),
				prop.getValueAsInt("BB"),
				prop.getValueAsInt("GG"),
				prop.getValueAsInt("RR")
			);	
		}
		CProperties getDefaultProperties() {
			CProperties prop;
			prop.setIntValue("RR", 0);
			prop.setIntValue("RG", 0);
			prop.setIntValue("RB", -90);
			prop.setIntValue("GR", 90);
			prop.setIntValue("GG", 0);
			prop.setIntValue("GB", 0);
			prop.setIntValue("BR", 0);
			prop.setIntValue("BG", 90);
			prop.setIntValue("BB", 0);
			return prop;			
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
				prop.getValueAsInt("Volatility"),
				prop.getValueAsInt("SignBit"),
				prop.getValueAsInt("DirectionBit")
			);
		}		
		CProperties getDefaultProperties() {
			CProperties prop;
			prop.setIntValue("Volatility", 2);
			prop.setIntValue("SignBit", 6);
			prop.setIntValue("DirectionBit", 3);			
			return prop;
		}		
	};	

	class LsdFact : public RFactory {
	public:
		std::string getName() { return "LSD";}
		Rule * createRule(const CProperties& prop) {
			return new Lsd(
				prop.getValueAsInt("DeltaBlue"),
				prop.getValueAsInt("DeltaGreen"),
				prop.getValueAsInt("DeltaRed"),
				prop.getValueAsInt("DxBlue"),
				prop.getValueAsInt("DyBlue"),
				prop.getValueAsInt("DxGreen"),
				prop.getValueAsInt("DyGreen"),
				prop.getValueAsInt("DxRed"),
				prop.getValueAsInt("DyRed")
			);
		}	
		CProperties getDefaultProperties() {	
			CProperties prop;
			prop.setIntValue("DeltaBlue", 2);
			prop.setIntValue("DeltaGreen", 1);
			prop.setIntValue("DeltaRed", 1);
			prop.setIntValue("DxBlue", -1);
			prop.setIntValue("DyBlue", 0);
			prop.setIntValue("DxGreen", 1);
			prop.setIntValue("DyGreen", 1);
			prop.setIntValue("DxRed", 1);
			prop.setIntValue("DyRed", -1);			
			return prop;
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
				prop.getValueAsInt("BlueConstant"),
				prop.getValueAsInt("GreenConstant"),
				prop.getValueAsInt("RedConstant"),
				prop.getValueAsInt("ColorInteraction")
			);
		}
		CProperties getDefaultProperties() {		
			CProperties prop;
			prop.setIntValue("BlueConstant", 500);
			prop.setIntValue("GreenConstant", 500);
			prop.setIntValue("RedConstant", 500);
			prop.setIntValue("ColorInteraction", 0);
			return prop;
		}				
	};	
	
	class BlurFact : public RFactory {
	public:
		std::string getName() { return "Blur";}
		Rule * createRule(const CProperties& prop) {
			return new Blur(prop.getValueAsInt("DisturbanceShift"));
		}	
		CProperties getDefaultProperties() {	
			CProperties prop;
			prop.setIntValue("DisturbanceShift", 12);
			return prop;				
		}			
	};		
	
	class TurnerFact : public RFactory {
	public:
		std::string getName() { return "Turner";}
		Rule * createRule(const CProperties& prop) {
			return new Turner(
				prop.getValueAsInt("Volatility"),
				prop.getValueAsInt("SignBit"),
				prop.getValueAsInt("DirectionBit")
			);
		}		
		CProperties getDefaultProperties() {
			CProperties prop;
			prop.setIntValue("Volatility", 2);
			prop.setIntValue("SignBit", 5);
			prop.setIntValue("DirectionBit", 6);			
			return prop;			
		}			
	};
		
	class HydraFact : public RFactory {
	public:
		std::string getName() { return "Hydra";}
		Rule * createRule(const CProperties& prop) {
			return new Hydra(
				prop.getValueAsInt("BodyLowerLimit"),
				prop.getValueAsInt("HeadLowerLimit")
			);
		}	
		CProperties getDefaultProperties() {
			CProperties prop;
			prop.setIntValue("BodyLowerLimit", 100);
			prop.setIntValue("HeadLowerLimit", 200);
			return prop;			
		}			
	};	
	
	class ColorHydraFact : public RFactory {
	public:
		std::string getName() { return "Color Hydra";}
		Rule * createRule(const CProperties& prop) {
			return new ColorHydra(
				prop.getValueAsInt("BodyLowerLimit"),
				prop.getValueAsInt("HeadLowerLimit")
			);
		}		
		CProperties getDefaultProperties() {
			CProperties prop;
			prop.setIntValue("BodyLowerLimit", 300);
			prop.setIntValue("HeadLowerLimit", 550);
			return prop;			
		}			
	};		
	
	class BraqueFact : public RFactory {
	public:
		std::string getName() { return "Braque";}
		Rule * createRule(const CProperties& prop) {
			return new Braque(
				prop.getValueAsInt("Volatility"),
				prop.getValueAsInt("Shift")
			);
		}		
		CProperties getDefaultProperties() {
			CProperties prop;
			prop.setIntValue("Volatility", 2);
			prop.setIntValue("Shift", 2);
			return prop;			
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
		registerFactory( new BraqueFact(), factories);
		std::cout << factories.size() << " Factories created" << std::endl;
		return factories;
	}	
}
