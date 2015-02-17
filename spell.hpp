#include "libtcod.hpp"
class Spell{
public:
	enum SpellIntensity { //the "intensity" of the spell is a means to scale the spell with caster level; higher-intensity spells cost more and have greater or longer-lasting effects
		MINOR,MAJOR,NORMAL,EPIC
	};
	enum ExpectedTarget {    //whether this particular spell is targeting items, creatures, or people
		ITEM, CREATURE, TILE   //item is as yet unused
	};
	enum TargetType {   //who the spell would prefer to target
		NEUTRAL, ENEMY, FRIENDLY
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
	enum SpellEffect {  //this is the effect of the spell (pretty straight-forward)
		NO_EFFECT,
		STRAIGHT_HEAL, //heal damage from the caster (restore HP)
		HEAL_OVER_TIME, //restore hp a small amount for a set number of turns
		STAT_BOOST, //increase a stat or stats
		CURE_DEBUFFS, //remove harmful status effects from the caster
		DETECTION, //detect items or creatures on the specified tiles
		POLYMORPH,      //permanent changing of a creature's form
		SHAPE_SHIFTING, //temporary changing of a creature's form
		LIGHT,          // not sure how an elemental subtype of a light or darkness spell would work
		ILLUSION, //create an illusory version of the caster in the specified tiles
		SIMULACRUM, //create a copy of the target creature a few tiles away from it, that is friendly towards the caster
		SHIELD, //shield the caster from a set amount (and subtype) of damage before disappearing
		REFLECTION, //reflect a set amount (and type) of damage back towards the attacker
		ABSORPTION, //absorb damage done by to the caster by spells of a certain type, refunding mana to the caster and preventing damage
		ANTIMAGIC_ZONE, //create a zone in which magic (or perhaps a certain elemental subtype of magic) cannot be cast (even by the caster) for a certain amount of time
		DAMAGING_TELEPORT, //teleports the target(s) and damages those around the target(s)'s starting location in a radius
		WEAPON_ENHANCEMENT, //gives an elemental brand (and perhaps other stat bonuses) to the equipped weapon(s) of the selected target for a limited amount of time
		PROTECT_FROM_HAZARDS, //protects the target(s) from elemental hazards (lava, glass shards, radiation, etc...), as determined by the elemental subtype of the casting
		TELEPORT, //teleports the target(s) to a random location
		DAMAGING_AURA, //places a damaging circular aura around the target(s) that damages surrounding creatures indiscriminately (enemies, neutrals, and friendlies could be damaged)
		INTELLIGENT_DAMAGING_AURA, //places a damaging circular aura around the the target(s) that damages only the enemies of the target
		
