namespace SpellFactories
{
	
	class TargetingSystemFactory{
	public:
		static TargetingSystems::t_TargetingSystem Build(SpellCastingConstants::TargetSystem targetSystem);
	};
	
	
	class EffectSystemFactory{
	public:
		static EffectSystems::t_EffectSystem Build(SpellCastingConstants::SpellEffect effect);
	};
}