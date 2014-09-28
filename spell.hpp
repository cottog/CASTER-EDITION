#include "libtcod.hpp"
class Spell{
public:
	enum SpellIntensity {
		MINOR,MAJOR,NORMAL,EPIC
	};
	enum ExpectedTarget {    //whether this particular spell is targeting items, creatures, or people
		ITEM, CREATURE, TILE
	};
	enum PreferredType {   //who the spell would prefer to target
		ANYONE, ENEMY, FRIENDLY
	};
	enum TargetType {   //this determines how the spell will select a target
		NONE
		SELF,
		ADJACENT_TILE,
		BOLT_SPELL,
		VISUAL_HOLOCAUST,
		LEVEL_WIDE_HOLOCAUST,
		RANDOM_IN_LOS,
		RANDOM_IN_LEVEL,
		ADJACENT_FRIENDLIES, ADJACENT_ENEMIES, ALL_ADJACENT_TILES,
		FRIENDLY_CHAINING, HOSTILE_CHAINING, ALL_CREATURES_CHAINING,
		ALL_FRIENDS_IN_LEVEL, ALL_ENEMIES_IN_LEVEL, ALL_CREATURES_IN_LEVEL,
		ALL_FRIENDLIES_IN_LINE, ALL_ENEMIES_IN_LINE, ALL_CREATURES_IN_LINE,
		SINGLE_FRIENDLY_IN_SIGHT, SINGLE_ENEMY_IN_SIGHT, SINGLE_CREATURE_IN_SIGHT,
		ALL_FRIENDLIES_IN_RADIUS, ALL_ENEMIES_IN_RADIUS, ALL_CREATURES_IN_RADIUS,
		X_FRIENDLIES_IN_SIGHT, X_ENEMIES_IN_SIGHT, X_CREATURES_IN_SIGHT,
		ALL_FRIENDLIES_IN_LOS, ALL_ENEMIES_IN_LOS, ALL_CREATURES_IN_LOS
	};
	enum SpellEffect {  //this is the effect of the spell (pretty straight-forward)
		NONE,
		STRAIGHT_HEAL,
		HEAL_OVER_TIME,
		STAT_BOOST,
		CURE_DEBUFFS,
		DETECTION,
		POLYMORPH,
		SHAPE_SHIFTING,
		LIGHT,          // not sure how an elemental subtype of a light or darkness spell would work
		ILLUSION,
		SIMULACRUM,
		SHIELD,
		REFLECTION,
		ABSORPTION,
		ANTIMAGIC_ZONE,
		DAMAGING_TELEPORT,
		DAMAGING_PULL,
		DAMAGING_PUSH,
		WEAPON_ENHANCEMENT,
		PROTECT_FROM_HAZARDS,
		TELEPORT,
		
		//these ones aren't great for self-targeting spells
		STRAIGHT_DAMAGE,
		BLEED_DAMAGE,
		STAT_DRAIN,
		STAT_SAPPING,
		LIFE_LEECHING,
		MANA_LEECHING,
		INSTAKILL,
		DEBUFFING,
		UNSUMMON,       //would seem to be incompatible with most targeting types (or at least usually useless), but it could attempted to completely destroy a summoned target and just weaken non-summoned targets
		BANISHING,
		MANA_DAMAGE,
		DOOM_TIMER,
		DARKNESS,       // not sure how an elemental subtype of a light or darkness spell would work
		PERCENTILE_DAMAGE,
		RESURRECT,

		//these ones aren't so compatible with all targeting types
		TELEPORTAL,
		ALTER TERRAIN,
		ITEM_CREATION,
		LINK,
		LIFE_TAP,
		SUMMON
		
		
	};
	enum ElementalSubtypes {  //these are the possible subtypes of a spell, determined by how much of each resource is spent on it
		NONE,
		FORCE,
		FIRE,
		WATER,
		EARTH,
		AIR,
		LIGHTNING,
		STEAM,
		LAVA,
		ICE,
		MUD,
		DUST,
		GLASS,
		RADIATION,
		METAL,
		POISON
	};
	
	//TODO: create the LastCast object for casting spell's more quickly
	//TODO: consider creating a tiny description of each spell that indicates what its purpose may be
	//TODO: (not so much for caster module, but for the whole engine) add a function for an actor to determine whether or not it is seen by the player, to keep messages from being displayed in error
	//TODO: implement a 'mana-scent' system, in which some spells whose elemental subtype otherwise wouldn't matter (teleport) can now have an effect. A spell will emit a surge of mana of the elemental subtype it corresponds to, attracting certain creatures
		//maybe a caster with a significantly greater amount of a certain mana will 'smell' strongly of the mana
	
	int ID; //spellID, unique reference INT to identify a spell based on its attributes, not its name
	int *coeffs; //the coefficients in the spell's formula, integral in determining success at casting a spell
	int *powers; //the exponents used in the spell's formula, see above
	int *values; //the values the caster has decided to devote to this spell's casting attempt, related to the above as well
	char *operators; //the operators (+ or *) used in the spell's formula. Pretty much only allowed to be '+' but that may change in the future if I get smarter
	float target; //the target value for a spell's casting. The values are substituted into the formula's variables, and are then modified by the coefficients, exponents, and operators. The closeness of the resultant integer to this target value determines a spell's success.
	SpellIntensity intensity;	//the intensity or strength of a spell. Allows spells to scale with the caster's level. Minor, Normal, Major, or Epic
	TargetType targeting; //the range and extent of creatures that this individual spell will target
	SpellEffect effect; //the effect of a spell (healing, damage, teleporting, etc). This can (and eventually will) form a useless combination with effect and targettype
	ExpectedTarget expected; //the type of 'thing' the spell will target (items, creatures, floor tiles)
	PreferredType preferred; //whether or not the particular spellEffect is supposed to help or harm the player
	
	Spell(int level);
	//TODO: make a means to create a specific spell, rather than a procedurally generated one
		//will be useful to give bosses or uniques specific spells
	//TODO: think of what would be best way to handle wands (procedural, not procedural, some of either)
	float setTarget();
	float cast();
	bool input();	
};