		//these ones aren't great for self-targeting spells
		DAMAGING_PULL, //pulls target(s) strictly towards the caster, while also doing elemental damage to them
		DAMAGING_PUSH, //pushes target(s) strictly away from the caster and damages them. perhaps does extra damage if they hit a wall and cannot be pushed the full distance away
		PULL, //pull target(s) towards the caster
		PUSH, //push target(s) away from the caster
		STRAIGHT_DAMAGE, //does HP damage to the target(s)
		BLEED_DAMAGE, //does small HP damage to the target(s)
		STAT_DRAIN, //reduce the target(s) stat(s)
		STAT_SAPPING, //reduce the target(s) stat(s) and increase the caster's corresponding stat(s)
		LIFE_LEECHING, //do bleed damage to the target(s) and do a corresponding amount of healing over time to the caster
		MANA_LEECHING, //do bleed damage to the target(s)'s mana resource(s) and restore a corresponding amount of the same resource(s) to the caster [do nothing if they have none]
		INSTAKILL, //instantly reduce the target(s) HP to zero
		DEBUFFING, //lower the stats of a target, or inflicts them with certain other status effects (blinded, slowed, confused, etc) for a certain amount of time
		UNSUMMON,       //would seem to be incompatible with most targeting types (or at least it will be usually useless), but it could attempted to completely destroy a summoned target and just weaken non-summoned targets
		BANISHING, //send the target(s) to another dimension, removing them from the map, but not necessarily killing them; hopefully, I will get around to making this "other dimension" an accessible location, so that the player could be sent here (or send himself here) and have to fight the enemies he sends away
		MANA_DAMAGE, //inflict damage to the target(s) mana resource(s) [do nothing if they have none]
		DOOM_TIMER, //set a timer of a set length on the target(s), which decrements each turn; after the timer reaches zero, they are instakilled
		DARKNESS,       // not sure how an elemental subtype of a light or darkness spell would work, but perhaps hide or highly increase the stealth value of a creature that would be masked by the elemental darkness (water creature hide in water-darkness)
		PERCENTILE_DAMAGE, //deals a set percentage of the target(s) max HP
		RESURRECT, //resurrect dead target(s), does nothing to living target(s)
		
		
		//these ones aren't so compatible with all targeting types
		TELEPORTAL, //creates a point on the ground to which the caster will return when he casts a teleportation spell, if he's in range; otherwise, his location upon teleportation will be random
		ALTER_TERRAIN, //adds or removes hazards from the ground (lava pools, shards of glass, etc)
		ITEM_CREATION, //creates an item of artifact quality for the caster. this one will likely be restricted to MAJOR intensity or above
		LINK, //"links" the caster and the target. they will share damage or debuffs or something
		LIFE_TAP, //sacrifices some of the caster's life in exchange for mana of all types
		SUMMON //summons a random creature near the caster
		
		
	};
	enum ElementalSubtypes {  //these are the possible subtypes of a spell, determined by how much of each resource is spent on it
		NO_SUBTYPE,
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
	//TODO: implement a 'mana-scent' system, in which some spells whose elemental subtype otherwise wouldn't matter (regular teleport) can now have an effect. A spell will emit a surge of mana-scent of the elemental subtype it corresponds to, possibly attracting certain creatures based on the subtype
		//maybe a caster with a significantly greater amount of a certain mana will 'smell' strongly of the mana
	
	int ID; //spellID, unique reference INT to identify a spell based on its attributes, not its name
	int *coeffs; //the coefficients in the spell's formula, integral in determining success at casting a spell
	int *powers; //the exponents used in the spell's formula, see above
	int *values; //the values the caster has decided to devote to this spell's casting attempt, related to the above as well
	float target; //the target value for a spell's casting. The values are substituted into the formula's variables, and are then modified by the coefficients, exponents, and operators. The closeness of the resultant integer to this target value determines a spell's success.
	SpellIntensity intensity;	//the intensity or strength of a spell. Allows spells to scale with the caster's level. Minor, Normal, Major, or Epic
	TargetSystem targeting; //the range and extent of creatures that this individual spell will target
	SpellEffect effect; //the effect of a spell (healing, damage, teleporting, etc). This can (and eventually will) form a useless combination with effect and targetsysem
	ExpectedTarget expected; //the type of 'thing' the spell will target (items, creatures, floor tiles)
	TargetType preferred; //the type of enemy (neutral, enemy, or ally) the spell would optimally target; of course, the spell could potentially do the exact opposite
	TargetType actual; //the type of enemy (neutral, enemy, or ally) that the spell actually targets, as opposed to the preferred type
	float cost; //a value used to determine the total cost for casting a spell; this is raised or lowered based on the attributes of the spell (targetsystem, spellEffect)
	
	Spell(int level);
	//TODO: make a means to create a specific spell, rather than a procedurally generated one
		//will be useful to give bosses or uniques specific spells
	//TODO: think of what would be best way to handle wands (procedural, not procedural, some of either)
	float setTarget(); //this function sets the targetNumber for a spell; this is the value to which the results of the spell's formula are compared in order to see if a spell is successfully cast
	float cast(); //this function resolves the effect of the spell;
	bool input(); //this function is called when a player elects to cast a spell; this takes the amounts of each resource to be spent for the spellcasting and inputs it into the values[] array
};