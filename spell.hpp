#include "libtcod.hpp"
class Spell{
public:
	enum SpellIntensity { //the "intensity" of the spell is a means to scale the spell with caster level; higher-intensity spells cost more and have greater or longer-lasting effects
		MINOR,MAJOR,NORMAL,EPIC
	};
	enum ExpectedTarget {    //whether this particular spell is targeting items, creatures, or people
		ITEM, CREATURE, TILE
	};
	enum PreferredType {   //who the spell would prefer to target
		NEUTRAL, ENEMY, FRIENDLY
	};
	enum TargetType {   //this determines how the spell will select a target
		NONE
		SELF, //target the caster
		ADJACENT_TILE, //one random tile next to the caster
		BOLT_SPELL, //travels in a line from caster to enemy and hits the first one in the line
		VISUAL_HOLOCAUST, //choose an enemy, all enemies of that type (in sight) take damage; so, if you pick an orc, all orcs in sight take damage
		LEVEL_WIDE_HOLOCAUST, //exactly as the above, but level-wide instead of restricted to sight
		RANDOM_IN_LOS,  //single random target in LOS
		RANDOM_IN_LEVEL, //single random target in level
		ADJACENT_FRIENDLIES, ADJACENT_ENEMIES, ALL_ADJACENT_TILES, //chooses creatures from the 8 tiles immediately adjacent to the caster
		FRIENDLY_CHAINING, HOSTILE_CHAINING, ALL_CREATURES_CHAINING,  //like chain lightning
		ALL_FRIENDS_IN_LEVEL, ALL_ENEMIES_IN_LEVEL, ALL_CREATURES_IN_LEVEL, //chooses all creatures in the level
		ALL_FRIENDLIES_IN_LINE, ALL_ENEMIES_IN_LINE, ALL_CREATURES_IN_LINE, //penetrating spell; passes through enemies and hits each in a line
		ALL_FRIENDLIES_IN_RADIUS, ALL_ENEMIES_IN_RADIUS, ALL_CREATURES_IN_RADIUS, //ball spell
		SINGLE_FRIENDLY_IN_SIGHT, SINGLE_ENEMY_IN_SIGHT, SINGLE_CREATURE_IN_SIGHT, //single chosen target in sight. not random like the above one
		X_FRIENDLIES_IN_SIGHT, X_ENEMIES_IN_SIGHT, X_CREATURES_IN_SIGHT, //chooses X random creatures in LoS of the caster
		ALL_FRIENDLIES_IN_LOS, ALL_ENEMIES_IN_LOS, ALL_CREATURES_IN_LOS //chooses all creatures in 
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
	//TODO: consider creating a tiny description of each spell that indicates what its purpose may be; this would be achieved by having each spell contain a string or TCODText object and you appending characters to it that would be defined in a wiki for players to read
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
	float cost; //a value used to determine the total cost for casting a spell; this is raised or lowered based on the attributes of the spell (targetType, spellEffect)
	
	Spell(int level);
	//TODO: make a means to create a specific spell, rather than a procedurally generated one
		//will be useful to give bosses or uniques specific spells
	//TODO: think of what would be best way to handle wands (procedural, not procedural, some of either)
	float setTarget(); //this function sets the targetNumber for a spell; this is the value to which the results of the spell's formula are compared in order to see if a spell is successfully cast
	float cast(); //this function resolves the effect of the spell;
	bool input(); //this function is called when a player elects to cast a spell; this takes the amounts of each resource to be spent for the spellcasting and inputs it into the values[] array
};