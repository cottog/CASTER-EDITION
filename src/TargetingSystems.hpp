
namespace TargetingSystems
{
	typedef TCODList<Actor *>(*t_TargetingSystem)(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
	TCODList<Actor *> SelfTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
	TCODList<Actor *> AdjCrtTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
	TCODList<Actor *> BoltTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
	TCODList<Actor *> VisHolocaustTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
	TCODList<Actor *> LvlHolocaustTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
		
	TCODList<Actor *> RdmLOSTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
	TCODList<Actor *> RdmLvlTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
	TCODList<Actor *> AllAdjCrtTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
	TCODList<Actor *> AllCrtrChainTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
	TCODList<Actor *> AllCrtrLvlTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
	TCODList<Actor *> AllCrtrLineTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
	TCODList<Actor *> AllCrtrRadTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
	TCODList<Actor *> OneCrtrLOSTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
	TCODList<Actor *> XCrtrLOSTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
	TCODList<Actor *> AllCrtrLOSTargetingSystem(Actor* caster, SpellCastingConstants::TargetType targetType, SpellCastingConstants::SpellIntensity intensity);
	
}