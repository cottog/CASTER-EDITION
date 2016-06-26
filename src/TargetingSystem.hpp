class TargetingSystemBase
{
public:	
	
	TargetingSystemBase()
	{
			expectedTarget = NO_EXPECT;
	}
	
	enum ExpectedTarget {    //whether this particular spell is targeting items, creatures, or people
		NO_EXPECT, ITEM, CREATURE, TILE   //item is as yet unused
	};
	enum TargetType {   //who the spell would prefer to target
		NO_TYPE, NEUTRAL, ENEMY, FRIENDLY
	};
	enum TargetSystem {   //this determines how the spell will select a target
		NO_TARGET,
		SELF, //target the caster
		ADJACENT_TILE, //one random tile next to the caster
		BOLT_SPELL, //travels in a line from caster to enemy and hits the first one in the line
		VISUAL_HOLOCAUST, //choose an enemy, all enemies of that type (in sight) take damage; so, if you pick an orc, all orcs in sight take damage
		LEVEL_WIDE_HOLOCAUST, //exactly as the above, but level-wide instead of restricted to sight
		RANDOM_IN_LOS,  //single random target in LOS
		RANDOM_IN_LEVEL, //single random target in level
		ALL_ADJACENT_TILES, //chooses creatures from the 8 tiles immediately adjacent to the caster
		ALL_CREATURES_CHAINING,  //like chain lightning
		ALL_CREATURES_IN_LEVEL, //chooses all creatures in the level
		ALL_CREATURES_IN_LINE, //penetrating spell; penetrates targets, ignores creatures that aren't targets.
		ALL_CREATURES_IN_RADIUS, //ball spell
		SINGLE_CREATURE_IN_SIGHT, //single chosen target in sight. not random like the above one
		X_CREATURES_IN_SIGHT, //chooses X random creatures in LoS of the caster
		ALL_CREATURES_IN_LOS //chooses all creatures in line of sight
	};

	virtual TCODList<Actor*> FindTargets(Actor* caster) = 0;	
	ExpectedTarget GetExpectedTarget() { return expectedTarget; }
protected:
	ExpectedTarget expectedTarget; //the type of 'thing' the spell will target (items, creatures, floor tiles)
};

class CreatureTargetingSystem : public TargetingSystemBase
{	
public:
	CreatureTargetingSystem(TargetingSystemBase::TargetType target = TargetingSystemBase::NO_TYPE);
	
private:
	TargetType targetType;	//the range and extent of creatures that this individual spell will target
};

class TileTargetingSystem : public TargetingSystemBase
{
public:
	TileTargetingSystem();
	
};

class SelfTargetingSystem : public CreatureTargetingSystem
{
public:
	SelfTargetingSystem() : CreatureTargetingSystem(FRIENDLY){}
	TCODList<Actor*> FindTargets(Actor* caster);
};