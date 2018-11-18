#include "main.hpp"

namespace SpellFactories
{
	TargetingSystems::t_TargetingSystem TargetingSystemFactory::Build(SpellCastingConstants::TargetSystem targetSystem)
	{
		TargetingSystems::t_TargetingSystem system;
		
		switch(targetSystem){
			
			case SpellCastingConstants::SELF:
				system = &TargetingSystems::SelfTargetingSystem;
				break;
			case SpellCastingConstants::ADJACENT_TILE:
				system = &TargetingSystems::AdjCrtTargetingSystem;
				break;
			case SpellCastingConstants::BOLT_SPELL:
				system = &TargetingSystems::BoltTargetingSystem;
				break;
			case SpellCastingConstants::VISUAL_HOLOCAUST:
				system = &TargetingSystems::VisHolocaustTargetingSystem;
				break;
			case SpellCastingConstants::LEVEL_WIDE_HOLOCAUST:
				system = &TargetingSystems::LvlHolocaustTargetingSystem;
				break;
			case SpellCastingConstants::RANDOM_IN_LOS:
				system = &TargetingSystems::RdmLOSTargetingSystem;
				break;
			case SpellCastingConstants::RANDOM_IN_LEVEL:
				system = &TargetingSystems::RdmLvlTargetingSystem;
				break;
			case SpellCastingConstants::ALL_ADJACENT_TILES:
				system = &TargetingSystems::AllAdjCrtTargetingSystem;
				break;
			case SpellCastingConstants::ALL_CREATURES_CHAINING:
				system = &TargetingSystems::AllCrtrChainTargetingSystem;
				break; 
			case SpellCastingConstants::ALL_CREATURES_IN_LEVEL:
				system = &TargetingSystems::AllCrtrLvlTargetingSystem;
				break; 
			case SpellCastingConstants::ALL_CREATURES_IN_LINE:
				system = &TargetingSystems::AllCrtrLineTargetingSystem;
				break;
			case SpellCastingConstants::ALL_CREATURES_IN_RADIUS:
				system = &TargetingSystems::AllCrtrRadTargetingSystem;
				break;
			case SpellCastingConstants::SINGLE_CREATURE_IN_SIGHT:
				system = &TargetingSystems::OneCrtrLOSTargetingSystem;
				break;
			case SpellCastingConstants::X_CREATURES_IN_SIGHT:
				system = &TargetingSystems::XCrtrLOSTargetingSystem;
				break;
			case SpellCastingConstants::ALL_CREATURES_IN_LOS:
				system = &TargetingSystems::AllCrtrLOSTargetingSystem;
				break;
			case SpellCastingConstants::NO_TARGET:			
				break;
		}
		
		return system;
	}
	
}